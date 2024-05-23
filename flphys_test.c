#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "flphys.h"

typedef enum { DONE, FAIL } ptest_res_t;

typedef ptest_res_t (*ptest_t)(void);

pflt_t dist(pvec_t target, pvec_t value) {
    return pvec_len((pvec_t){
        target.x - value.x,
        target.y - value.y,
        target.z - value.z,
    });
}

/*Блок с тестами*/
ptest_res_t test_obj_run() {
    pobj_t o = pobj_create((pvec_t){0}, (pvec_t){100, 100, 100}, 1, 0.1);
    o.force = (pvec_t){0, -10, 0};
    pobj_run(&o, 20);
    if (dist((pvec_t){2000, 0, 2000}, o.pos) < 1)
        return DONE;
    return FAIL;
}

ptest_res_t test_obj_run_phys() {
    pobj_t o = pobj_create((pvec_t){0}, (pvec_t){100, 100, 100}, 1, 0.1);
    phys_t a = phys_create(0, (pvec_t){0, -10, 0}, (pvec_t){0}, &o, 1, 0);
    phys_run(&a, 0.1, 200);
    if (dist((pvec_t){2000, 0, 2000}, o.pos) < 1)
        return DONE;
    return FAIL;
}

ptest_res_t test_100_obj_run_phys() {
    const int OBJ_NUM = 100;
    pobj_t o[OBJ_NUM];
    for (int i = 0; i < OBJ_NUM; i++) {
        o[i] = pobj_create(((pvec_t){0}), ((pvec_t){i * 100, i * 100, i * 100}), 1, 0.1);
    }
    phys_t a = phys_create(0, (pvec_t){0, -10, 0}, (pvec_t){0}, o, OBJ_NUM, false);
    phys_run(&a, 0.1, 100);
    for (int i = 0; i < OBJ_NUM; i++) {
        if (dist((pvec_t){1000.0L * i, -500.0L + 1000.0L * i, 1000.0L * i}, o[i].pos) > 1)
            return FAIL;
    }
    return DONE;
}

ptest_res_t test_air() {
    pobj_t o = pobj_create(((pvec_t){0}), ((pvec_t){100, 100, 100}), 1, 0.1);
    phys_t a = phys_create(1.225L, ((pvec_t){0, -10, 0}), ((pvec_t){0}), &o, 1, 0);
    phys_run(&a, 0.1, 200);
    if (dist((pvec_t){181.66, -402.62, 181.66}, o.pos) < 1)
        return DONE;
    return FAIL;
}

ptest_res_t test_err() {
    phys_t a = phys_create(0, (pvec_t){0}, (pvec_t){0}, NULL, 1, 0);

    if (phys_run(&a, 1, 1) != PHYS_ERR_NULL_PTR)
        return FAIL;

    a = phys_create(0, (pvec_t){0}, (pvec_t){0}, NULL, 0, 0);

    if (phys_run(&a, 1, 1) != PHYS_OK)
        return FAIL;

    pobj_t o = pobj_create((pvec_t){0}, (pvec_t){0}, 0, 0);
    a = phys_create(0, (pvec_t){0}, (pvec_t){0}, &o, 1, 0);
    if (phys_run(&a, 1, 1) != PHYS_ERR_ZERO_MASS)
        return FAIL;

    pobj_t os[2];
    os[0] = pobj_create((pvec_t){0}, (pvec_t){0}, 1, 0);
    os[1] = pobj_create((pvec_t){0}, (pvec_t){0}, 1, 0);
    a = phys_create(0, (pvec_t){0}, (pvec_t){0}, os, 2, 1);
    if (phys_run(&a, 1, 1) != PHYS_ERR_ZERO_DIST)
        return FAIL;

    o = pobj_create((pvec_t){0}, (pvec_t){10, 10, 10}, 1, 0.1);
    a = phys_create(1.225, (pvec_t){0, -9.8, 0}, (pvec_t){-1, -1, -1}, &o, 1, 0);
    if (phys_run(&a, 0.1, 10) != PHYS_OK)
        return FAIL;

    return DONE;
}

ptest_res_t test_gravity() {
    const pflt_t MASS = 1.0e24L;
    const pflt_t RAD = 1.0e1L;
    const uint64_t STEPS = 100000;

    pobj_t objs[3];
    pflt_t V = sqrt(PHYS_G * (MASS / RAD));
    pflt_t time = (2 * PHYS_PI * RAD) / V / 2;

    pvec_t pos1 = (pvec_t){0, RAD, 0};
    pvec_t pos2 = (pvec_t){0, -RAD, 0};

    objs[0] = pobj_create((pvec_t){0}, (pvec_t){0}, MASS, 0);
    objs[1] = pobj_create(pos1, (pvec_t){V, 0, 0}, 1, 0);
    objs[2] = pobj_create(pos2, (pvec_t){-V, 0, 0}, 1, 0);

    phys_t phys = phys_create(0, (pvec_t){0}, (pvec_t){0}, objs, 3, 1);

    for (uint64_t i = 0; i < STEPS; i++) {
        if (phys_run(&phys, time / STEPS, 1) != PHYS_OK)
            return FAIL;
    }

    if (dist((pvec_t){0}, objs[0].pos) > 1)
        return FAIL;
    if (dist(pos1, objs[2].pos) > 3)
        return FAIL;
    if (dist(pos2, objs[1].pos) > 3)
        return FAIL;

    return DONE;
}

ptest_t phys_test[] = {
    test_obj_run,
    test_obj_run_phys,
    test_100_obj_run_phys,
    test_air,
    test_err,
    test_gravity,
};

int main() {
    for (int i = 0; i < sizeof(phys_test) / sizeof(ptest_t); i++) {
        printf("Test %d ", i + 1);
        if (phys_test[i]() == DONE) {
            puts("Done");
        } else {
            puts("Failed");
        }
    }
}
