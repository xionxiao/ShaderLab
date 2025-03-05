#include <array>

#include <GLES3/gl3.h>
#include "include/Log.h"
#include "include/Shader.h"

void GLSLShader::activate() const {
    glUseProgram(mProgram);
}

void GLSLShader::deactivate() const {
    glUseProgram(0);
}

GLuint GLSLShader::loadShader(GLenum shaderType, const std::string &shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        auto *shaderRawString = (GLchar *) shaderSource.c_str();
        auto shaderLength = (GLint)(shaderSource.length());
        glShaderSource(shader, 1, &shaderRawString, &shaderLength);
        glCompileShader(shader);
    }
    return shader;
}

GLuint GLSLShader::loadShader(const std::string &vertexShaderSource,
                            const std::string &fragmentShaderSource) {
    Shader *shader = nullptr;
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (!fragmentShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
    }
}

template<typename Type>
void GLSLShader::setUniform(const std::string &key, Type &value) {
    static_assert(true, "unsupported type");
}

template<typename Type>
void GLSLShader::setUniform(const std::string &key, Type &x, Type &y) {
    static_assert(true, "unsupported type");
}

template<typename Type>
void GLSLShader::setUniform(const std::string &key, Type &x, Type &y, Type &z) {
    static_assert(true, "unsupported type");
}

template<typename Type>
void GLSLShader::setUniform(const std::string &key, Type &x, Type &y, Type &z, Type &w) {
    static_assert(true, "unsupported type");
}

#define UNIFORM1X(Type, glFunc) \
template<> void GLSLShader::setUniform<Type>( \
        const std::string &key,  \
        Type &value) { \
    GLint location = glGetUniformLocation(mProgram, key.c_str()); \
    glFunc(location, value); \
}

UNIFORM1X(GLfloat, glUniform1f)
UNIFORM1X(GLint, glUniform1i)
UNIFORM1X(GLuint, glUniform1ui)

#define UNIFORM1XV(Type, glFunc) \
template<> void GLSLShader::setUniform<std::vector<Type>>( \
        const std::string &key,  \
        std::vector<Type> &value) { \
    GLint location = glGetUniformLocation(mProgram, key.c_str()); \
    glFunc(location, (GLint)value.size(), value.data()); \
}

UNIFORM1XV(GLfloat, glUniform1fv)
UNIFORM1XV(GLint, glUniform1iv)
UNIFORM1XV(GLuint, glUniform1uiv)

#define UNIFORM2X(Type, glFunc) \
template<> void GLSLShader::setUniform<Type>( \
        const std::string &key,  \
        Type &x, \
        Type &y ) { \
    GLint location = glGetUniformLocation(mProgram, key.c_str()); \
    glFunc(location, x, y); \
}

UNIFORM2X(GLfloat, glUniform2f)
UNIFORM2X(GLint, glUniform2i)
UNIFORM2X(GLuint, glUniform2ui)
