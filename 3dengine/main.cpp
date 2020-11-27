#include <SDL.h>

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "InputManager.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "HeightMap.h"
#include "Water.h"
#include "Scene.h"
#include "Renderer.h"

class Demo
{
public:
    void run()
    {
        gl_init();
        mainloop();
        gl_cleanup();
    }

private:
    Scene scene;
    Camera camera;
    Renderer renderer;
    InputManager input_manager = InputManager::instance();

    float delta_time;
    bool quit = false;

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

            // printf("%f %f %f\n", camera.position.x, camera.position.y, camera.position.z);

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
        renderer.render(scene, camera);
    }

    void gl_init()
    {
        renderer.init();

        Mesh mesh("assets/inn/inn.obj", "assets/inn/");
        Mesh mesh2("assets/human/human.obj", "assets/human/");
        HeightMap map("assets/map.png");
        Mesh mesh3 = map.mesh();

        Mesh tree("assets/trees/elwynntreecanopy01.obj", "assets/trees/");

        Mesh water_pond = water_quad(256, 256);

        Model m1(mesh, Material::BASIC, { 50.0f, 0.5f, 220.0f });
        Model m2(mesh2, Material::BASIC);
        Model m3(mesh3, Material::TERRAIN);
        Model m4(water_pond, Material::WATER);
        Model tree_m(tree, Material::BASIC, { 145.0f, 10.0f, 128.0f });

        m2.translate({ 10.0f, 0.0f, 10.0f });

        scene.add_model(m1);
        scene.add_model(m2);
        scene.add_model(m3);
        scene.add_model(tree_m);
        scene.add_model(m4);
    }

    void gl_cleanup()
    {
        renderer.cleanup();
    }
};

int main(int argc, char** argv)
{
    Demo app;

    app.run();

    return 0;
}
