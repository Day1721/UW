#include <stdlib.h>
#include "structure.h"

struct list_def;
/**
 * Structure, that represents a field of a list.
 */
typedef struct list_def {
    list prev;       ///< Pointer to a previous field in a list (or NULL, if present field is a first).
    enum tier human; ///< Contains a tier information of a unit.
    int color;       ///< Contains a "color" of unit, ie. number of his player - 1.
    int x;           ///< Contains a number of column of unit.
    int y;           ///< Contains a number of row of unit.
    int n_of_turn;   ///< Contains a number of last turn unit do something.
    list next;       ///< Pointer to a next field in a list (or NULL, if present field is a last).
};
/*
static int get_hash_num(int x, int y){
    return 10 * (y - 1) + x - 1;
}*/

list new(enum tier human, int color, int x, int y){
    list l = (list)malloc(sizeof(struct list_def));
    l->prev = l->next = NULL;
    l->human = human;
    l->x = x;
    l->y = y;
    l->color = color;
    l->n_of_turn = 0;
    return l;
}


enum tier get_tier(list l){
    return l->human;
}

int get_x(list l){
    return l->x;
}

int get_y(list l){
    return l->y;
}

int get_color(list l){
    return l->color;
}

int get_last_turn(list l){
    return l->n_of_turn;
}

void set_last_turn(list l, int n){
    if(l != NULL && n >= 0)
        l->n_of_turn = n;
}

void set_coords(list l, int x, int y){
    if(l != NULL && x > 0 && y > 0){
        l->x = x;
        l->y = y;
    }
}

int counter(list l, int pred(list)){
    int res = 0;
    while(l != NULL){
        if(pred(l)){
            res++;
        }
        l = l->next;
    }
    return res;
}
/*
int fold(list l, int begin_var, int func(int var, list element)){
    while(l != NULL){
        begin_var = func(begin_var, l);
        l = l->next;
    }
    return begin_var;
}*/

int distance(list p1, list p2){
    int dx = p1->x - p2->x > 0 ? p1->x - p2->x : p2->x - p1->x;
    int dy = p1->y - p2->y > 0 ? p1->y - p2->y : p2->y - p1->y;
    return dx > dy ? dx : dy;
}

list* find_all(list l, int pred(list), int length){
    if(pred == NULL) return NULL;
    list* res = (list*)malloc(sizeof(list) * length);
    for(int i = 0; i < length; i++){
        res[i] = NULL;
    }
    int i = 0;
    while(l != NULL){
        if(pred(l)){
            res[i] = l;
            i++;
        }
        l = l->next;
    }
    return res;
}


/*
void sort_list_array(list **l, int length, int compare(list, list)){
    int i = 0;
    while(i < length - 1){

    }
}*/

/*list* find_all(list l, int pred(int, int), int length){
    list *res = (list*)malloc(sizeof(list) * 100);
    for(int i = 0; i < length; i++){
        res[i] = NULL;
    }
    while(l != NULL){
        if(pred(l->x, l->y)){
            res[get_hash_num(l->x, l->y)] = l;
        }
        l = l->next;
    }
    return res;
}*/

list find(list l, int x, int y){
    if(l == NULL){
        return NULL;
    }
    if(l->prev != NULL){
        return find(l->prev, x, y);
    }
    while(l != NULL){
        if(l->x == x && l->y == y){
            return l;
        }
        l = l->next;
    }
    return NULL;
}

void add(list* to, list what){
    if(*to == NULL){
        *to = what;
        return;
    }
    if(what->prev != NULL){
        add(to, what->prev);
    }
    while((*to)->next != NULL){
        *to = (*to)->next;
    }
    what->prev = (*to);
    (*to)->next = what;
    while((*to)->prev != NULL){
        *to = (*to)->prev;
    }
}

void delete(list *l, int x, int y){
    list li = (*l);
    while(li != NULL){
        if(li->x == x && li->y == y){
            if(li->prev == NULL){
                if(li->next == NULL){
                    free(*l);
                    (*l) = NULL;
                    break;
                }
                (*l) = (*l)->next;
                free((*l)->prev);
                (*l)->prev = NULL;
                break;
            }
            if(li->next == NULL){
                li = li->prev;
                free(li->next);
                li->next = NULL;
                break;
            }
            li = li->prev;
            li->next = li->next->next;
            free(li->next->prev);
            li->next->prev = li;
            break;
        }
        li = li->next;
    }
}

void delete_all(list *l){
    while((*l) != NULL){
        delete(l, (*l)->x, (*l)->y);
    }
}