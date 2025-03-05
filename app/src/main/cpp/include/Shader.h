#ifndef SHADERTOYANDROID_SHADER_H
#define SHADERTOYANDROID_SHADER_H

#include <GLES3/gl3.h>
#include <string>

class Shader {
public:
    virtual void execute() = 0;
};

class GLSLShader : public Shader {
public:
    virtual void execute() override {}

    void activate() const;

    void deactivate() const;

    template<typename Type>
    void setUniform(const std::string &key, Type &value);

    template<typename Type>
    void setUniform(const std::string &key, Type &x, Type &y);

    template<typename Type>
    void setUniform(const std::string &key, Type &x, Type &y, Type &z);

    template<typename Type>
    void setUniform(const std::string &key, Type &x, Type &y, Type &z, Type &w);
private:
    GLuint loadShader(GLenum shaderType, const std::string &shaderSource);
    GLuint loadShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

    GLuint mProgram;
    std::string mShaderSource;
};

class SKSLShader : public Shader {
public:
    virtual void execute() override {}
};

#endif //SHADERTOYANDROID_SHADER_H
