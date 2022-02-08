import json
import re
from threading import Thread

from django.shortcuts import render
from django.http import JsonResponse
from django.forms.models import model_to_dict
import requests
import websocket
# from urllib.parse import urljoin

import nodemcu0.models as models0
# from .forms import *
from .devices import *
from nodemcu0.jsonwsc import *

# Create your views here.

def chat_index(request):
    return render(request, 'chat/index.html')

def chat_room(request, room_name):
    return render(request, 'chat/room.html', {
        'room_name': room_name
    })

def audit_record(request):
    data = json.loads(request.body)
    ar = data['audit']
    models0.AuditRecord(**ar).save()
    return JsonResponse({'success': True})

def get_records(request):
    data = json.loads(request.body)

    if 'limit' in data:
        limit = int(data['limit'])
        queryset = models0.AuditRecord.objects.all().order_by('-timestamp')[:limit]
    else:
        queryset = models0.AuditRecord.objects.all()

    audit_records_data = [model_to_dict(i) for i in queryset]
    return JsonResponse({'audit': audit_records_data})

def switch_led(request):
    data = json.loads(request.body)
    all_responses = dict()
    for device_id in data['device_ids']:
        all_responses[device_id] = (requests.get(url=f'http://{DEVICEID_IP[device_id]}/switchled').text)
    return JsonResponse({'all_responses': all_responses})

# def switch_led_multi(request):
#     if request.method=="POST":
#         POST = request.POST
#         device_id = POST['device_id']
#         LED_switches = {led: False for led in DEVICEID_LED[device_id]}
#         for key in POST:
#             re_result = re.search(r'^([a-z]{1,})(\d)$', key)
#             if re_result:
#                 LED_switches[int(re_result.groups()[1])] = True
        
#         for led, switch in LED_switches.items():
#             if switch:
#                 data = {'led': led}
#                 requests.post(url=f'http://{DEVICEID_IP[device_id]}/switchledmulti', data=json.dumps(data))

#     return render(request, 'multi_led_switch.html', context={'device_id_list': ['NODMC0']})

def switch_led_multi(request):
    if request.method=="POST":
        POST = request.POST
        device_id = POST['device_id']
        LED_switches = {led: False for led in DEVICEID_LED[device_id]}
        for key in POST:
            re_result = re.search(r'^([a-z]{1,})(\d)$', key)
            if re_result:
                LED_switches[str(int(re_result.groups()[1]))] = True
        
        data = JsonWscRequestPacket(
            JsonWscHeader('broadcast_data', JsonWscResponseType(JsonWscResponseTypeEnum.BC_ALL)), 
            multiswitchled_commands_arguments(LED_switches)
        )

        def _send(): ws = websocket.WebSocket();ws.connect('ws://127.0.0.1:8000/ws/main/send_nodemcu/'); print('sent:', ws.send(json.dumps(data.__dict__()))); print('resp:', json.loads(ws.recv())); ws.close()
        Thread(target=_send, args=[]).start()

    
    return render(request, 'multi_led_switch.html', context={'device_id_list': ['NODMC0']})


def send_json(request):
    data = json.loads(request.body)
    all_responses = dict()
    for device_id, json_str in data['device_ids_jsons'].items():
        response = requests.post(url=f'http://{DEVICEID_IP[device_id]}/postjson', data=json.dumps(json_str))
        all_responses[device_id] = (response.text)
        # print(f'Response from {device_id}: {str(response.text)}')
    return JsonResponse({'all_responses': all_responses})