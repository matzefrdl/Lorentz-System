#ifndef SHADER_H_
#define SHADER_H_
#include "glad/glad.h"
#include "mat4x4.h"

typedef GLuint shader;

shader createShader(const char * vShader, const char * fShader);
void loadUniformMat4x4(shader shader, const char * name, mat4x4 * mat);
void deleteShader(shader shader);

#endif