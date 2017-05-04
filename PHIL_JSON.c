#include "PHIL_JSON.h"
#include <string.h>


int remove_slash(char*b) {//removes all instances of a character from a string, useful for removing stray "/" from json strings
	for (int i = 0; i < strlen(b); i++) {
		if ((b[i] == '\\') && ((b[i + 1] == '"'))) {
			memmove((void*)&b[i], (void*)&b[i + 1], strlen(b) - i);
		}
		if ((b[i] == '\\') && ((b[i - 1] == '"'))) {
			memmove((void*)&b[i - 1], (void*)&b[i], strlen(b) - i);
		}
	}
	return 0;
}

int count_slash(char*b) {//removes all instances of a character from a string, useful for removing stray "/" from json strings
	char* n = b;
	int i = 0;
	int count = 0;
	int running = 0;
	while (n[i] != '\0') {
		if (b[i] == '\\') {
			count++;
		}
		if ((b[i] != '\\') && (count != 0) && (count > running)) {
			running = count;
			count = 0;
		}
		i++;
	}
	return running;
}

int check_JSMN_error(int numtok, char* location) {//Prints error and error location to the location specified by output_string definition, returns 1 if error is detected
	if (numtok == JSMN_ERROR_INVAL) {//Error Checking
		char errorString[50] = "ERROR:INVALID JSON IN ";
		strcat(errorString, location);
		output_string(errorString);
		output_string("\n");
		return 1;
	}
	if (numtok == JSMN_ERROR_NOMEM) {
		char errorString[50] = "ERROR:MEMORY OVERFLOW IN ";
		strcat(errorString, location);
		output_string(errorString);
		output_string("\n");
		return 1;
	}
	if (numtok == JSMN_ERROR_PART) {
		char errorString[50] = "ERROR:MORE DATA EXPECTED IN ";
		strcat(errorString, location);
		output_string(errorString);
		output_string("\n");
		return 1;
	}
	return 0;
}

void output_string(char* s) {//redefine this to fit whatever system you're working on, for example, on an embedded system reimpliment this as output to TX buffer
	printf(s);
}


int pack_json(char* format, char* target, ...) {//works similar to printf: example format string "{s:b,s:{s:[n,n,n,n]}}"  b is bool, n is float, s is string, returns 1 on failure
	char* s;
	float f;
	char m[100];

	va_list arg;
	va_start(arg, target);

	int z = 0;
	for (int j = 0; j < strlen(format); j++) {
		if ((format[j] == '{') || (format[j] == '}') || (format[j] == ':') || (format[j] == ',') || (format[j] == '"') || (format[j] == ']') || (format[j] == '[')) {
			target[z] = format[j];
			z++;
		}
		else {
			if (format[j] == 's') {
				s = va_arg(arg, char*);
				target[z] = '"';
				z++;
				for (int n = 0; n < strlen(s); n++) {
					target[z] = s[n];
					z++;
				}
				target[z] = '"';
				z++;
			}
			if (format[j] == 'n') {
				f = va_arg(arg, double);
				sprintf(m, "%g", f);
				for (int n = 0; n < strlen(m); n++) {
					target[z] = m[n];
					z++;
				}
			}
			if (format[j] == 'b') {
				s = va_arg(arg, char*);
				if ((s[0] == 't') || (s[0] == 'f')) {
					for (int n = 0; n < strlen(s); n++) {
						target[z] = s[n];
						z++;
					}
				}
				else {
					return 1;
				}
			}
		}
	}
	va_end(arg);
	target[z] = '\0';
	return 0;
}

int extract_value(char* JSONSTR, char* key, char *target) {

	char output1[256];
	jsmn_parser parser;
	jsmn_init(&parser);
	jsmntok_t tokens1[50];
	int numtok1 = jsmn_parse(&parser, JSONSTR, strlen(JSONSTR), tokens1, 50);//tokenize JSON string
	if (check_JSMN_error(numtok1, "EXTRACTION")) {//Error Checking
		return 5;
	}
	for (int i = 0; i < numtok1; i++) {//Loop through all the tokens
		memcpy(output1, (const char*)&JSONSTR[tokens1[i].start], abs(tokens1[i].start - tokens1[i].end));//copy the token of interest into a separate string
		output1[abs(tokens1[i].start - tokens1[i].end)] = '\0';
		if (strcmp(output1, key) == 0) {//Check to see if the token matches the specified key
			memcpy(target, &JSONSTR[tokens1[i + 1].start], abs(tokens1[i + 1].start - tokens1[i + 1].end));//copy the response type information into the target string location
			target[abs(tokens1[i + 1].start - tokens1[i + 1].end)] = '\0';//terminate the target string
			return 0;
		}
	}
	return 1;
}


int extract_value_2(char* JSONSTR, char* key0, char* key1, char* key2, char *target)
{
	char output[256];
	char output1[256];
	int a, b, c, d = 1;
	d = count_slash(JSONSTR);
	if (d != 6) {
		return 1;
	}
	a = extract_value(JSONSTR, key0, output);
	remove_slash(output);
	remove_slash(output);
	/*printf("OUTPUTREMOVEDSLASH=");
	printf(output);
	printf("\n");*/
	b = extract_value(output, key1, output1);
	remove_slash(output1);
	c = extract_value(output1, key2, target);
	if (!a && !b && !c) {
		return 0;
	}
	else {
		return 1;
	}
}

int extract_value_1(char* JSONSTR, char* key0, char* key1, char *target)
{
	char output[256];
	int a, b, d = 1;
	d = count_slash(JSONSTR);
	if (d != 2) {
		return 1;
	}
	a = extract_value(JSONSTR, key0, output);
	remove_slash(output);
	b = extract_value(output, key1, target);
	if (!a && !b) {
		return 0;
	}
	else {
		return 1;
	}
}

