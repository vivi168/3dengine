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
#include "Model.h"
#include "Camera.h"
#include "HeightMap.h"

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
    Shader shader, terrain_shader;
    std::vector<Model> models;
    Camera camera;
    InputManager input_manager = InputManager::instance();

    float delta_time;
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

        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    void mainloop()
    {
        auto tp1 = std::chrono::system_clock::now();
        auto tp2 = std::chrono::system_clock::now();
        delta_time = 0.0f;

        const Uint32 FPS = 144;
        const Uint32 ticks_per_frame = 1000 / FPS;
        Uint32 frame_start, frame_time;

        while (!quit) {
            tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsed_time = tp2 - tp1;
            tp1 = tp2;
            delta_time = elapsed_time.count();

            frame_start = SDL_GetTicks();

            poll_events();

            process_input();

            render();

            frame_time = SDL_GetTicks() - frame_start;
            if (ticks_per_frame > frame_time) {
                SDL_Delay(ticks_per_frame - frame_time);
            }
        }
    }

    void poll_events()
    {
        input_manager.update();
    }

    void process_input()
    {
        camera.process_mouse(input_manager.mouse_x(), input_manager.mouse_y());

        if (input_manager.quit_requested() || input_manager.is_pressed(SDLK_ESCAPE)) {
            quit = true;
        }

        if (input_manager.is_held(SDLK_w)) {
            camera.process_keyboard(CameraDirection::FORWARD, delta_time);
        }

        if (input_manager.is_held(SDLK_s)) {
            camera.process_keyboard(CameraDirection::BACKWARD, delta_time);
        }

        if (input_manager.is_held(SDLK_a)) {
            camera.process_keyboard(CameraDirection::LEFTWARD, delta_time);
        }

        if (input_manager.is_held(SDLK_d)) {
            camera.process_keyboard(CameraDirection::RIGHTWARD, delta_time);
        }

        if (input_manager.is_held(SDLK_UP)) {
            camera.process_keyboard(CameraDirection::UP, delta_time);
        }

        if (input_manager.is_held(SDLK_DOWN)) {
            camera.process_keyboard(CameraDirection::DOWN, delta_time);
        }

        if (input_manager.is_held(SDLK_LEFT)) {
            camera.process_keyboard(CameraDirection::LEFT, delta_time);
        }

        if (input_manager.is_held(SDLK_RIGHT)) {
            camera.process_keyboard(CameraDirection::RIGHT, delta_time);
        }
    }

    void render()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glm::mat4 view = camera.look_at();
        glm::mat4 projection = glm::perspective(camera.zoom(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 pv = projection * view;

        for (auto m : models)
            m.draw(pv);

        SDL_GL_SwapWindow(window);
    }

    void gl_init()
    {
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);

        shader.load("shader", "shaders/");
        terrain_shader.load("terrain", "shaders/");

        Mesh mesh("assets/inn/inn.obj", "assets/inn/");
        Mesh mesh2("assets/human/human.obj", "assets/human/");
        HeightMap map("assets/test.png");
        Mesh mesh3 = map.mesh();

        Model m1(mesh, &shader, { 32.0f, 64.0f, -100.0f });
        Model m2(mesh2, &shader);
        Model m3(mesh3, &terrain_shader);

        m2.translate({ 0.0f, 128.0f / 255.0f * 50.0f, 0.0f });

        models.push_back(m1);
        models.push_back(m2);
        models.push_back(m3);
    }

    void gl_cleanup()
    {
        for (auto m : models)
            m.cleanup();
        shader.unlink();
        terrain_shader.unlink();
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
