#pragma once
#include "Sonic/Util/Math/Vec3.h"
#include "Sonic/Util/Math/Mat4.h"

namespace Sonic {

    class Camera2D
    {
    public:
        Camera2D(float left, float right, float bottom, float top);

        const Mat4& GetView() const { return m_ViewMatrix; }
        const Mat4& GetProjection() const { return m_ProjectionMatrix; }

        void SetPosition(float x, float y, float z);
        void SetPosition(const Vec3& position);
        void SetRotation(float rotation);
    private:
        void UpdateViewMatrix();
    private:
        Vec3 m_Position;
        float m_Rotation;
        Mat4 m_ViewMatrix;
        Mat4 m_ProjectionMatrix;
    };

}
