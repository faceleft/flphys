#ifndef LIBFLPHYS_H
#define LIBFLPHYS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(PHYS_USE_FLOAT)
typedef float pflt_t;
#elif defined(PHYS_USE_LONG_DOUBLE)
typedef long double pflt_t;
#else
typedef double pflt_t;
#endif

extern const pflt_t PHYS_G;                  // 6.6743015151515151514e-11 m^3/(kg*s^2)
extern const pflt_t PHYS_PI;                 // 3.1415926535897932385
extern const pflt_t PHYS_AIR_DENSITY;        // 1.225 kg/m^3
extern const pflt_t PHYS_ACCEL_OF_FREE_FALL; // 9.80665 m/s^2
extern const pflt_t PHYS_BALL_DRAG_COEF;     // 0.47

typedef struct {
    pflt_t x; // m | m/s | N
    pflt_t y; // m | m/s | N
    pflt_t z; // m | m/s | N
} pvec_t;

typedef struct {
    pvec_t pos;    // m
    pvec_t mov;    // m/s
    pflt_t mass;   // kg
    pflt_t radius; // m
    pflt_t area;   // m^2
    pflt_t volume; // m^3
    pvec_t force;  // N
} pobj_t;

typedef struct {
    pflt_t density;          // ambient air density kg/m^3
    pvec_t accel_of_gravity; // constant acceleration acting on all objects m/s^2
    pvec_t wind;             // ambien wind m/s
    bool is_gravity;         // inter-object gravity flag
    pflt_t time;             // total simulation time
    pobj_t *objects;         // pointer to objects array
    size_t objects_num;      // number of objects in array
} phys_t;

// enumeration returned by phys_run() and pobj_run()
typedef enum {
    PHYS_OK = 0,        // success
    PHYS_ERR_NULL_PTR,  // pointer to objects is null when their number is greater than 0
    PHYS_ERR_ZERO_DIST, // 2 objects are at the same point, with gravity turned on
    PHYS_ERR_ZERO_MASS  // massless object
} pres_t;

// returns a new initialized vector using length, XOY angle and ZOY angle
pvec_t pvec_scs_create(pflt_t len, pflt_t xy_angle, pflt_t zy_angle);

// calculates the length of the vector
pflt_t pvec_len(pvec_t vector);

// calculates the XOY angle of the vector
pflt_t pvec_xy_angle(pvec_t vector);

// calculates the ZOY angle of the vector
pflt_t pvec_zy_angle(pvec_t vector);

// returns a new initialized vector using position, movement, mass and radius
pobj_t pobj_create(pvec_t pos, pvec_t mov, pflt_t mass, pflt_t radius);

// calculates the displacement of an object with obj->force
pres_t pobj_run(pobj_t *obj, pflt_t time);

// sets the radius and recalculates area and volume
void pobj_set_radius(pobj_t *obj, pflt_t radius);

// sets the area and recalculates radius and volume
void pobj_set_area(pobj_t *obj, pflt_t area);

// sets the volume and recalculates radius and area
void pobj_set_volume(pobj_t *obj, pflt_t volume);

// returns a new initialized scene
phys_t phys_create(
    pflt_t density,
    pvec_t accel_of_gravity,
    pvec_t wind,
    pobj_t objects[],
    size_t objects_num,
    bool is_gravity
);

// steps times calculates the strength for all objects, and calls pobj_run(phys->obj+i, step_time)
pres_t phys_run(phys_t *phys, pflt_t step_time, uint64_t steps);

#endif
