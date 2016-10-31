/** @file
 * Interface of list structure
 */

#ifndef MIDDLE_AGES_STRUCTURE_H
#define MIDDLE_AGES_STRUCTURE_H

/**
 * Type, that contains the tier information: Peasant, Knight or King.
 */
enum tier {
    peasant = 0,
    knight,
    king
};

/**
 * Pointer to list field, that represends a list of units.
 */
typedef struct list_def* list;

/**
 * Creates a new list with one field, initiated by params
 * @param[in] human One of three tiers in game: peasant, knight, king.
 * @param[in] color Number of player (0 or 1), whose unit is.
 * @param[in] x Column number of new unit.
 * @param[in] y Row number of new unit.
 * @return New unit.
 */
list new(enum tier human, int color, int x, int y);

int get_x(list l);

int get_y(list l);

/**
 * Getter to a tier of argument unit.
 * @param[in] unit Unit to access to tier info.
 * @return Tier of input
 */
enum tier get_tier(list unit);

/**
 * Getter to a color of argument unit.
 * @param[in] unit Unit to access to color info.
 * @return Color of input
 */
int get_color(list unit);

/**
 * Getter to a last turn unit do anything.
 * @param[in] unit Unit to access to last turn info.
 * @return Last turn.
 */
int get_last_turn(list unit);

/**
 * Setter to the last turn unit do anything.
 * @param[in] unit Unit to change field.
 * @param[in] num New value.
 */
void set_last_turn(list unit, int num);

/**
 * Setter to coordinates of argument unit.
 * @param[in] l Unit to change a coordinates.
 * @param[in] x New column number.
 * @param[in] y New row number.
 */
void set_coords(list l, int x, int y);

/**
 * Search a unit on a given map with given coordinates.
 * @param[in] l Map to seach on.
 * @param[in] x Column of a searched unit.
 * @param[in] y Row of a searched unit.
 * @return Searched unit or NULL, if searched unit not exist.
 */
list find(list l, int x, int y);

list* find_all(list l, int pred(list), int length);

int counter(list l, int pred(list));

int distance(list p1, list p2);

//void sort_list_array(list **l, int length, int compare(list, list));
/**
 * Concatenate one list to another.
 * @param[in] to Pointer to list, where you want concatenate to.
 * @param[in] what List, that you want concatenate to.
 */
void add(list *to, list what);

/**
 * Removing the field from the list and free's allocated memory
 * @param[in] l Pointer to list with field to remove.
 * @param[in] x Column number of removing field
 * @param[in] y Row number of removing field
 */
void delete(list *l, int x, int y);

/**
 * Romove all fields from given list and free's all allocated memory.
 * @param[in] l list to remove.
 */
void delete_all(list *l);
#endif //MIDDLE_AGES_STRUCTURE_H
