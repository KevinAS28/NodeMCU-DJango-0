
from channels.generic.websocket import WebsocketConsumer

class DictWebSocketConnections(dict):

    def __init__(self, *args, **kwargs):
        super(dict, self).__init__(*args, **kwargs)
        self.connections:dict = self
        self.used_connection_ids = []
        # self.connection_max_count = 10000
    
    # def get_connection_id(self):
    #     count = 0
    #     while (count < self.connection_max_count):
    #         if not (count in self.used_connection_ids):
    #             self.used_connection_ids.append(count)
    #             return count
    #     raise ValueError(f'Maximum connection count ({self.connection_max_count}) has been reached! Cannot get a connection_id')

    def add_connection(self, name:str, group:str, connection:WebsocketConsumer):
        
        if not group in self.connections:
            self.connections[group] = {name: [connection]}
            return 0
        elif not name in self.connections[group]:
            self.connections[group][name] = [connection]
            return 0
        else:
            connection_id = len(self.connections[group][name])
            self.connections[group][name].append(connection)
            return connection_id
            
    def find_connections(self, connection_id:int, group:str='', name:str=''):
        possible_groups = list(self.connections.keys()) if group=='' else [group]
        possible_names = [list(gp.keys()) for gp in possible_groups] if name=='' else [name]

        founded_connections = []
        for pg in possible_groups:
            for pn in possible_names:
                possible_connections = self.connections[pg][pn]
                if connection_id < len(possible_connections):
                    connection = possible_connections[connection_id]
                    founded_connections.append({'name': pn, 'group': pg, 'connection_id': connection_id, 'connection': connection})                    
        return founded_connections

    def delete_connection(self, connection_id:int, group:str='', name:str='', raise_not_found=True):
        connections_to_delete = self.find_connections(connection_id, group, name)
        if raise_not_found:
            if len(connections_to_delete) < 1:
                raise ValueError(f'Cannot delete WebSocket Connection with connection_id={self.connection_id}, channel_name={self.channel_name}, channel_group_name={self.channel_group_name}: no connection found')

        for connection_dict in connections_to_delete:
            del self.connections[connection_dict['group']][connection_dict['name']][connection_dict['connection_id']]
        return len(connections_to_delete)

