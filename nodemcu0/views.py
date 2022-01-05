import json

from django.shortcuts import render
from django.http import JsonResponse


import nodemcu0.models as models0

# Create your views here.

def audit_record(request):
    data = json.loads(request.body)
    ar = data['audit']

    models0.AuditRecord(**ar).save()
    return JsonResponse({'success': True})