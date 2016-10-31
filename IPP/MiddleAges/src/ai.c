#include <stdlib.h>
#include <stdio.h>
#include "ai.h"
#include "engine.h"

#ifdef abs
#undef abs
#endif //abs

extern int player_number;
extern int n_of_turn;
extern int dim;

extern list collection;

static list fst_peasant = NULL;
static list snd_peasant = NULL;

static int is_my_knight(list l){
    return player_number == get_color(l) && knight == get_tier(l);
}

static int is_enemy_unit(list l){
    return player_number != get_color(l);
}

static list get_nearest_enemy(list *enemies, int length, list knight){
    list result = enemies[0];
    for(int i = 1; i < length; i++){
        result = distance(knight, result) > distance(knight, enemies[i]) ? enemies[i] : result;
    }
    return result;
}

static int get_nearest_enemy_num(list *enemies, int length, list knight) {
    list result = enemies[0];
    int res = 0;
    for (int i = 1; i < length; i++) {
        res = distance(knight, result) > distance(knight, enemies[i]) ? i : res;
        result = distance(knight, result) > distance(knight, enemies[i]) ? enemies[i] : result;
    }
    return res;
}

static int is_my_peasant(list l){
    return player_number == get_color(l) && peasant == get_tier(l);
}
/*
static int sgn(int x){
    return x == 0 ? 0 : x > 0 ? 1 : -1;
}

static int distance_by_points(int x1, int y1, int x2, int y2){
    return abs(x1 - x2) > abs(y1 - y2) ? abs(x1 - x2) : abs(y1 - y2);
}
*/
static int city_distance(int x1, int y1, int x2, int y2){
    return abs(x1 - x2) + abs(y1 - y2);
}

static int go_to(list who, list where){
    int who_x = get_x(who);
    int who_y = get_y(who);
    int where_x = get_x(where);
    int where_y = get_y(where);
    int dir_x = who_x;
    int dir_y = who_y;
    //int dist = distance_by_points(who_x, who_y, where_x, where_y);
    int cdist = city_distance(who_x, who_y, where_x, where_y);
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            //int temp_dist = distance_by_points(who_x + i, who_y + j, where_x, where_y);
            int temp_cdist = city_distance(who_x + i, who_y + j, where_x, where_y);
            if(temp_cdist <= cdist && (i * i + j * j) != 0 && who_x + i > 0 && who_y + j > 0 &&
                    who_x + i <= dim & who_y + j <= dim /*&& temp_dist <= dist*/){
                list wall = find(collection, who_x + i, who_y + j);
                if(wall == NULL || get_color(wall) != player_number){
                    dir_x = who_x + i;
                    dir_y = who_y + j;
                    //dist = temp_dist;
                    cdist = temp_cdist;
                }
            }
        }
    }
    if(dir_x != who_x || dir_y != who_y){
        printf("MOVE %i %i %i %i\n", who_x, who_y, dir_x, dir_y);
        int res = move(who_x, who_y, dir_x, dir_y);
        return res == -1 ? 2 : where_x == dir_x && where_y == dir_y;
    }
    return 0;
}
/*
typedef int (*intfunc)(int);

#define lambda(return_type, function_body) \
  ({ \
    return_type __fn__ function_body \
    __fn__; \
  })

static intfunc compose(int f(int), int g(int)){
  return lambda(int, (int x) {return f(g(x));});
} */

static int produce(list p, list k, enum tier human){
    if(p == NULL) return 0;
    int p_x = get_x(p);
    int p_y = get_y(p);
    int k_x = get_x(k);
    int k_y = get_y(k);
    int dir_x = p_x;
    int dir_y = p_y;
    int cdist = city_distance(p_x, p_y, k_x, k_y);   //Tak, w tym miejscu robię to samo, co w go_to, ale wyciąganie tego do osobnej funkcji wymaga
    for(int i = -1; i < 2; i++){                     //tworzenia struktury do zwracania dwóch zmiennych, co IMO nie jest zbyt dobre, więc zostawiłem
        for(int j = -1; j < 2; j++){                 //jak jest.
            int temp_cdist = city_distance(p_x + i, p_y + j, k_x, k_y);
            if(temp_cdist <= cdist && (i * i + j * j) != 0 && p_x + i > 0 && p_y + j > 0
                    && p_x + i <= dim && p_y + j <= dim){
                list wall = find(collection, p_x + i, p_y + j);
                if(wall == NULL || get_color(wall) != player_number){
                    dir_x = p_x + i;
                    dir_y = p_y + j;
                    cdist = temp_cdist;
                }
            }
        }
    }
    if(dir_x != p_x || dir_y != p_y){
        if(human == peasant){
            printf("PRODUCE_PEASANT %i %i %i %i\n", p_x, p_y, dir_x, dir_y);
            return produce_peasant(p_x, p_y, dir_x, dir_y);
        }
        printf("PRODUCE_KNIGHT %i %i %i %i\n", p_x, p_y, dir_x, dir_y);
        return produce_knight(p_x, p_y, dir_x, dir_y);
    }
    return 0;
}

