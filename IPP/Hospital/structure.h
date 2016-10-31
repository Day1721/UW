#ifndef HOSPITAL_STRUCTURE_H
#define HOSPITAL_STRUCTURE_H

void addPatientDisease(char *p, char *d);

int copyDisease(char *from, char *to);

int changeDescription(char *p, int n, char *d);

int printDescription(char *p, int n);

int deletePatientData(char *name);

#endif //HOSPITAL_STRUCTURE_H
