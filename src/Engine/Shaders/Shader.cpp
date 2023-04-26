//
// Created by shriller44 on 10/8/22.
//

#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath){

    auto [vertexCode, fragmentCode] = load(vertexPath, fragmentPath);

    if (vertexCode == std::nullopt){
        std::cerr << "failed to read vertex shader" << std::endl;
        success = -1;
    }

    if (vertexCode == std::nullopt){
        std::cerr << "failed to read fragment shader" << std::endl;
        success = -1;
    }

    if (success != -1) compile(vertexCode.value(), fragmentCode.value());
}

std::optional<std::string> Shader::readFile(ShaderType st, std::ifstream& fileStream){
    using charIter = std::istreambuf_iterator<char>;

    std::string output;

    if (fileStream){
        switch(st){
            case ShaderType::FRAGMENT:{ std::cout << "read fragment shader okay" << std::endl; break;}
            case ShaderType::VERTEX:{ std::cout << "read vertex shader okay" << std::endl; break;}
        }
        output = {charIter(fileStream), charIter()};
    }
    else {
        return std::nullopt;
    }
    return output;
}

auto Shader::load(std::string_view vertexPath, std::string_view fragPath) -> std::pair<std::optional<std::string> ,std::optional<std::string>>{

    std::ifstream vertexIn (static_cast<std::string>(vertexPath));
    std::ifstream fragIn (static_cast<std::string>(fragPath));

    auto vertexContents = readFile(ShaderType::VERTEX, vertexIn);
    auto fragContents = readFile(ShaderType::FRAGMENT, fragIn);

    return std::make_pair(vertexContents, fragContents);
}

void Shader::compile(std::string_view vertexSource, std::string_view fragSource){

    GLuint vertex{}, fragment{};

    auto compileShader = [](ShaderType st, std::string_view source,  GLuint& shader){
        shader = glCreateShader(ShaderType::VERTEX == st ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
        std::string source_ {source};
        const char* shaderCode = source_.c_str();
        glShaderSource(shader, 1, &shaderCode, nullptr);
        glCompileShader(shader);
        checkCompileErrors(shader, ShaderType::VERTEX == st ? compileStage::VERTEX : compileStage::FRAGMENT);
    };

    compileShader(ShaderType::VERTEX, vertexSource, vertex);
    compileShader(ShaderType::FRAGMENT, fragSource, fragment);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, compileStage::PROGRAM);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::checkCompileErrors(GLuint shader, compileStage stage)
{
    GLint success;
    GLchar infoLog[1024];

    switch (stage) {
        case compileStage::PROGRAM : {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << stage << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
            break;
        }
        default: {

            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << stage << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
            else {
                std::cout << "compiled "<< stage <<" shader okay" << std::endl;
            }
        }
    }
}

Shader::operator bool() const { return success == 0;}

Shader::operator GLuint() const {
    return ID;
}

void Shader::use() const {
    glUseProgram(ID);
}