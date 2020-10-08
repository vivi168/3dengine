#pragma once

#include <SDL.h>
#include <map>

struct keyState {
    bool pressed, held, released;
};

class InputManager
{
public:
    static InputManager& getInstance()
    {
        static InputManager instance;

        return instance;
    }

    void update()
    {
        SDL_Event event;

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                newKeyState[event.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                newKeyState[event.key.keysym.sym] = false;
                break;
            default:
                break;
            }
        }

        for (auto& [idx, k_state] : k_states) {
            k_state.pressed = false;
            k_state.released = false;

            if (newKeyState[idx] != oldKeyState[idx]) {
                if (newKeyState[idx] == true) {
                    k_state.pressed = !k_state.held;
                    k_state.held = true;
                }
                else {
                    k_state.released = true;
                    k_state.held = false;
                }
            }

            oldKeyState[idx] = newKeyState[idx];
        }
    }

    bool quitRequested()
    {
        return quit;
    }

    bool isHeld(int k)
    {
        return k_states[k].held;
    }

    bool isPressed(int k)
    {
        return k_states[k].pressed;
    }

    bool isReleased(int k)
    {
        return k_states[k].released;
    }

private:
    InputManager() {}

    bool quit = false;
    std::map<SDL_Keycode, bool> newKeyState;
    std::map<SDL_Keycode, bool> oldKeyState;
    std::map<SDL_Keycode, keyState> k_states;
};