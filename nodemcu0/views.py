import json

from django.shortcuts import render
from django.http import JsonResponse
from django.forms.models import model_to_dict


import nodemcu0.models as models0

# Create your views here.

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