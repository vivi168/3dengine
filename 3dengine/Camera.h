#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraDirection {
    FORWARD,
    BACKWARD,
    LEFTWARD,
    RIGHTWARD,

    UP,
    DOWN,
    LEFT,
    RIGHT,
};

class Camera
{
public:
    Camera();
    glm::mat4 look_at();
    void process_keyboard(CameraDirection, float);
    void process_mouse(float, float);
    float zoom();
private:
    glm::vec3 position;
    glm::vec3 front, up, right;
    glm::vec3 world_up;

    float yaw, pitch;
    float speed, sensitivity, m_zoom;
    bool m_constrain_pitch;

    void update();
    void constrain_pitch();
};