import json
import re

from django.shortcuts import render
from django.http import JsonResponse
from django.forms.models import model_to_dict
import requests
# from urllib.parse import urljoin

import nodemcu0.models as models0
# from .forms import *
from .devices import *

# Create your views here.



def index(request):
    return render(request, 'chat/index.html')


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

def switch_led_multi(request):
    if request.method=="POST":
        POST = request.POST
        device_id = POST['device_id']
        LED_switches = {led: False for led in DEVICEID_LED[device_id]}
        for key in POST:
            re_result = re.search(r'^([a-z]{1,})(\d)$', key)
            if re_result:
                LED_switches[int(re_result.groups()[1])] = True
        
        for led, switch in LED_switches.items():
            if switch:
                data = {'led': led}
                requests.post(url=f'http://{DEVICEID_IP[device_id]}/switchledmulti', data=json.dumps(data))

        return render(request, 'multi_led_switch.html')
    else:
        return render(request, 'multi_led_switch.html')
    

def send_json(request):
    data = json.loads(request.body)
    all_responses = dict()
    for device_id, json_str in data['device_ids_jsons'].items():
        response = requests.post(url=f'http://{DEVICEID_IP[device_id]}/postjson', data=json.dumps(json_str))
        all_responses[device_id] = (response.text)
        # print(f'Response from {device_id}: {str(response.text)}')
    return JsonResponse({'all_responses': all_responses})