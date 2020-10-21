#pragma once

#include <SDL.h>
#include <map>

struct keyState {
    bool pressed, held, released;
};

class InputManager
{
public:
    static InputManager& instance()
    {
        static InputManager instance;

        return instance;
    }

    void update()
    {
        SDL_Event event;

        mouse_x_rel = 0;
        mouse_y_rel = 0;

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                new_keystate[event.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                new_keystate[event.key.keysym.sym] = false;
                break;
            case SDL_MOUSEMOTION:
                mouse_x_rel += event.motion.xrel;
                mouse_y_rel += event.motion.yrel;
                break;
            default:
                break;
            }
        }

        for (auto& [idx, k_state] : k_states) {
            k_state.pressed = false;
            k_state.released = false;

            if (new_keystate[idx] != old_keystate[idx]) {
                if (new_keystate[idx] == true) {
                    k_state.pressed = !k_state.held;
                    k_state.held = true;
                }
                else {
                    k_state.released = true;
                    k_state.held = false;
                }
            }

            old_keystate[idx] = new_keystate[idx];
        }
    }

    bool quit_requested()
    {
        return quit;
    }

    bool is_held(int k)
    {
        return k_states[k].held;
    }

    bool is_pressed(int k)
    {
        return k_states[k].pressed;
    }

    bool is_released(int k)
    {
        return k_states[k].released;
    }

    float mouse_x()
    {
        return (float)mouse_x_rel;
    }

    float mouse_y()
    {
        return (float)mouse_y_rel;
    }

private:
    InputManager() {}

    bool quit = false;
    int mouse_state = 0;
    int mouse_x_rel = 0, mouse_y_rel = 0;

    std::map<SDL_Keycode, bool> new_keystate;
    std::map<SDL_Keycode, bool> old_keystate;
    std::map<SDL_Keycode, keyState> k_states;
};