
#ifndef BOOL_H
#define BOOL_H
typedef enum { false, true } bool;
#endif

#ifndef PARSER_H
#define PARSER_H
typedef long (*f1)(char *);
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

Parser *createParser();
char * getVal(char *, char *);

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
#endif
