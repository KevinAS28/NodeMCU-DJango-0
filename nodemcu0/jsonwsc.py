import json, enum

class JsonWscResponseTypeEnum():
    BC_ALL = 'bc_all'
    ONE2ONE = '121'
    ONE2ONE_SAVE = '121_save'
    BC_SPC = 'bc_spc'

class JsonWscResponseType:
    def __init__(self, response_type, *args, **kwargs):
        self.response_type = response_type
        # if self.response_type == JsonWscResponseTypeEnum.BC_SPC:
    
    def __dict__(self):
        return {'response_type': str(self.response_type)}

class JsonWscHeader:
    def __init__(self, input_type, response_type: JsonWscResponseType):
        self.input_type = input_type
        self.response_type = response_type
    
    def __dict__(self):
        header = {
            'input_type': self.input_type,
            **self.response_type.__dict__(),            
        }

        return header
    
    def __str__(self):
        return json.dumps(self.__dict__())

def multiswitchled_commands_arguments(led_switchs):
    return {
        'type': 'json_command',
        'command_arguments': [
            {
                'command': 'multi_switch_led',
                'arguments': {
                    'LED_switchs': led_switchs
                }
            }
        ]
    }

class JsonWscRequestPacket:
    def __init__(self, header: JsonWscHeader, data):
        self.header = header
        self.data = data
    
    def __dict__(self):
        header = self.header.__dict__()
        header['data'] = self.data
        return header
    
    def __str__(self):
        return json.dumps(self.__dict__())