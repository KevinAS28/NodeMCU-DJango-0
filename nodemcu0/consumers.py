import json
import time
import traceback

from asgiref.sync import async_to_sync
from .consumers_support import *

class hashabledict(dict):
    def __hash__(self):
        return hash(tuple(sorted(self.items())))


all_websocket_connections = DictWebSocketConnections()
nodemcu_devices_connections = DictWebSocketConnections()

class DefaultConsumer(WebsocketConsumer, dict):
    # def __init__(self, *args, **kwargs):
    #     super(WebsocketConsumer, self).__init__(*args, **kwargs)

    connection_keys = [
        'connection_id',
        'room_name',
        'room_group_name',
        'channel_name',
        
        'device_info'
    ]

    saved_responses = dict()

    def get_saved_response_id(self):
        return len(self.saved_responses)

    def add_saved_responses(self, response, saved_id=None):
        response_id = self.get_saved_response_id() if saved_id is None else saved_id
        self.saved_responses[response_id] = response
        return response_id

    def to_dict(self):

        super(dict, self).__init__()
        for key in self.connection_keys:
            if hasattr(self, key):
                self[key] = getattr(self, key)
        return dict(self)

    def connect(self):
        self.list_connection_lists = [all_websocket_connections]

        self.room_name = self.scope['url_route']['kwargs']['room_name']
        self.room_group_name = 'room_group_%s' % self.room_name

        # Join group
        async_to_sync(self.channel_layer.group_add)(
            self.room_group_name,
            self.channel_name
        )

        self.accept()

        self.connection_id = all_websocket_connections.add_connection(self.channel_name, self.room_group_name, self)

        self.to_dict()

    def disconnect(self, close_code):

        # Leave group
        async_to_sync(self.channel_layer.group_discard)(
            self.room_group_name,
            self.channel_name
        )

        for connection_list in self.list_connection_lists:
            connection_list.delete_connection(self.connection_id, self.room_group_name, self.channel_name)
    
    def parse_consumer_event(self, event, response_type_request='null', response_id=None, only_get_header=False):
        parsed = {'data': event}

        json_header = dict()
        if not (response_id is None):
            json_header['response_id'] = response_id
        if response_type_request != 'null':
            json_header['response_type_request'] = response_type_request
        
        if only_get_header:
            return json_header
        
        parsed['json_header'] = json_header
        return parsed
    
    def run_consumer(self, input_type, event, response_type_request='null', auto_event=True, wait_response=False):
        response_id = None
        if wait_response:
            response_id = self.get_saved_response_id()

        if auto_event:
            event = self.parse_consumer_event(event, response_type_request, response_id=response_id)
        else:
            event_header = self.parse_consumer_event(event, response_type_request, response_id=response_id, only_get_header=True)
            event['json_header'] = event_header

        if hasattr(self, input_type):
            method = getattr(self, input_type)
            method(event)

        
        return response_id

    # Receive message from WebSocket
    def receive(self, text_data, *args, **kwargs):
        # print('args:', args)
        # print('kwargs:', kwargs)
        try:
            text_data_json = json.loads(text_data)
            json_data = text_data_json['data']

            input_type = 'broadcast_data'
            if ('input_type' in text_data_json):
                inp_type = text_data_json['input_type']
                if hasattr(self, inp_type):
                    input_type = inp_type
            
            room_group_name_list = [self.room_group_name]
            if ('room_group_name_list' in text_data_json):
                room_group_name_list = text_data_json['room_group_name_list']

            response_type = 'bc'
            if ('response_type' in text_data_json):
                response_type = text_data_json['response_type']

            room_group_name_map = {
                0: self.room_group_name,
            }

            get_room_group_name = lambda room_group_name: room_group_name if not (room_group_name in room_group_name_map) else room_group_name_map[room_group_name]

            if response_type=='121':
                self.run_consumer(inp_type, json_data)
            elif response_type=='bc_all':
                # Send data to all groups
                for room_group_name in room_group_name_list:
                    async_to_sync(self.channel_layer.group_send)(
                        get_room_group_name(room_group_name),
                        {
                            'type': input_type,
                            'data': json_data
                        }
                    )
            elif response_type=='121_save':
                # Send data to all groups
                response_id = text_data_json['response_id']
                self.add_saved_responses(json_data, response_id)
            elif response_type=='bc_spc':
                # Still not working
                # Send data to specified connections
                connection_responses = dict()
                for connection_target in text_data_json['connections_specified']:
                    con_id = connection_target['connection_id']
                    con_group_name = connection_target['room_group_name']
                    con_channel_name = connection_target['channel_name']
                    connection = all_websocket_connections.find_connections(connection_id=con_id, group=con_group_name, name=con_channel_name)
                    if len(connection) < 0:
                        connection_responses[connection_target] = {'success': False, 'response': None, 'error': 'Connection not found'}
                        continue
                    connection = connection[0]['connection']
                    resp_id = connection.run_consumer(inp_type, json_data, '121', wait_response=True)
                    connection_responses[hashabledict(connection_target)] = connection.wait_response(resp_id)
                    json_data['bc_spc_responses'] = {'success': True, 'response': connection_responses}
                self.run_consumer(inp_type, json_data)
            else:
                raise ValueError(f'Invalid response_type: {response_type}')
        except:    
            traceback.print_exc()
    
    def send_to_group(self, input_type, json_data, group_name=None):
        # Send data to group
        async_to_sync(self.channel_layer.group_send)(
            self.room_group_name if group_name is None else group_name,
            {
                'type': input_type,
                'data': json_data
            }
        )
    
    def wait_response(self, response_id, timeout_sec=10):
        timer = 0
        check_resp_id = lambda resp_id: ((response_id in self.saved_responses))
        while (not check_resp_id(response_id)) and (timer<timeout_sec):
            timer+=0.1
            time.sleep(0.1)
        
        if not check_resp_id(response_id):
            return {'success': False, 'response': None, 'error': 'Target response timeout'}

        return self.saved_responses[response_id]

    def send_json_with_header(self, event, json_data, *args, **kwargs):
        tosend = {'json_header': event['json_header'] if 'json_header' in event else dict(), 'data': json_data}
        self.send(text_data=json.dumps(tosend), *args, **kwargs)

    def broadcast_data(self, event):
        data = event['data']

        # Send message to WebSocket
        self.send_json_with_header(event, json_data=data)

    def __str__(self):
        return json.dumps(self.to_dict())
    
    def __hash__(self):
        return hash(str(self))

    def __eq__(self, other):
        for key in self.connection_keys:
            if hasattr(self, key) and hasattr(other, key):
                if getattr(self, key)==getattr(other, key):
                    continue
            return False
        return True

class MainConsumer(DefaultConsumer):
    # Receive message from room group

    def get_all_connections(self, event):
        connections_info = {
            'connections': all_websocket_connections,
            'count': len(all_websocket_connections)
        }
        
        self.send_json_with_header(event, json_data=(connections_info))
    
    def register_nodemcu(self, event):
        self.device_connection_id = nodemcu_devices_connections.add_connection(self.channel_name, self.room_group_name, self)
        self.list_connection_lists.append(nodemcu_devices_connections)
        json_data = event['data']
        self.device_info = json_data['device_info']
        self.to_dict()

        self.send_json_with_header(event, json_data={'success': True})
    
    def get_nodemcu_connections(self, event):
        connections_info = {
            'connections': nodemcu_devices_connections,
            'count': len(nodemcu_devices_connections)
        }
        
        self.send_json_with_header(event, json_data=(connections_info))
