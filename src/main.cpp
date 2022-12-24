/*
 * 2017 SDL2 with OpenGL Boilerplate Code
 * Use this as you wish. Licensed under CC0. No attribution required.
 */
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "data/Images/logo.h"
#include "ResourceManagers/Shader.h"
#include "Engine/Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <filesystem>
#include <fmt/format.h>
#include <algorithm>
#include <array>


Shader* customShader;

struct windowContext{
    SDL_Window    *m_window {};
    SDL_GLContext  m_context {};
    std::uint32_t nFrames {};
};

GLuint texture1;
GLuint VAO{}, VBO{}, colourBuffer{};
GLuint EBO{};

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int width, height, nrChannels;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices


std::array<GLfloat, 12> FRONT_QUAD{

      0.5f, 0.5f, -0.5f,  // top right
      0.5f, -0.5f, -0.5f,  // bottom right
      -0.5f, -0.5f, -0.5f,  // bottom left
     -0.5f, 0.5f, -0.5f   // top left
};

 std::array<GLfloat, 12> BACK_QUAD {
         0.5f, 0.5f, -1.5f,  // top right
         0.5f, -0.5f, -1.5f,  // bottom right
         -0.5f, -0.5f, -1.5f,  // bottom left
         -0.5f, 0.5f, -1.5f   // top left
};

std::array<GLfloat, 12> LEFT_QUAD {
        -0.5f, 0.5f, -1.5f,  // top right
        -0.5f, -0.5f, -1.5f,  // bottom right
        -0.5f, -0.5f, -0.5f,  // bottom left, dont change
        -0.5f, 0.5f, -0.5f   // top left, dont change
};


std::array<GLfloat, 12> RIGHT_QUAD {
        0.5f, 0.5f, -1.5f,  // top right
        0.5f, -0.5f, -1.5f,  // bottom right
        0.5f, -0.5f, -0.5f,  // bottom left, dont change
        0.5f, 0.5f, -0.5f   // top left, dont change
};

std::array<std::uint32_t, 24> indices {
    0,1,3,
    1,2,3,
    4,5,7,
    5,6,7,
    8,9,11,
    9,10,11,
    12,13,15,
    13,14,15
};

std::vector<std::array<GLfloat, 12>> cube {
    FRONT_QUAD,
    BACK_QUAD,
    LEFT_QUAD,
    RIGHT_QUAD,
};

// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
};

// world space positions of our cubes
glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
};


/*
 * Initialize Shaders
 */

/// @brief Does some shade ahh shit bro
int InitShaders()
{
    customShader = new Shader{"../data/Shaders/basic.vert", "../data/Shaders/basic.frag"};
    return 0;
}

enum class quadType{
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    BOTTOM,
    TOP
};

std::array<GLfloat, 6> getQuad(quadType const& qt,  std::array<GLfloat, 6>& baseQuad){
    switch(qt) {
        case quadType::BACK : {
            std::transform(std::begin(baseQuad), std::end(baseQuad), std::begin(baseQuad), [&](GLfloat vertex){
                return vertex - 0.5f;
            });
        }
        case quadType::LEFT: {
            std::transform(std::begin(baseQuad), std::end(baseQuad), std::begin(baseQuad), [&](GLfloat vertex){
                return vertex - 0.5f;
            });
            break;
        }
        case quadType::RIGHT:
            break;
        case quadType::BOTTOM:
            break;
        case quadType::TOP:
            break;
    }
}

/*
 * Initialize Geometry
 */
int InitGeometry()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    GLfloat cube_buffer_data[cube.size() * 12];

    int counter {0};
    for (const auto& quad: cube){
        for (size_t vertex {0} ; vertex < quad.size(); ++vertex){
            cube_buffer_data[vertex + (quad.size() * counter)] = quad.at(vertex);
        }
        ++counter;
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_buffer_data), cube_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,0, (void*)0);
    glEnableVertexAttribArray(1);

    return 0;
}

/*
 * Initialize textures
 */
