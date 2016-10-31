#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "parse.h"
#include "utils.h"


static regex_t init_regex;
static regex_t move_regex;
static regex_t produce_k_regex;
static regex_t produce_p_regex;
static regex_t end_turnn;

void build_parser(){
    regcomp(&init_regex,
            "^INIT [1-9][0-9]{0,9} [1-9][0-9]{0,9} [1,2] [1-9][0-9]{0,9} [1-9][0-9]{0,9} [1-9][0-9]{0,9} [1-9][0-9]{0,9}\n$",
            REG_EXTENDED);
    regcomp(&move_regex, "^MOVE [1-9][0-9]{0,9} [1-9][0-9]{0,9} [1-9][0-9]{0,9} [1-9][0-9]{0,9}\n$",
            REG_EXTENDED);
    regcomp(&produce_k_regex,
            "^PRODUCE_KNIGHT [1-9][0-9]{0,9} [1-9][0-9]{0,9} [1-9][0-9]{0,9} [1-9][0-9]{0,9}\n$",
            REG_EXTENDED);
    regcomp(&produce_p_regex,
            "^PRODUCE_PEASANT [1-9][0-9]{0,9} [1-9][0-9]{0,9} [1-9][0-9]{0,9} [1-9][0-9]{0,9}\n$",
            REG_EXTENDED);
    regcomp(&end_turnn, "^END_TURN\n$", REG_EXTENDED);
}

command* parse_command() {
    command *result = (command*)malloc(sizeof(struct def_command));
    char *str = NULL;
    char *temp;
    int bool;
    int i = 0;
    size_t sto = 100;
    getline(&str, &sto, stdin);


    bool = regexec(&init_regex, str, 0, 0, 0);
    if(!bool){
        strcpy(result->name, "INIT");
        str += 5;
        temp = strtok(str, " ");
        while(temp != 0){
            result->data[i] = atoi(temp);
            temp = strtok(0, " ");
            i++;
        }
        for(int i = 0; i < 6; i++){
            ssize_t pos = strcspn(str, "\0");
            str[pos] = ' ';
        }
        str -= 5;
        free(str);
        return result;
    }

    bool = regexec(&move_regex, str, 0, 0, 0);
    if(!bool){
        strcpy(result->name, "MOVE");
        str += 5;
        temp = strtok(str, " ");
        while(temp != 0){
            result->data[i] = atoi(temp);
            temp = strtok(0, " ");
            i++;
        }
        str -= 5;
        free(str);
        return result;
    }

    bool = regexec(&produce_k_regex, str, 0, 0, 0);
    if(!bool){
        strcpy(result->name, "PRODUCE_KNIGHT");
        str += 15;
        temp = strtok(str, " ");
        while(temp != 0){
            result->data[i] = atoi(temp);
            temp = strtok(0, " ");
            i++;
        }
        str -= 15;
        free(str);
        return result;
    }

    bool = regexec(&produce_p_regex, str, 0, 0, 0);
    if(!bool){
        strcpy(result->name, "PRODUCE_PEASANT");
        str += 16;
        temp = strtok(str, " ");
        while(temp != 0){
            result->data[i] = atoi(temp);
            temp = strtok(0, " ");
            i++;
        }
        str -= 16;
        free(str);
        return result;
    }\

    bool = regexec(&end_turnn, str, 0, 0, 0);
    if(!bool){
        strcpy(result->name, "END_TURN");
        free(str);
        return result;
    }
    free(str);
    free(result);
    return NULL;
}

void clean_parse(){
    regfree(&init_regex);
    regfree(&move_regex);
    regfree(&produce_k_regex);
    regfree(&produce_p_regex);
    regfree(&end_turnn);
}
