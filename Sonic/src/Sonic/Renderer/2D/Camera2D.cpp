#include <glm/gtc/matrix_transform.hpp>
#include "Camera2D.h"

using namespace Sonic;


Camera2D::Camera2D(float left, float right, float bottom, float top)
    : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f), m_Rotation(0), m_Position({ 0.0f, 0.0f, 0.0f })
{
    UpdateViewMatrix();
}

void Camera2D::SetPosition(float x, float y, float z)
{
    m_Position.x = x;
    m_Position.y = y;
    m_Position.z = z;

    UpdateViewMatrix();
}

void Camera2D::SetPosition(const glm::vec3& position)
{
    m_Position.x = position.x;
    m_Position.y = position.y;
    m_Position.z = position.z;

    UpdateViewMatrix();
}

void Camera2D::SetRotation(float rotation)
{
    m_Rotation = rotation;

    UpdateViewMatrix();
}

void Camera2D::UpdateViewMatrix()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), { 0.0f, 0.0f, 1.0f });

    m_ViewMatrix = glm::inverse(transform);
}
