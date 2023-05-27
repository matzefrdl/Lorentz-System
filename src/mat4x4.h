#ifndef MAT4X4_H_
#define MAT4x4_H_
#include <stdlib.h>
#include <math.h>

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

typedef struct mat4x4 {

    float data[16];

} mat4x4;

mat4x4 * createMat4x4();
vec3* make_vec3();

void cross(vec3 * a, vec3 * b, vec3 * dest);

void norm(vec3 * v);

void deleteVec3(vec3 * vec);
void deleteMat4x4(mat4x4 * matrix);

void identity(mat4x4 * matrix);
void perspective(mat4x4 * matrix, float fovy, float aspect, float zNear, float zFar);

void lookAt(mat4x4 * matrix, vec3 target, vec3 current, vec3 up);

#endif 