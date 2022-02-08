
from django.urls import path
from django.contrib import admin


from nodemcu0.views import *

app_name = 'nodemcu0'

urlpatterns = [ 
    path('', chat_index, name='index'),
    path('room/<str:room_name>/', chat_room, name='room'),

    path('admin/', admin.site.urls),
    path('audit_record', audit_record),
    path('get_records', get_records),
    path('switchled', switch_led),
    path('switchledmulti', switch_led_multi),
    path('sendjson', send_json)
]