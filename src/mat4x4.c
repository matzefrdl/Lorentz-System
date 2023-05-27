#include "mat4x4.h"

#include <stdio.h>
int mf(int x, int y) {
    return x * 4 + y;
}

mat4x4 * createMat4x4() {
    mat4x4 * mat = (mat4x4 *) malloc(sizeof(mat4x4));
    identity(mat);
    return mat;
}
void deleteMat4x4(mat4x4 * matrix) {
    free(matrix);
}

void identity(mat4x4 * matrix) {
    matrix->data[0] = 1;
    matrix->data[1] = 0;
    matrix->data[2] = 0;
    matrix->data[3] = 0;

    matrix->data[4] = 0;
    matrix->data[5] = 1;
    matrix->data[6] = 0;
    matrix->data[7] = 0;
    
    matrix->data[8] = 0;
    matrix->data[9] = 0;
    matrix->data[10] = 1;
    matrix->data[11] = 0;
    
    matrix->data[12] = 0;
    matrix->data[13] = 0;
    matrix->data[14] = 0;
    matrix->data[15] = 1;
}

void perspective(mat4x4 * matrix, float fovy, float aspect, float zNear, float zFar) {
    for(int i = 0; i < 16; i++) {
        matrix->data[i] = 0;
    }

    float h = tan(fovy * 0.5f);
    matrix->data[mf(0,0)] = 1.0f / (aspect * h);
    matrix->data[mf(1,1)] = 1.0f / h;

    matrix->data[mf(2,2)] = (zFar + zNear) / (zNear - zFar);
    matrix->data[mf(3, 2)] = (zFar + zFar) * zNear / (zNear - zFar);
    matrix->data[mf(2, 3)] = -1.0f;
}
void lookAt(mat4x4 * matrix, vec3 target, vec3 current, vec3 up) {
    vec3 Z;
    Z.x = current.x - target.x;
    Z.y = current.y - target.y;
    Z.z = current.z - target.z;
    norm(&Z);

    vec3 * X = make_vec3();
    cross(&up, &Z, X);
    norm(X);

    vec3 * Y = make_vec3();
    cross(&Z, X, Y);
    norm(Y);

    matrix->data[mf(0, 0)] = X->x;
    matrix->data[mf(1, 0)] = X->y;
    matrix->data[mf(2, 0)] = X->z;
    matrix->data[mf(3, 0)] = X->x * (-current.x) + X->y * (-current.y) + X->z * (-current.z);
    
    matrix->data[mf(0, 1)] = Y->x;
    matrix->data[mf(1, 1)] = Y->y;
    matrix->data[mf(2, 1)] = Y->z;
    matrix->data[mf(3, 1)] = Y->x * (-current.x)+ Y->y * (-current.y) + Y->z * (-current.z);
    
    matrix->data[mf(0, 2)] = Z.x;
    matrix->data[mf(1, 2)] = Z.y;
    matrix->data[mf(2, 2)] = Z.z;
    matrix->data[mf(3, 2)] = Z.x * (-current.x) + Z.y * (-current.y) + Z.z * (-current.z);
    
    matrix->data[mf(0, 3)] = 0;
    matrix->data[mf(1, 3)] = 0;
    matrix->data[mf(2, 3)] = 0;
    matrix->data[mf(3, 3)] = 1;

    deleteVec3(X);
    deleteVec3(Y);

}


void cross(vec3 * a, vec3 * b, vec3 * dest) {
    dest->x = a->y * b->z - a->z * b->y;
    dest->y = a->z * b->x - a->x * b->z;
    dest->z = a->x * b->y - a->y * b->x;
}


vec3* make_vec3() {
    vec3 * vec = (vec3*) malloc(sizeof(vec3));
    return vec;
}

void deleteVec3(vec3 * vec) {
    free(vec);
}

void norm(vec3 * Z) {
    float len = sqrt(Z->x * Z->x + Z->y * Z->y +Z->z * Z->z);
    Z->x /= len;
    Z->y /= len;
    Z->z /= len;
}