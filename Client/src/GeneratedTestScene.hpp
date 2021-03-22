#include "sonic/core/scene/Scene.h"

#include "SceneData.hpp"

namespace sonic::client
{
    class TestScene : public Scene
    {
    public:
        TestScene()
            : Scene("TestScene", &mData)
        {
        }

        void initComponentPools() override
        {
        }

        void initSystems() override
        {
        }

        void updateComponents(float deltaTime) override
        {
        }

        void updateSystems(float deltaTime) override
        {
        }

        SceneData mData;
    };

} // namespace sonic::client
