typedef uint32_t Entity;

struct Component1
{

}

struct Component2
{

}

class TestSystem
{
public:
    void init()
    {

    }

    void init(Entity e, const Component1& component)
    {

    }

    void init(Entity e, const Component2& component)
    {

    }

    void update(float deltaTime, Entity e, const Component1& component)
    {

    }

    void update(float deltaTime, Entity e, const Component1& component, const Component2& component2)
    {
        
    }
}

struct ComponentType
{
    uint16_t id;
    std::vector<uint16_t> required;
}

static uint16_t nextID;

template<typename C>
ComponentType getID()
{
    static uint16_t id = nextID++;
    return id;
}

// -> Scene class

template<typename Component, typename... Args>
void AddComponent(Enntity entity, Args&&... args)
{
    ComponentType type = getID<Component>();
}
