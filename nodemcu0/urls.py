
from django.urls import path

from nodemcu0.views import *

app_name = 'nodemcu0'

urlpatterns = [ 
    path('audit_record', audit_record),
    path('get_records', get_records),
    path('switchled', switch_led),
    path('switchledmulti', switch_led_multi),
    path('sendjson', send_json)
]