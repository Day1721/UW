 /** @file
    Interface of parser.

 */

#ifndef PARSE_H
#define PARSE_H
/**
 * Stucture, that contains parsed data
 */
typedef struct def_command {
    char name[16]; ///< Contains a name of function as C string (char*).
    int data[7];   ///< Contains a params of function as int[].
} command;

void build_parser();

/** Reads a command.
  Returns command with data points using 'command' structure.
  */
command* parse_command();

/**
 * Free's allocated at beginning memory, need an the end of program.
 */
void clean_parse();

#endif /* PARSE_H */
