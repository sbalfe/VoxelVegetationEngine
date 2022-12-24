//
// Created by shriller44 on 10/8/22.
//

#ifndef CUBES_SHADER_H
#define CUBES_SHADER_H

#include <utility>
#include <fstream>
#include <string.h>
#include <iostream>
#include <optional>
#include <exception>
#include <GL/glew.h>
#include <glm/glm.hpp>

template<typename T> void func(T arg){}

namespace test {
}

class Shader {
public:

    enum class ShaderType{
        VERTEX,
        FRAGMENT
    };

    enum class compileStage {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

    friend auto& operator<<(std::ostream&os, compileStage& cs){

        switch (cs){
            case compileStage::PROGRAM: {
                os << "PROGRAM";
                break;
            }
            case compileStage::VERTEX:
                os << "VERTEX";
                break;
            case compileStage::FRAGMENT:
                os << "FRAGMENT";
                break;
        }
        return os;
    }

    ~Shader() = default;

    Shader(const char* vertexPath, const char* fragmentPath);

    explicit operator bool() const;

    /*
    template<typename T, typename... params> void setUniform(std::string_view name, params&&... args) {

        if constexpr (same<T,float>::value){
            glUniform1f(glGetUniformLocation(ID, std::string{name}.c_str()), std::forward<T>(args)...);
        }
        else if (same<T,bool>::value){

        }

        if constexpr (argCount == 1){
            if constexpr (std::is_same_v<T, float>){
                glUniform1f(glGetUniformLocation(ID, std::string{name}.c_str()), std::forward<T>(args)...);
            }
            else {
                glUniform1i(glGetUniformLocation(ID, std::string{name}.c_str()), std::forward<T>(args)...);
            }
        }
        else if (argCount == 2) {
            if constexpr (std::is_same_v<T, glm::vec2>) {
                glUniform2fv(glGetUniformLocation(ID, std::string{name}.c_str()), std::forward<T>(args)...);
            } else {
                glUniform2f(glGetUniformLocation(ID, std::string{name}.c_str()), std::forward<T>(args)...);
            }
        }
        else if (argCount == 3){
            if constexpr (std::is_same_v<T, glm::vec3>){
                glUniform3v(glGetUniformLocation(ID, std::string{name}.c_str()), std::forward<T>(args)...);
            } else {
                glUniform3f(glGetUniformLocation(ID, std::string{name}.c_str()), std::forward<T>(args)...);
            }
        }

    }
    */

    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void use() const;

    explicit operator GLuint() const;

private:

    static std::optional<std::string> readFile(ShaderType st, std::ifstream& fileStream);

    static auto load(std::string_view vertexPath, std::string_view fragPath) -> std::pair<std::optional<std::string> ,std::optional<std::string>>;

    void compile(std::string_view vertexSource, std::string_view fragSource);

    static void checkCompileErrors(GLuint shader, compileStage stage);
private:
    int success {};
    GLuint ID{};

};


#endif //CUBES_SHADER_H
