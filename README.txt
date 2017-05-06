This is a library of helper functions that uses the jsmn library to parse nested JSON strings.  
The original design intent was to provide a simple series or functions to parse output messages from the wifi bridge used in ECE 631 taught by Jay Hermmann.

extracting a value from a JSON string is as simple as calling the function extract_value with the arguments being the JSON string, the key associated with the desired value, and the buffer (array) you would like the output written to.

This library also contains functions that will automatically parse nested JSON strings, and take multiple key arguments, one for each layer.  Examples below:

Example: Parsing a value from a double nested string

char input[256] = "{\"Response\":\"WifiSetup\",\"Message\":{\"Wifi\":\"{\\\"Result\\\":\\\"Success\\\",\\\"IP\\\":\\\"192.168.123.135\\\"}\"}}";
char output[256];

extract_value_2(input,"Message","Wifi","Result",output);

printf(output);

This code will print "Success", after extracting it from the innermost string as the value associated with the key "Result"