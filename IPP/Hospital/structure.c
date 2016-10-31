#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"

struct Dis;
typedef struct Dis* Disease;
struct DL;
typedef struct DL* DiseaseList;
struct PL;
typedef struct PL* PatientList;

struct Dis {
    char *desc;
    int counter;
    DiseaseList pntr;
};

struct DL {
    DiseaseList prev, next;
    Disease dis;
};

struct PL {
    char *name;
    PatientList next;
    DiseaseList dList;
};

int count = 0;
static DiseaseList allDiseases = 0;
static PatientList patients = 0;

//Help static functions
static PatientList findPatient(char *name){
    if(patients == 0){return 0;}
    PatientList patient = patients;
    while(strcmp(name, patient->name) != 0 && patient->next != patient){
        patient = patient->next;
    }
    if(patient->next == patient){
        if(strcmp(name, patient->name) != 0) return 0;
        return patient;
    }
    return patient;
}

static Disease constructDisease(char* d){
    Disease list = (Disease)malloc(sizeof(struct Dis));
    list->desc = (char*)malloc(sizeof(char) * (strlen(d) + 1));
    strcpy(list->desc, d);
    list->counter = 1;
    list->pntr = 0;
    return list;
}

static void deconstructDiseaseList(DiseaseList *list){
    DiseaseList li;
    while((*list)->next != (*list)){
        li = *list;
        (*list) = (*list)->next;
        li->prev->next = li->next;
        li->next->prev = li->prev;
        if(li->dis->counter > 1){
            li->dis->counter--;
        }
        else{
            DiseaseList root = li->dis->pntr;
            if(root == allDiseases){
                allDiseases = allDiseases->next;
            }
            root->prev->next = root->next;
            root->next->prev = root->prev;
            free(root);
            free(li->dis->desc);
            free(li->dis);
            count--;
        }
        free(li);
    }
    if((*list)->dis->counter == 1){
        DiseaseList root = (*list)->dis->pntr;
        root->prev->next = root->next;
        root->next->prev = root->prev;
        free(root);
        free((*list)->dis->desc);
        free((*list)->dis);
        count--;
    }
    else{
        (*list)->dis->counter--;
    }
    free(*list);
}

static void addDisease(DiseaseList *list, Disease d){
    DiseaseList dl = (DiseaseList)malloc(sizeof(struct DL));
    dl->dis = d;
    if(*list == 0){
        dl->next = dl->prev = dl;
        *list = dl;
    }
    else{
        dl->next = (*list);
        dl->prev = (*list)->prev;
        (*list)->prev->next = dl;
        (*list)->prev = dl;
    }
}

static PatientList constructPatient(char *n){
    PatientList patient = (PatientList)malloc(sizeof(struct PL));
    patient->name = (char*)malloc(sizeof(char) * (strlen(n) + 1));
    strcpy(patient->name, n);
    patient->dList = 0;
    return patient;
}

static void addPatient(PatientList *list, PatientList p){
    if(*list == 0){
        p->next = p;
        (*list) = p;
    }
    else{
        p->next = *list;
        (*list) = p;
    }
}

//Main functions
void addPatientDisease(char *p, char *d){
    Disease dis = constructDisease(d);
    count++;
    addDisease((&allDiseases), dis);
    (dis->pntr) = (allDiseases->prev);
    PatientList patient = findPatient(p);
    if(patient == 0){
        addPatient(&patients, constructPatient(p));
        addDisease(&(patients->dList), dis);
    }
    else{
        addDisease(&(patient->dList), dis);
    }
}

int copyDisease(char *to, char *from){
    PatientList pfrom = findPatient(from);
    PatientList pto = findPatient(to);
    if(pfrom == 0 || pfrom->dList == 0){
        return 0;
    }
    if(pto == 0){
        addPatient(&patients, constructPatient(to));
        pto = patients;
    }
    Disease dis = pfrom->dList->prev->dis;
    (dis->counter)++;
    addDisease(&(pto->dList), dis);
    return 1;
}

int changeDescription(char * p, int n, char * d){
    int i;
    PatientList patient = findPatient(p);
    if(patient == 0 || patient->dList == 0){
        return 0;
    }
    DiseaseList disList = patient->dList;
    if(n < 1){
        return 0;
    }
    for(i = 1; i < n; i++){
        if(disList->next == patient->dList){
            return 0;
        }
        disList = disList->next;
    }
    if(disList->dis->counter == 1){
        free(disList->dis->desc);
        disList->dis->desc = (char*)malloc(sizeof(char) * (strlen(d) + 1));
        strcpy(disList->dis->desc, d);
    }
    else{
        Disease dis = constructDisease(d);
        count++;
        addDisease(&allDiseases, dis);
        dis->pntr = allDiseases->prev;
        disList->dis->counter--;
        disList->dis = dis;
    }
    return 1;
}

int printDescription(char * p, int n){
    int i;
    PatientList patient = findPatient(p);
    if(patient == 0 || patient->dList == 0){
        return 0;
    }
    if(n == 1){
        printf("%s\n", patient->dList->dis->desc);
        return 1;
    }
    DiseaseList dis = patient->dList->next;
    if(dis == patient->dList){
        return 0;
    }
    for(i = 2; i < n; i++){
        if(dis == patient->dList){
            return 0;
        }
        dis = dis->next;
    }
    if(dis == patient->dList){
        return 0;
    }
    printf("%s\n", dis->dis->desc);
    return 1;
}

int deletePatientData(char *p){
    PatientList patient = findPatient(p);
    if(patient == 0){
        return 0;
    }
    if(patient->dList == 0){
        return 1;
    }
    deconstructDiseaseList(&(patient->dList));
    patient->dList = 0;
    if(count == 0){
        allDiseases = 0;
    }
    return 1;
}