int InitTextures()
{

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load("../data/Textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //customShader->use();
    //customShader->setInt("texture1",0);

    return 0;
}

/*
 * Free resources
 */
int Cleanup(windowContext* wc)
{
    return 0;
}

/*
 * Render a frame
 */
int Update(windowContext* wc)
{

    glEnable(GL_DEPTH_TEST);
    auto currentFrame = static_cast<float>(SDL_GetTicks());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    customShader->use();
    glBindVertexArray(VAO);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)(SCR_WIDTH) / (float)(SCR_HEIGHT), 0.1f, 100.0f);
    //glm::mat4 projection = glm::perspective(90.0f, (float)(SCR_WIDTH) / (float)(SCR_HEIGHT), 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = glm::mat4(1.0f);
    customShader->setMat4("projection", projection);
    customShader->setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 scale = glm::vec3(0.2, 0.2, 0.2);
    model = glm::translate(model, {0,0,0});
    model = glm::scale(model, scale);
    customShader->setMat4("model", model);

    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, nullptr);
    /*
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)(SCR_WIDTH) / (float)(SCR_HEIGHT), 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    customShader->setMat4("projection", projection);
    customShader->setMat4("view", view);

    glBindVertexArray(VAO);
    for (size_t i {0}; i < 10; ++i ){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * static_cast<float>(i);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        customShader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    */
    SDL_GL_SwapWindow(wc->m_window);
    return 0;
}

/*
 * Basic Initialization
 */
///@brief initialise from window context
///@param wc, window used in the SDL
int Initialize(windowContext* wc)
{
    // Initialize SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "test" << std::endl;
        fprintf(stderr, "Failed to initialize SDL video\n");
        return 1;
    }

    // Create main window
    wc->m_window = SDL_CreateWindow(
            "SDL App",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800, 600,
            SDL_WINDOW_OPENGL);
    if (wc->m_window == NULL) {
        fprintf(stderr, "Failed to create main window\n");
        SDL_Quit();
        return 1;
    }

    // Initialize rendering context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /* create opengl context in SDL */
    wc->m_context = SDL_GL_CreateContext(wc->m_window);
    if (wc->m_context == nullptr) {
        fprintf(stderr, "Failed to create GL context\n");
        SDL_DestroyWindow(wc->m_window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetSwapInterval(1); // Use VSYNC

    // Initialize GL Extension Wrangler (GLEW)
    GLenum err;
    glewExperimental = GL_TRUE; // Please expose OpenGL 3.x+ interfaces
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to init GLEW\n");
        SDL_GL_DeleteContext(wc->m_context);
        SDL_DestroyWindow(wc->m_window);
        SDL_Quit();
        return 1;
    }

    InitShaders();
    InitGeometry();
    InitTextures();
    return 0;
}

void processKeyboard(){
    auto state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_GetScancodeFromKey(SDLK_w)]){
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    else if (state[SDL_GetScancodeFromKey(SDLK_s)]){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    else if (state[SDL_GetScancodeFromKey(SDLK_a)]){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    else if (state[SDL_GetScancodeFromKey(SDLK_d)]){
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void Cum(float x_pos, float y_pos){

    std::cout << fmt::format("mouse x: {}, mouse y: {}\n", x_pos, y_pos);

    if (firstMouse)
    {
        lastX = x_pos;
        lastY = y_pos;
        firstMouse = false;
    }

    float x_offset = x_pos - lastX;
    float y_offset = lastY - y_pos; // reversed since y-coordinates go from bottom to top

    lastX = x_pos;
    lastY = y_pos;

    camera.ProcessMouseMovement(x_offset, y_offset);
}
/*
 * Program entry point
 */
int main(int argc, char *argv[])
{
    int should_run;

    printf("Initializing...\n");

    windowContext wc{};

    if (Initialize(&wc)) {
        return 1;
    }

    std::cout << "running" << std::endl;

    for (should_run = 1; should_run; ) {
        SDL_Event event;
        processKeyboard();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    should_run = 0;
                    SDL_Quit();
                    break;
                }
                case SDL_MOUSEMOTION: {
                    int x,y;
                    SDL_GetMouseState(&x,&y);
                    Cum(x,y);
                }
            }

        }

        Update(&wc);
    }

    printf("Exiting...\n");
    //Cleanup(&wc);
    return 0;
}