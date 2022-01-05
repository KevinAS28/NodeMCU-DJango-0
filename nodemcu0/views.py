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

def get_audit(request):
    audit_records_data = [model_to_dict(i) for i in models0.AuditRecord.objects.all()]
    return JsonResponse({'audit': audit_records_data})