#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

static const char* NDED = "NEW_DISEASE_ENTER_DESCRIPTION";
static const char* NDCD = "NEW_DISEASE_COPY_DESCRIPTION";
static const char* CD = "CHANGE_DESCRIPTION";
static const char* PD = "PRINT_DESCRIPTION";
static const char* DPD = "DELETE_PATIENT_DATA";

static int stoi(char* str, int l){
    int i, n = 0;
    for(i = 0; i < l; i++){
        n *= 10;
        n += (int)str[i] - 48;
    }
    return n;
}

inline static void parseNDED(char *str, ReturnData d){
    long i = strchr(str, ' ') - str;
    d->string1 = (char*)malloc(sizeof(char) * (i + 1));
    strncpy(d->string1, str, sizeof(char) * (i));
    d->string1[i] = '\0';
    str += i + 1;
    i = strlen(str);
    d->string2 = (char*)malloc(i * sizeof(char));
    strncpy(d->string2, str, (i - 1) * sizeof(char));
    d->string2[i - 1] = '\0';
}

inline static void parseNDCD(char *str, ReturnData d){
    long i = strchr(str, ' ') - str;
    d->string1 = (char*)malloc(sizeof(char) * (i + 1));
    strncpy(d->string1, str, sizeof(char) * (i));
    d->string1[i] = '\0';
    str += i + 1;
    i = strlen(str);
    d->string2 = (char*)malloc(i * sizeof(char));
    strncpy(d->string2, str, (i - 1) * sizeof(char));
    d->string2[i - 1] = '\0';
}

inline static void parseCD(char *str, ReturnData d){
    long i = strchr(str, ' ') - str;
    d->string1 = (char*)malloc(sizeof(char) * (i + 1));
    strncpy(d->string1, str, sizeof(char) * (i));
    d->string1[i] = '\0';
    str += i + 1;
    d->number = 0;
    i = strchr(str, ' ') - str;
    d->number = stoi(str, i);
    str += i + 1;
    i = strlen(str);
    d->string2 = (char*)malloc(i * sizeof(char));
    strncpy(d->string2, str, (i - 1) * sizeof(char));
    d->string2[i - 1] = '\0';
}

inline static void parsePD(char *str, ReturnData d){
    long i = strchr(str, ' ') - str;
    d->string1 = (char*)malloc(sizeof(char) * (i + 1));
    strncpy(d->string1, str, sizeof(char) * (i));
    d->string1[i] = '\0';
    str += i + 1;
    d->number = 0;
    i = strchr(str, '\n') - str;
    d->number = stoi(str, i);
}

inline static void parseDPD(char *str, ReturnData d){
    long i = strlen(str);
    d->string1 = (char*)malloc(sizeof(char) * i);
    strncpy(d->string1, str, (i - 1) * sizeof(char));
    d->string1[i - 1] = '\0';
}

ReturnData parse(char *str){
    ReturnData d = (ReturnData)malloc(sizeof(struct Data));
    if(strncmp(NDED, str, strlen(NDED) * sizeof(char)) == 0){
        d->id = 1;
        str += strlen(NDED) + 1;
        parseNDED(str, d);
        return d;
    }
    if(strncmp(NDCD, str, strlen(NDCD) * sizeof(char)) == 0){
        d->id = 2;
        str += strlen(NDCD) + 1;
        parseNDCD(str, d);
        return d;
    }
    if(strncmp(CD, str, strlen(CD) * sizeof(char)) == 0){
        d->id = 3;
        str += strlen(CD) + 1;
        parseCD(str, d);
        return d;
    }
    if(strncmp(PD, str, strlen(PD) * sizeof(char)) == 0){
        d->id = 4;
        str += strlen(PD) + 1;
        parsePD(str, d);
        return d;
    }
    if(strncmp(DPD, str, strlen(DPD) * sizeof(char)) == 0){
        d->id = 5;
        str += strlen(DPD) + 1;
        parseDPD(str, d);
        return d;
    }
    else{
        d->id = -1;
        return d;
    }
}