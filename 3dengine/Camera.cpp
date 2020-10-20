#include "Camera.h"

const float YAW = -glm::half_pi<float>();
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.01f;
const float ZOOM = glm::quarter_pi<float>();

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 3.0f);
    world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = YAW;
    pitch = PITCH;
    speed = SPEED;
    sensitivity = SENSITIVITY;
    m_zoom = ZOOM;
    m_constrain_pitch = true;

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
    float velocity = speed * delta_time;

    switch (direction) {
    case CameraDirection::FORWARD:
        position += front * velocity;
        break;
    case CameraDirection::BACKWARD:
        position -= front * velocity;
        break;
    case CameraDirection::LEFTWARD:
        position -= right * velocity;
        break;
    case CameraDirection::RIGHTWARD:
        position += right * velocity;
        break;

    case CameraDirection::UP:
        pitch += sensitivity;
        break;
    case CameraDirection::DOWN:
        pitch -= sensitivity;
        break;
    case CameraDirection::LEFT:
        yaw -= sensitivity;
        break;
    case CameraDirection::RIGHT:
        yaw += sensitivity;
        break;
    }

    constrain_pitch();
    update();
}

void Camera::process_mouse(float x, float y)
{
    int invert_mouse = -1;

    yaw += x * sensitivity;
    pitch += (y * sensitivity) * invert_mouse;

    constrain_pitch();
    update();
}

void Camera::constrain_pitch()
{
    if (!m_constrain_pitch)
        return;

    constexpr float pitch_limit = glm::half_pi<float>() - 0.01f;
    if (pitch > pitch_limit)
        pitch = pitch_limit;
    if (pitch < -pitch_limit)
        pitch = -pitch_limit;
}

float Camera::zoom()
{
    return m_zoom;
}