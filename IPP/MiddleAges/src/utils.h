#ifndef MIDDLE_AGES_UTILS_H
#define MIDDLE_AGES_UTILS_H

#ifdef UNIT_TESTING


#ifdef getline
#undef getline
#endif // getline
extern ssize_t mock_getline(char **lineptr, size_t *t, FILE *stream);
#define getline mock_getline

#ifdef malloc
#undef malloc
#endif // malloc
extern void* mock_malloc(size_t size);
#define malloc mock_malloc

#ifdef regexec
#undef regexec
extern int mock_regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags);
#define regexec mock_regexec
#endif // regexec

#ifdef fputs
#undef fputs
#endif // fputs
extern int mock_fputs(const char *str, FILE *stream);
#define fputs mock_fputs

#ifdef free
#undef free
#endif //free
extern void mock_free(void *ptr);
#define free mock_free

int middle_main();
#define main middle_main

#define static

#endif //UNIT_TESTING

#endif //MIDDLE_AGES_UTILS_H
