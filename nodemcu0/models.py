from django.db import models
from django.db.models.fields import DateTimeField

from datetime import datetime



# Create your models here.

class AuditRecord(models.Model):
    device_id = models.CharField(max_length=6)
    last_timestamp = DateTimeField(default=datetime.now)