from datetime import datetime
import random
from string import ascii_letters

from django.db import models
from django.db.models.fields import DateTimeField


# Create your models here.

class AuditRecord(models.Model):
    def random_char(n=50):
        return ''.join(random.sample(ascii_letters, n))
        
    device_id = models.CharField(max_length=6)
    timestamp = DateTimeField(default=datetime.now)
    data = models.CharField(max_length=50, default=random_char)