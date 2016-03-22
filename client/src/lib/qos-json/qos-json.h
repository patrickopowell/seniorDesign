#include <jansson.h>
#include <stdio.h>

#define STRING_TYPE 0
#define INTEGER_TYPE 1
#define ARRAY_TYPE 2

struct protocol_tuple {
	char *name;
	int type;
	protocol_tuple sub_tuples[];
};

