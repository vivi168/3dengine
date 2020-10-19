#include <GL/gl3w.h>
#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "InputManager.h"
#include "Shader.h"
#include "Mesh.h"

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
    Mesh mesh;

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

        const Uint32 FPS = 144;
        const Uint32 ticks_per_frame = 1000 / FPS;
        Uint32 frame_start, frame_time;

        while (!quit) {
            tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsedTime = tp2 - tp1;
            tp1 = tp2;
            float fElapsedTime = elapsedTime.count() * 100;

            frame_start = SDL_GetTicks();

            poll_events();
            // Update here
            render();

            frame_time = SDL_GetTicks() - frame_start;
            if (ticks_per_frame > frame_time) {
                SDL_Delay(ticks_per_frame - frame_time);
            }
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 transform = glm::mat4(1.0f);
        // transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, -0.7f));
        transform = glm::rotate(transform, (float)SDL_GetTicks() /1000, glm::vec3(0.3f, 1.0f, 0.0f));
        transform = glm::scale(transform, glm::vec3(0.2f, 0.2f, 0.2f));

        GLuint transformLoc = glGetUniformLocation(shader.id(), "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        shader.use();

        mesh.draw(shader);

        SDL_GL_SwapWindow(window);
    }

    void gl_init()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        shader.load("shader.vert", "shader.frag");

        mesh.init("backpack.obj");
    }

    void gl_cleanup()
    {
        mesh.cleanup();
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
