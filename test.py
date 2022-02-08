import json
import websocket
from nodemcu0.jsonwsc import *

data1 = JsonWscRequestPacket(JsonWscHeader('broadcast_data', JsonWscResponseType(JsonWscResponseTypeEnum.BC_ALL)), multiswitchled_commands_arguments({'0': False, '1': True}))

print(data1)

bc_data = 'broadcast_data'
get_nc = 'get_nodemcu_connections'
get_all = 'get_all_connections'
reg_nc = 'register_nodemcu'

data0 = {"message": "Kevin", "data": {
    "device_info": {
        "device_id": "NODMC1"
       }
    }, 
    "json_data": "Kevin Data",
    "input_type": get_all,
    'response_type': 'bc_all',
    'room_group_name_list': ['send_nodemcu', 0]
}

ws = websocket.WebSocket()
ws.connect('ws://127.0.0.1:8000/ws/main/send_nodemcu/')

print('sent:', ws.send(json.dumps(data1.__dict__())))
resp = json.loads(ws.recv())
print('resp:', resp)

