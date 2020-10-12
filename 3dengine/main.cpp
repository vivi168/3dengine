#include <GL/gl3w.h>
#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "InputManager.h"
#include "Shader.h"

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

class Demo
{
public:
    void run()
    {
        init();
        create_window("3D Engine", WINDOW_WIDTH, WINDOW_HEIGHT);

        gl_init();
        mainloop();

        gl_cleanup();
        cleanup();
    }

private:
    SDL_Window* window;
    SDL_GLContext context;
    Shader shader;
    GLuint vertex_array_obj, vertex_buffer_obj, texture_id;

    bool quit = false;

    void init()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            std::cerr << "Unable to init SDL\n";
            exit(EXIT_FAILURE);
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    }

    void create_window(const char* title, const int width, const int height)
    {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (window == NULL) {
            std::cerr << "Error while creating SDL_Window\n";
            SDL_Quit();
            exit(EXIT_FAILURE);
        }

        context = SDL_GL_CreateContext(window);

        if (gl3wInit()) {
            std::cerr << "failed to init GL3W" << std::endl;
            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(EXIT_FAILURE);
        }

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    void mainloop()
    {
        auto tp1 = std::chrono::system_clock::now();
        auto tp2 = std::chrono::system_clock::now();

        while (!quit) {
            tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsedTime = tp2 - tp1;
            tp1 = tp2;
            float fElapsedTime = elapsedTime.count() * 100;

            poll_events();
            // Update here
            render();
        }
    }

    void poll_events()
    {
        InputManager::getInstance().update();

        if (InputManager::getInstance().quitRequested()) {
            quit = true;
        }
    }

    void render()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    void gl_init()
    {
        shader.load("shader.vert", "shader.frag");

        std::vector<GLfloat> vertices = {
            // positions         // colors          // texutre coordinates
            -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // left
             0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // right
             0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f, // top
        };

        // create one vertex array object
        glGenVertexArrays(1, &vertex_array_obj);
        glBindVertexArray(vertex_array_obj);

        // create one vertex buffer object
        glGenBuffers(1, &vertex_buffer_obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
        // creates and initializes a buffer object's data store
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

        // define an array of generic vertex attribute data
        // layout location = 0
        // 3 float components
        // 3 float between each elements
        // first element at index 0
        // positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // colors
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // texture coordinates
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        int width, height, channels, mode;
        unsigned char* img_data = stbi_load("texture.png", &width, &height, &channels, 0);
        mode = channels == 4 ? GL_RGBA : GL_RGB;

        if (img_data) {
            glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, img_data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "Error while loading image\n";
        }

        stbi_image_free(img_data);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        shader.use();
    }

    void gl_cleanup()
    {
        glDeleteVertexArrays(1, &vertex_array_obj);
        glDeleteBuffers(1, &vertex_buffer_obj);
        glDeleteTextures(1, &texture_id);
        shader.unlink();
    }

    void cleanup()
    {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);

        SDL_Quit();
    }
};

int main(int argc, char** argv)
{
    Demo app;

    app.run();

    return 0;
}