#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

int mf2(int x, int y) {
    return y * 4 + x;
}

char * readFile(const char * path) {
    FILE * file = fopen(path, "r");

    int length = 0;

    while(1) {
        int c = fgetc(file);
        if(c == EOF) break;
        length++;
    }
    rewind(file);

    char * content = (char*) malloc(sizeof(char) * (length + 1));

    for(int i = 0; i < length; i++) {
        content[i] = fgetc(file);
    }

    content[length] = '\0';

    fclose(file);

    return content;
}

void printErrorFromCompiler(GLuint shader) {
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, 0, infoLog);
        printf(infoLog);
    }
}

shader createShader(const char * vShad, const char * fShad) {

    char * vC = readFile(vShad);
    char *  fC = readFile(fShad);

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, (char const * const *)&vC, 0);
    glCompileShader(vShader);
    printErrorFromCompiler(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1,(char const * const *) &fC, 0);
    glCompileShader(fShader);
    printErrorFromCompiler(fShader);

    shader shader = glCreateProgram();
    glAttachShader(shader, vShader);
    glAttachShader(shader, fShader);
    glLinkProgram(shader);

    char infoLog[512];
    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader, 512, 0, infoLog);
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    free(vC);
    free(fC);

    return shader;
}

void deleteShader(shader shader) {
    glDeleteProgram(shader);
}

void loadUniformMat4x4(shader shader, const char * name, mat4x4 * mat) {
    GLuint loc = glGetUniformLocation(shader, name);

    glUniformMatrix4fv(loc, 1, GL_FALSE, mat->data);
}