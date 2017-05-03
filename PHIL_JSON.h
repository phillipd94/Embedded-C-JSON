#IFNDEF PHIL_JSON
#DEFINE PHIL_JSON

#include "jsmn.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int remove_slash(char*b);
int check_JSMN_error(int numtok, char* location);
void output_string(char* s);
int extract_value(char* JSONSTR, char* key, char *target);
int extract_deep_value(char* JSONSTR, char* key0, char* key1,char* key2, char *target);
int extract_deep_value(char* JSONSTR, char* key0, char* key1, char *target);
int pack_json(char* format, char* target, ...);
int count_slash(char*b);

#ENDIF