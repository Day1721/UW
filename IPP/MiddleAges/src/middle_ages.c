#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "engine.h"
#include "utils.h"

static const char* IE = "input error\n";

int main(){

    start_game();
    build_parser();

    command *new_command;
    int bool = 0;


    while (1) {
        new_command = parse_command();

        if(new_command == NULL){
            fputs(IE, stderr);
            end_game();
            clean_parse();
            return 42;
        }

        if(strcmp(new_command->name, "INIT") == 0){
            bool = init(new_command->data[0],
                        new_command->data[1],
                        new_command->data[2],
                        new_command->data[3],
                        new_command->data[4],
                        new_command->data[5],
                        new_command->data[6]);
        }

        if(strcmp(new_command->name, "MOVE") == 0){
            bool = move(new_command->data[0],
                        new_command->data[1],
                        new_command->data[2],
                        new_command->data[3]);
        }

        if(strcmp(new_command->name, "PRODUCE_KNIGHT") == 0){
            bool = produce_knight(new_command->data[0],
                                  new_command->data[1],
                                  new_command->data[2],
                                  new_command->data[3]);
        }

        if(strcmp(new_command->name, "PRODUCE_PEASANT") == 0){
            bool = produce_peasant(new_command->data[0],
                                   new_command->data[1],
                                   new_command->data[2],
                                   new_command->data[3]);
        }

        if(strcmp(new_command->name, "END_TURN") == 0){
            bool = end_turn();
            if(bool == 1){
                bool = 0;
                free(new_command);
                break;
            }
            bool = make_turn();
            if(bool != 42 && bool != 0){
                if(bool == 2) bool = 0;
                free(new_command);
                break;
            }
            //puts("END_TURN");
            //fflush(stdout);
            //bool = end_turn();
            //if(bool == 1){
                //bool = 0;
                //free(new_command);
                //break;
            //}
        }

        if(bool == 42){
            fputs(IE, stderr);
            free(new_command);
            end_game();
            clean_parse();
            return 42;
        }
        /*
        if(strcmp(new_command->name, "END_TURN") != 0){
            print_topleft();
            puts("");
        }
        */
        free(new_command);

        if(abs(bool) == 1){
            break;
        }
        if(bool == 2){
            bool = 0;
            break;
        }
    }

    end_game();
    clean_parse();

    switch(bool){
        case 0:
            return 1;
        case 1:
            return 0;
        case (-1):
            return 2;
        default:
            return 42;
    }
}
