#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include "cmocka.h"

#include "structure.h"

extern list collection;

extern int middle_main();

static int is_printed_to_stderr = 0;
static int n_of_param = 0;
static char *tab_of_entries[10];

// MOCKERS

ssize_t mock_getline(char **lineptr, size_t *t, FILE *stream){
    assert_ptr_equal(stream, stdin);
    assert_int_equal(*t, 100);
    if(*lineptr == NULL) {
        *lineptr = (char*)malloc(sizeof(char) * 100);
        strcpy(*lineptr, tab_of_entries[n_of_param]);
    }
    n_of_param++;
    return 0;
}

int mock_fputs(const char *str, FILE *stream){
    assert_true(stream == stderr);
    assert_true(strcmp(str, "input error"));
    is_printed_to_stderr++;
    return 0;
}

void* mock_malloc(size_t size){
    assert_true(size > 0);
    return malloc(size);
}

void mock_free(void *ptr){
    assert_non_null(ptr);
    free(ptr);
}

int mock_regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags){
    assert_ptr_equal(pmatch, NULL);
    assert_int_equal(eflags, 0);
    assert_int_equal(nmatch, 0);
    char *str = (char*)malloc(sizeof(string));
    strcpy(str, string);
    char *temp = strtok(str, " ");
    assert_non_null(temp);
    int bool = 0;
    if(strcmp(temp, "INIT") == 0) bool = 1;
    if(strcmp(temp, "MOVE") == 0) bool = 2;
    if(strcmp(temp, "PRODUCE_KNIGHT") == 0) bool = 3;
    if(strcmp(temp, "PRODUCE_PEASANT") == 0) bool = 4;
    if(strcmp(temp, "END_TURN") == 0) bool = 5;
    assert_true(bool);
    int counter = 0;
    temp = strtok(NULL, " ");
    int check = 0;
    while(temp != NULL){
        assert_int_equal(check,0);
        counter++;
        long i = atol(temp);
        long max_int = 2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2l;
        assert_true(i > 0 && i < max_int);
        if(temp[strlen(temp) - 1] == '\n') check = 1;
        assert_true(strlen(temp) <= 10);
        assert_int_not_equal('-', temp[0]);
        temp = strtok(NULL, " ");
    }
    switch(bool){
        case 1:
            assert_int_equal(counter, 7);
            break;
        case 2:
        case 3:
        case 4:
            assert_int_equal(counter, 4);
            break;
        case 5:
            assert_int_equal(counter, 0);
            break;
        default:
            assert_true(0);
    }
    free(str);
    return regexec(preg, string, nmatch, pmatch, eflags);
}

// HELPER

static void do_test(){
    n_of_param = 0;
    is_printed_to_stderr = 0;
    int res = middle_main();
    assert_int_equal(res, 42);
    assert_int_equal(is_printed_to_stderr, 1);
}


//TESTS
//move

static void test_move1(void **state){
    tab_of_entries[0] = "INIT 10 10 2 1 1 10 1\n";
    tab_of_entries[1] = "MOVE 1 1 0 0\n";
    do_test();
}

static void test_move2(void **state){
    tab_of_entries[0] = "INIT 10 10 2 1 1 10 1\n";
    tab_of_entries[1] = "MOVE 1 1 5 5\n";
    do_test();
}

static void test_move3(void **state){
    tab_of_entries[0] = "INIT 10 10 2 1 1 10 1\n";
    tab_of_entries[1] = "MOVE 10 1 9 1\n";
    do_test();
}

static void test_move4(void **state){
    tab_of_entries[0] = "INIT 10 10 2 1 1 10 1\n";
    tab_of_entries[1] = "MOVE 1 1 2 2\n";
    tab_of_entries[2] = "MOVE 2 2 3 3\n";
    do_test();
}

static void test_move5(void **state){
    tab_of_entries[0] = "INIT 10 10 2 10 1 1 1\n";
    tab_of_entries[1] = "MOVE 10 1 11 2\n";
    do_test();
}

static void test_move6(void **state){
    tab_of_entries[0] = "INIT 10 10 2 10 1 1 1\n";
    tab_of_entries[1] = "MOVE 10 1 9 2";
    do_test();
}

//find
static void test_find1(void **state){
    list l = new(king, 1, 1, 1);
    assert_ptr_equal(l, find(l, 1, 1));
    delete_all(&l);
}

static void test_find2(void **state){
    assert_ptr_equal(NULL, find(NULL, 1, 1));
}

static void test_find3(void **state){
    list l = new(king, 1, 1, 1);
    assert_ptr_equal(NULL, find(l, 1, 2));
    delete_all(&l);
}

static void test_find4(void **state){
    list l = new(king, 1, 1, 1);
    list k = new(knight, 1, 1, 1);
    add(&l, k);
    assert_ptr_equal(l, find(l, 1, 1));
    delete(&l, 1, 1);
    assert_ptr_equal(k, find(l, 1, 1));
    delete_all(&l);
}

static void test_find5(void **state){
    list l = new(king, 1, 1, 1);
    assert_ptr_equal(NULL, find(l, 0, 0));
    delete_all(&l);
}

//MAIN

int main() {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_move1),
            cmocka_unit_test(test_move2),
            cmocka_unit_test(test_move3),
            cmocka_unit_test(test_move4),
            cmocka_unit_test(test_move5),
            cmocka_unit_test(test_move6),
            cmocka_unit_test(test_find1),
            cmocka_unit_test(test_find2),
            cmocka_unit_test(test_find3),
            cmocka_unit_test(test_find4),
            cmocka_unit_test(test_find5)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}