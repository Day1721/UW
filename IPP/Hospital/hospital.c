#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "structure.h"

extern int count;
const char* v = "-v";
int flag = 0;

static void putToErrorStream(){
    if(flag == 1){
        fprintf(stderr, "DESCRIPTIONS: %d\n", count);
    }
}

int main(int argc, char **argv){
    char str[100000];
    ReturnData d;
    int n;
    if(argc > 2 || (argc == 2 && strcmp(v, argv[1]) != 0)){
        printf("ERROR");
        return 1;
    }
    if(argc == 2 && strcmp(v, argv[1]) == 0){
        flag = 1;
    }
    while(fgets(str, 100000, stdin) != 0){
        d = parse(str);
        switch(d->id){
            case 1:
                addPatientDisease(d->string1, d->string2);
                printf("OK\n");
                free(d->string1);
                free(d->string2);
                break;
            case 2:
                n = copyDisease(d->string1, d->string2);
                if(n == 0){printf("IGNORED\n");}
                else{printf("OK\n");}
                free(d->string1);
                free(d->string2);
                break;
            case 3:
                n = changeDescription(d->string1, d->number, d->string2);
                if(n == 0){printf("IGNORED\n");}
                else{printf("OK\n");}
                free(d->string1);
                free(d->string2);
                break;
            case 4:
                n = printDescription(d->string1, d->number);
                if(n == 0){printf("IGNORED\n");}
                free(d->string1);
                break;
            case 5:
                n = deletePatientData(d->string1);
                if(n == 0){printf("IGNORED\n");}
                else{printf("OK\n");}
                free(d->string1);
                break;
            default:
                printf("IGNORED\n");
                break;
        }
        free(d);
        putToErrorStream();
    }
    return 0;
}
