#include "Sonic/Util/Math/Math.h"
#include "Camera2D.h"
using namespace Sonic;


Camera2D::Camera2D(float left, float right, float bottom, float top)
    : m_ProjectionMatrix(Mat4::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f), m_Rotation(0), m_Position({ 0.0f, 0.0f, 0.0f })
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

void Camera2D::SetPosition(const Vec3& position)
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
    Mat4 transform = Mat4(1.0f).Translate(m_Position) * Mat4(1.0f).Rotate(Math::toRadians(m_Rotation), { 0.0f, 0.0f, 1.0f });
    m_ViewMatrix = transform.Inverse();
}
