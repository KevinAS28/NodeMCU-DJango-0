#include "tools.h"
#include "conf.h"

std::string json_register_nodemcu = string_format("{\
    \"data\": {\
        \"device_info\": {\
            \"device_id\": \"%s\"\
        }\
    },\
    \"input_type\": \"register_nodemcu\"\
}", DEVICE_ID);

std::string json_test_nodemcu = string_format("{\
    \"data\": {\
        \"time\": \"%s\"\
    },\
    \"input_type\": \"register_nodemcu\"\
}", get_time_string());
