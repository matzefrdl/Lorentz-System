#include "lorentz.h"

lorentz * setupLorentz(float o, float b, float p) {
    lorentz * lor = (lorentz*) malloc(sizeof(lorentz));

    lor->o = o;
    lor->b = b;
    lor->p = p;
    lor->position.x = 1;
    lor->position.y = 1;
    lor->position.z = 1;

    return lor;
}

void deleteLorentz(lorentz * lorentz) {
    free(lorentz);
}

float dx(float o, float x, float y, float z) {
    return o * (y - x);
}

float dy(float p, float x, float y, float z) {
    return x * (p - z) - y;
}

float dz(float b, float x, float y, float z) {
    return x * y - b*z;
}

void calculateChange(lorentz * lorentz, vec3 * change) {
    float x = lorentz->position.x;
    float y = lorentz->position.y;
    float z = lorentz->position.z;
    float h = 0.005f;
    float h2 = h / 2.0f;

    float k1x = dx(lorentz->o, x, y, 0);
    float k2x = dx(lorentz->o, x + h2 * k1x, y, 0);
    float k3x = dx(lorentz->o, x + h2 * k2x, y, 0);
    float k4x = dx(lorentz->o, x + h * k3x, y, 0);
    float scX = (1.0f / 6.0f) * (k1x + 2.0f*k2x + 2.0f*k3x + k4x);
    
    float k1y = dy(lorentz->p, x, y, z);
    float k2y = dy(lorentz->p, x, y + h2 * k1y, z);
    float k3y = dy(lorentz->p, x, y + h2 * k2y, z);
    float k4y = dy(lorentz->p, x, y + h * k3y, z);
    float scY = (1.0f / 6.0f) * (k1y + 2.0f*k2y + 2.0f*k3y + k4y);

    float k1z = dz(lorentz->b, x, y, z);
    float k2z = dz(lorentz->b, x, y, z + h2 * k1z);
    float k3z = dz(lorentz->b, x, y, z + h2 * k2z);
    float k4z = dz(lorentz->b, x, y, z + h * k3z);
    float scZ = (1.0f / 6.0f) * (k1z + 2.0f*k2z + 2.0f*k3z + k4z);

    change->x = x + h * scX;
    change->y = y + h * scY;
    change->z = z + h * scZ;
}

lorentz_render_data * setupRenderData() {
    lorentz_render_data * data = (lorentz_render_data*) malloc(sizeof(lorentz_render_data));

    data->size = 2;
    data->pos = 1;
    data->vertices = (float*)malloc(sizeof(float) * 3 * data->size);
    data->colors = (float*)malloc(sizeof(float) * data->size);
    data->indices = (int*)malloc(sizeof(int) * (data->size-1) * 2);
    data->vertices[0] = 1;
    data->vertices[1] = 1;
    data->vertices[2] = 1;
    data->colors[0] = 0;

    return data;
}
void deleteRenderData(lorentz_render_data * data) {
    free(data->indices);
    free(data->vertices);
    free(data);
}
void addNewPoint(lorentz * lorentz, lorentz_render_data * data) {
    if(data->pos >= data->size) {
        data->size += 10000;
        
        data->vertices = realloc(data->vertices, sizeof(float) * 3 * data->size);
        data->colors = realloc(data->colors, sizeof(float) * data->size);
        data->indices =  realloc(data->indices, sizeof(int) * (data->size-1)*2);
    }
    vec3 change;

    calculateChange(lorentz, &change);

    data->vertices[data->pos * 3] = change.x;
    data->vertices[data->pos * 3 + 1] = change.y;
    data->vertices[data->pos * 3 + 2] = change.z;
    
    data->indices[(data->pos - 1) * 2] = data->pos - 1;
    data->indices[(data->pos - 1) * 2+1] = data->pos;

    float dX = dx(lorentz->o, change.x, change.y, change.z);
    float dY = dy(lorentz->p, change.x, change.y, change.z);
    float dZ = dz(lorentz->b, change.x, change.y, change.z);
    float l = sqrt(dX * dX + dY * dY + dZ * dZ);
    data->colors[data->pos] = l;

    data->pos++;
    lorentz->position.x = change.x;
    lorentz->position.y = change.y;
    lorentz->position.z = change.z;
}