#pragma once

#include <glm/glm.hpp>

namespace Sonic {

    class Camera2D
    {
    public:
        Camera2D(float left, float right, float bottom, float top);

        const glm::mat4& GetView() const { return m_ViewMatrix; }
        const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }

        void SetPosition(float x, float y, float z);
        void SetPosition(const glm::vec3& position);
        void SetRotation(float rotation);
    private:
        void UpdateViewMatrix();
    private:
        glm::vec3 m_Position;
        float m_Rotation;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
    };

}
