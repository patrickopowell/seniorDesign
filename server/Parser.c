#include <stdlib.h>
#include <string.h>

/* Function Prototypes */
char *getVal(char *, char *);
long getClientID(char *);
long getProtocol(char *);
long getVersion(char *);
long getStorageType(char *);
long getIopsMax(char *);
long getIopsMin(char *);
long getThroughputMax(char *);
long getThroughputMin(char *);
long getUnusedBandwidth(char *);

/* Typedeff the bool */
typedef enum { false, true } bool;

/* Typedeffing the function for my struct. */
typedef long (*f1)(char *);

/*************************************************************************************************
 * My Parser struct. C is a real pain in my ass.
 * USAGE: Parser *p = createParser();
 * 		  long client_id = p->F1(test); // where test is a JSON string containing key, "client_id"
 ************************************************************************************************/
typedef struct {
    f1 F1; // get client id
    f1 F2; // get protocol version
    f1 F3; // get version
    f1 F4; // get storage type
    f1 F5; // get IOPS max
    f1 F6; // get IOPS min
    f1 F7; // get throughput max
    f1 F8; // get throughput min
    f1 F9; // get unused bandwidth
} Parser;

/*************************************************************************************************
 * This creates a parser.
 * USAGE: Parser *p = createParser();
 ************************************************************************************************/
Parser *createParser(){
    Parser *temp = (Parser *)malloc(sizeof(Parser));
    temp->F1 = getClientID;
    temp->F2 = getProtocol;
    temp->F3 = getVersion;
    temp->F4 = getStorageType;
    temp->F5 = getIopsMax;
    temp->F6 = getIopsMin;
    temp->F7 = getThroughputMax;
    temp->F8 = getThroughputMin;
    temp->F9 = getUnusedBandwidth;
    return temp;
}

/* Self Explanatory*/
long getClientID(char *str){
	char *value = getVal("client_id", str);
    long temp = atol(value);
    free(value); // Always make sure to free any memory that was allocated!
	return temp;
}

long getProtocol(char *str){
	char *value = getVal("protocol_version_number", str);
    long temp = atol(value);
    free(value); // Always make sure to free any memory that was allocated!
	return temp;
}

long getVersion(char *str){
	char *value = getVal("version", str);
    long temp = atol(value);
    free(value); // Always make sure to free any memory that was allocated!
	return temp;
}

long getStorageType(char *str){
	char *value = getVal("storage_type", str);
    long temp = atol(value);
    free(value); // Always make sure to free any memory that was allocated!
	return temp;
}

long getIopsMax(char *str){
	char *value = getVal("iops_max", str);
    long temp = atol(value);
    free(value); // Always make sure to free any memory that was allocated!
	return temp;
}

long getIopsMin(char *str){
	char *value = getVal("iops_min", str);
    long temp = atol(value);
    free(value); // Always make sure to free any memory that was allocated!
	return temp;
}

long getThroughputMax(char *str){
	char *value = getVal("throughput_max", str);
    long temp = atol(value);
    free(value); // Always make sure to free any memory that was allocated!
	return temp;
}

long getThroughputMin(char *str){
	char *value = getVal("throughput_min", str);
    long temp = atol(value);
    free(value); // Always make sure to free any memory that was allocated!
	return temp;
}

long getUnusedBandwidth(char *str){
	char *value = getVal("SLA_unused", str);
    long temp = atol(value);
    free(value); // Always make sure to free any memory that was allocated!
	return temp;
}

/**
 * Give it a key, and it returns the value.
 * str is the whole JSON string.
 */
char * getVal(char *key, char *str){
	bool found = false;
	int len_key = strlen(key);
	int len_str = strlen(str);
	char *temp = malloc(sizeof(char)*len_str); // This will temporarily hold the value chars.
	for(int i = 0; (i < len_str) && (!found); i++){
		for(int j = 0; j < len_key; j++){
			if((str[i+j] == key[j]) && (j == (len_key-1))){ // it is at the end of our key. Time for value.
				for(int k = 0; str[i+j+k+3] != ' '; k++){ // + 3 to skip the comma
					temp[k] = str[i+j+k+2]; // the +2 to skip the ':'
				}
				found = true;
				break;
			}
		}
	}
	return temp;
}


