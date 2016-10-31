#include <stdio.h>
#include <stdlib.h>

#include "structure.h"
#include "engine.h"
#include "ai.h"

list collection;
int dim;
int n_of_turn;
static int turns;
static int color_of_turn;
int player_number;
/*
static int print_x_tab[100];
static int print_y_tab[100];
*/
static int max(int a, int b){
    return (a > b ? a : b);
}

static int dis(int x1, int y1, int x2, int y2){
    return max(abs(x2 - x1), abs(y2 - y1));
}
/*
static int get_hash_num(int x, int y){
    return 10 * (y - 1) + x - 1;
}
*/
static int fight(list who, list to){
    if(get_tier(who) == get_tier(to)){
        return 0;
    }
    if(get_tier(who) == knight){
        return 1;
    }
    if(get_tier(to) == knight){
        return -1;
    }
    if(get_tier(who) == king){
        return 1;
    }
    return -1;
}

void start_game() {
    collection = NULL;
    color_of_turn = 1;
    dim = 0;
    player_number = 0;
//    for(int i = 0; i < 100; i++){
//        print_x_tab[i] = (i % 10) + 1;
//        print_y_tab[i] = (i / 10) + 1;
//    }
}

void end_game() {
    delete_all(&collection);
}
/*
static int predicate(int x, int y){
    return x <= 10 && y <= 10;
}

char finder(list *lt, int x, int y){
    if(lt[get_hash_num(x, y)] != NULL){
        list l = lt[get_hash_num(x, y)];
        switch(get_tier(l)){
            case peasant:
                if(get_color(l) == 0)return 'C';
                return 'c';
            case knight:
                if(get_color(l) == 0)return 'R';
                return 'r';
            case king:
                if(get_color(l) == 0)return 'K';
                return 'k';
        }
    }
    return '.';
}

void print_topleft() {
    int n = (dim > 10 ? 10 : dim);
    list *l_tab = find_all(collection, predicate, n * n);
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            char c = finder(l_tab, j, i);
            printf("%c", c);
            fflush(stdout);
        }
        puts("");
    }
    free(l_tab);
}
*/
int init(int n, int k, int p, int x1, int y1, int x2, int y2) {
    if(n <= 0 || k <= 0 || x1 > n - 3 || x2 > n - 3 || y1 > n || y2 > n || player_number != 0 || dis(x1, y1, x2, y2) < 8){
        return 42;
    }
    player_number = p;
    dim = n;
    turns = k;
    n_of_turn = 1;
    add(&collection, new(king, 1, x1, y1));
    add(&collection, new(peasant, 1, x1 + 1, y1));
    add(&collection, new(knight, 1, x1 + 2, y1));
    add(&collection, new(knight, 1, x1 + 3, y1));
    add(&collection, new(king, 2, x2, y2));
    add(&collection, new(peasant, 2, x2 + 1, y2));
    add(&collection, new(knight, 2, x2 + 2, y2));
    add(&collection, new(knight, 2, x2 + 3, y2));
    if(p == 1){
        make_turn();
    }
    return 0;
}

int move(int x1, int y1, int x2, int y2) {

    list l1 = find(collection, x1, y1);
    if(player_number == 0 || l1 == 0 || color_of_turn != get_color(l1) || get_last_turn(l1) == n_of_turn ||
            x1 - x2 > 1 || x1 - x2 < -1 || y1 - y2 > 1 || y1 - y2 < -1 || x2 > dim || y2 > dim){
        return 42;
    }
    list l2 = find(collection, x2, y2);
    if(l2 == 0) {
        set_coords(l1, x2, y2);
        set_last_turn(l1,n_of_turn);
        return 0;
    }
    if(get_color(l2) == get_color(l1)){
        return 42;
    }
    int i = fight(l1, l2);
    int j = 0;
    if(get_tier(l1) == king && i == -1){
        j = 1;
    }
    if(get_tier(l2) == king && i == 1){
        j = -1;
    }
    if(get_tier(l1) == king && get_tier(l2) == king){
        j = 2;
    }
    switch(i){
        case 1:
            delete(&collection, x2, y2);
            set_coords(l1, x2, y2);
            break;
        case 0:
            delete(&collection, x1, y1);
            delete(&collection, x2, y2);
            l1 = NULL;
            break;
        case -1:
            delete(&collection, x1, y1);
            l1 = NULL;
            break;
        default:
            break;
    }
    if(l1 != NULL){
        set_last_turn(l1,n_of_turn);
    }
    return j;
}

static int produce(int x1, int y1, int x2, int y2, enum tier t) {
    list l1 = find(collection, x1, y1);
    if(player_number == 0 || l1 == 0 || color_of_turn != get_color(l1) || get_last_turn(l1) > n_of_turn - 3 ||
       get_tier(l1) != peasant || x1 - x2 > 1 || x1 - x2 < -1 || y1 - y2 > 1 || y1 - y2 < -1 || x2 > dim || y2 > dim){
        return 42;
    }
    list l2 = find(collection, x2, y2);
    if(l2 == NULL){
        list unit = new(t, color_of_turn, x2, y2);
        add(&collection, unit);
        set_last_turn(unit, n_of_turn - 1);
        set_last_turn(l1, n_of_turn);
        return 0;
    }
    return 42;
}

int produce_knight(int x1, int y1, int x2, int y2) {
    return produce(x1, y1, x2, y2, knight);
}

int produce_peasant(int x1, int y1, int x2, int y2) {
    return produce(x1, y1, x2, y2, peasant);
}

int end_turn() {
    if(color_of_turn == 1){
        color_of_turn = 2;
        return 0;
    }
    if(n_of_turn < turns){
        color_of_turn = 1;
        n_of_turn++;
        return 0;
    }
    return 1;
}
/*
static int knight_find(int player){

}
*/
int make_turn(){
    return make_turn_ai();
}
