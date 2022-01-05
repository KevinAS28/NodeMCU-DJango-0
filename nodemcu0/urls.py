
from django.urls import path

from nodemcu0.views import *

app_name = 'nodemcu0'

urlpatterns = [ 
    path('audit_record', audit_record),
]