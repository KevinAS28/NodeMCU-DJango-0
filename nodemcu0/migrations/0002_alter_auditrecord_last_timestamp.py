# Generated by Django 3.2.7 on 2022-01-05 07:39

import datetime
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('nodemcu0', '0001_initial'),
    ]

    operations = [
        migrations.AlterField(
            model_name='auditrecord',
            name='last_timestamp',
            field=models.DateTimeField(default=datetime.datetime.now),
        ),
    ]
