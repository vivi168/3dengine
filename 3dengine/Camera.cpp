#include "Camera.h"

const float YAW = -glm::half_pi<float>();
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = glm::quarter_pi<float>();

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = YAW;
    pitch = PITCH;
    speed = SPEED;

    update();
}

void Camera::update()
{
    glm::vec3 f;

    f.x = glm::cos(yaw) * glm::cos(pitch);
    f.y = glm::sin(pitch);
    f.z = glm::sin(yaw) * glm::cos(pitch);

    front = glm::normalize(f);

    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::look_at()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::process_keyboard(CameraDirection direction, float delta_time)
{
    float velocity = speed * delta_time / 1000.0f;

    switch (direction) {
    case CameraDirection::FORWARD:
        position += front * velocity;
        break;
    case CameraDirection::BACKWARD:
        position -= front * velocity;
        break;
    case CameraDirection::LEFT:
        position -= right * velocity;
        break;
    case CameraDirection::RIGHT:
        position += right * velocity;
        break;
    }
}

void Camera::process_mouse()
{

}