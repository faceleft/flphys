#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#include "flphys.h"

#define TEST_TIME_SEC 10
#define MAX_OBJS_NUM 1000

int main() {
    time_t start;
    pobj_t objs[MAX_OBJS_NUM];
    phys_t phys;
    uint64_t c;
    puts("Step: 1 ms");
    puts("simulation_time/real_time");

    objs[0] = pobj_create((pvec_t){1,1,1}, (pvec_t){-1,-1,-1}, 1, 0.1);
    phys = phys_create(PHYS_AIR_DENSITY, (pvec_t){0,-PHYS_ACCEL_OF_FREE_FALL,0}, (pvec_t){0}, objs, 1, 0);
    printf("1 obj: ");
    fflush(stdout);
    c = 0;
    start = time(NULL);
    while(difftime(time(NULL), start) < TEST_TIME_SEC) {
        phys_run(&phys, 0.001, 1000);
        c++;
    }
    printf("x%.2f\n", (float)c/TEST_TIME_SEC);
    fflush(stdout);



    #undef OBJS_NUM
    #define OBJS_NUM 10
    for(int i = 0; i<OBJS_NUM; i++) {
        objs[i] = pobj_create((pvec_t){i,i,i}, (pvec_t){-i,-i,-i}, 1, 0.1);
    }
    phys = phys_create(PHYS_AIR_DENSITY, (pvec_t){0,-PHYS_ACCEL_OF_FREE_FALL,0}, (pvec_t){0}, objs, OBJS_NUM, 0);
    printf("%d objs: ", OBJS_NUM);
    fflush(stdout);
    c = 0;
    start = time(NULL);
    while(difftime(time(NULL), start) < TEST_TIME_SEC) {
        phys_run(&phys, 0.001, 1000);
        c++;
    }
    printf("x%.2f\n", (float)c/TEST_TIME_SEC);
    fflush(stdout);



    #undef OBJS_NUM
    #define OBJS_NUM 100
    for(int i = 0; i<OBJS_NUM; i++) {
        objs[i] = pobj_create((pvec_t){i,i,i}, (pvec_t){-i,-i,-i}, 1, 0.1);
    }
    phys = phys_create(PHYS_AIR_DENSITY, (pvec_t){0,-PHYS_ACCEL_OF_FREE_FALL,0}, (pvec_t){0}, objs, OBJS_NUM, 0);
    printf("%d objs: ", OBJS_NUM);
    fflush(stdout);
    c = 0;
    start = time(NULL);
    while(difftime(time(NULL), start) < TEST_TIME_SEC) {
        phys_run(&phys, 0.001, 1000);
        c++;
    }
    printf("x%.2f\n", (float)c/TEST_TIME_SEC);
    fflush(stdout);



    #undef OBJS_NUM
    #define OBJS_NUM 1000
    for(int i = 0; i<OBJS_NUM; i++) {
        objs[i] = pobj_create((pvec_t){i,i,i}, (pvec_t){-i,-i,-i}, 1, 0.1);
    }
    phys = phys_create(PHYS_AIR_DENSITY, (pvec_t){0,-PHYS_ACCEL_OF_FREE_FALL,0}, (pvec_t){0}, objs, OBJS_NUM, 0);
    printf("%d objs: ", OBJS_NUM);
    fflush(stdout);
    c = 0;
    start = time(NULL);
    while(difftime(time(NULL), start) < TEST_TIME_SEC) {
        phys_run(&phys, 0.001, 1000);
        c++;
    }
    printf("x%.2f\n", (float)c/TEST_TIME_SEC);
    fflush(stdout);
    //pvector_create(i,i,i)
}   