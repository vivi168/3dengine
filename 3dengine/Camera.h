#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

class Camera
{
private:
    glm::vec3 position;
    glm::vec3 front, up, right;
    glm::vec3 world_up;

    float yaw, pitch;
    float speed, sensitivity, zoom;

    void update();
public:
    Camera();
    glm::mat4 look_at();
    void process_keyboard(CameraDirection, float);
    void process_mouse();
};