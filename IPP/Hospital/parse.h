#ifndef HOSPITAL_PARSE_H
#define HOSPITAL_PARSE_H

struct Data{
    int id, number;
    char* string1;
    char* string2;
};

typedef struct Data* ReturnData;

ReturnData parse(char*);

#endif //HOSPITAL_PARSE_H
