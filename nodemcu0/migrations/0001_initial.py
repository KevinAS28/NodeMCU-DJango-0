# Generated by Django 3.2.7 on 2022-01-05 07:37

import datetime
from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='AuditRecord',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('device_id', models.CharField(max_length=6)),
                ('last_timestamp', models.DateTimeField(default=datetime.datetime(2022, 1, 5, 7, 37, 25, 697159))),
            ],
        ),
    ]
