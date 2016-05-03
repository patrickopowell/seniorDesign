#include <stdlib.h>

typedef enum { false, true } bool;

typedef struct Parser{
	long (*getClientID)(char *str);
	long (*getProtocolVersion)(char *str);
	long (*vetVersion)(char *str);
	long (*getStorageType)(char *str);
	long (*getIopsMax)(char *str);
	long (*getIopsMin)(char *str);
	long (*getThroughputMax)(char *str);
	long (*getThroughputMin)(char *str);
	long (*getUnused)(char *str);
};

long getID(char *str){
	bool found = false;
	long ID = 0;
	char temp[sizeof(long)]; // This will temporarily hold chars as client_id until I transform them into long.
	char *comparison = "client_id:";
	for(int i = 0; (str[i] != '\0') && (!found); i++){
		for(int j = 0; j < 10; j++){ // 10 is len of "client_id:"
			if((str[i+j] == comparison[j]) && (j == 9)){ // it is at the end of our key. Time for value.
				for(int k = 0; str[i+k] != ' '; k++){
					temp[k] = str[i+k];
				}
				found = true;
				break;
			}
		}
	}
	ID = atol(temp);
	return ID;
}

/**
 * Give it a key, and it returns the value.
 */
char * getVal(char *key){
	char temp[sizeof(long)];
}