int make_turn_ai(){
    int bool;
    int enemies_amount = counter(collection, is_enemy_unit);
    list* enemies = find_all(collection, is_enemy_unit, enemies_amount);
    if(fst_peasant == NULL){
        int peasants_amount = counter(collection, is_my_peasant);
        list* peasants = find_all(collection, is_my_peasant, peasants_amount);
        fst_peasant = peasants[0];
        free(peasants);
    }
    if(snd_peasant == NULL){
        if(n_of_turn >= get_last_turn(fst_peasant) + 3){
            produce(fst_peasant, enemies[0], peasant);
            list* peasants = find_all(collection, is_my_peasant, 2);
            snd_peasant = peasants[1];
            free(peasants);
        }
    }
    else{
        if(n_of_turn >= get_last_turn(snd_peasant) + 3){
            produce(snd_peasant, enemies[0], knight);
        }
    }
    if(n_of_turn >= get_last_turn(fst_peasant) + 3){
        produce(fst_peasant, enemies[0], knight);
    }
    /*
    else{
        int x = get_x(snd_peasant);
        int y = get_y(snd_peasant);
        if(n_of_turn >= get_last_turn(snd_peasant) + 3) {
            list enemy = enemies[0];
            list wall = find(collection, x + sgn(get_x(enemy) - x), y + sgn(get_y(enemy) - y));
            if (wall == NULL) {
                produce_knight(x, y, x + sgn(get_x(enemy) - x), y + sgn(get_y(enemy) - y));
                printf("PRODUCE_KNIGHT %i %i %i %i\n", x, y, x + sgn(get_x(enemy) - x), y + sgn(get_y(enemy) - y));
                //fflush(stdout);
            }
            else if(get_tier(wall) != knight){
                wall = find(collection, x + 1, y + sgn(get_y(enemy) - y));
                if(wall == NULL){
                    produce_knight(x, y, x + 1, y + sgn(get_y(enemy) - y));
                    printf("PRODUCE_KNIGHT %i %i %i %i\n", x, y, x + 1, y + sgn(get_y(enemy) - y));
                }
            }
        }
    }
    if(n_of_turn >= get_last_turn(fst_peasant) + 3){
        int x = get_x(fst_peasant);
        int y = get_y(fst_peasant);
        list enemy = enemies[0];
        list wall = find(collection, x + sgn(get_x(enemy) - x), y + sgn(get_y(enemy) - y));
        if(wall == NULL){
            produce_knight(x, y, x + sgn(get_x(enemy) - x), y + sgn(get_y(enemy) - y));
            printf("PRODUCE_KNIGHT %i %i %i %i\n", x, y, x + sgn(get_x(enemy) - x), y + sgn(get_y(enemy) - y));
            //fflush(stdout);
        }
        else if(get_tier(wall) != knight){
            wall = find(collection, x + 1, y + sgn(get_y(enemy) - y));
            if(wall == NULL){
                produce_knight(x, y, x + 1, y + sgn(get_y(enemy) - y));
                printf("PRODUCE_KNIGHT %i %i %i %i\n", x, y, x + 1, y + sgn(get_y(enemy) - y));
            }
        }
    }*/
    int knights_amount = counter(collection, is_my_knight);
    list* knights = find_all(collection, is_my_knight, knights_amount);
    for(int i = 0; i < knights_amount; i++){
        list enemy = get_nearest_enemy(enemies, enemies_amount, knights[i]);
        int num = get_nearest_enemy_num(enemies, enemies_amount, knights[i]);
        bool = go_to(knights[i], enemy);
        if(bool == 1){
            enemies[num] = enemies[enemies_amount - 1];
            enemies[enemies_amount - 1] = 0;
            enemies_amount--;
        }
        if(bool == 2){
            fflush(stdout);
            free(knights);
            free(enemies);
            return 1;
        }
    }
    free(knights);
    free(enemies);
    puts("END_TURN");
    end_turn();
    fflush(stdout);
    return 0;
}