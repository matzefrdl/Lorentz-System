#ifndef LOR_H_
#define LOR_H_

#include "shader.h"

#include <stdio.h>

typedef struct lorentz {

    float o;
    float b;
    float p;
    vec3 position;

} lorentz;

typedef struct lorentz_render_data {
    float * vertices;
    float * colors;
    int * indices;
    int size;
    int pos;
} lorentz_render_data;

lorentz_render_data * setupRenderData();
void deleteRenderData(lorentz_render_data * data);
void addNewPoint(lorentz * lorentz, lorentz_render_data * data);

lorentz * setupLorentz(float o, float b, float p);
void deleteLorentz(lorentz * lorentz);

void calculateChange(lorentz * lorentz, vec3 * change);

#endif