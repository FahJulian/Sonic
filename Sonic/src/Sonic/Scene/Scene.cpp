#pragma once
#include "Sonic/Debug/Profiler/Profiler.h"
#include "Sonic/App.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/UI/Font/FontRenderer.h"
#include "Sonic/UI/UIRenderer.h"
#include "Scene.h"

using namespace Sonic;


Scene::Scene()
	: m_Camera(Camera2D(0, Window::getWidth(), 0, Window::getHeight())), m_UIHandler(this)
{
	AddListener(this, &Scene::OnRenderer2DComponentAdded);
	AddListener(this, &Scene::OnTransform2DComponentAdded);

	AddListener(this, &Scene::OnWindowResized);
}

Entity Scene::AddEntity()
{
	static Entity nextEntity = 1;
	return nextEntity++;
}

void Scene::DeactivateEntity(Entity entity)
{
#ifdef SONIC_DEBUG
	if (!m_MainRegistry.HasEntity(entity))
	{
		SONIC_LOG_WARN("Cannot deactivate entity. Active registry does not contain the given entity.");
		return;
	}
#endif

	DispatchEvent(EntityDeactivatedEvent(entity));
	m_MainRegistry.TransferEntity(entity, &m_InactiveRegistry);

	if (HasComponent<UIRendererComponent>(entity))
		UIRenderer::markDirty();
}

void Scene::ReactivateEntity(Entity entity)
{
#ifdef SONIC_DEBUG
	if (!m_InactiveRegistry.HasEntity(entity))
	{
		SONIC_LOG_WARN("Cannot reactivate entity. Inactive registry does not contain the given entity.");
		return;
	}
#endif

	m_InactiveRegistry.TransferEntity(entity, &m_MainRegistry);
	DispatchEvent(EntityReactivatedEvent(entity));

	if (HasComponent<UIRendererComponent>(entity))
		UIRenderer::markDirty();
}

void Scene::RemoveEntity(Entity entity)
{
	DispatchEvent(EntityRemovedEvent(entity));
	m_MainRegistry.RemoveEntity(entity);
	m_InactiveRegistry.RemoveEntity(entity);

	if (HasComponent<UIRendererComponent>(entity))
		UIRenderer::markDirty();
}

void Scene::Init()
{
	Load();
	OnInit();
}

void Scene::Update(float deltaTime)
{
	SONIC_PROFILE_FUNCTION("Scene::Update");

	OnUpdate(deltaTime);

	UpdateComponents(deltaTime);

	UpdateBehaviours(deltaTime);

	m_UIHandler.Update(deltaTime);

	PollCollisionEvents();
}

void Scene::UpdateBehaviours(float deltaTime)
{
	SONIC_PROFILE_FUNCTION("Scene::UpdateBehaviours");

	//for (BaseBehaviourPool* pool : GenericContainer::GetAll<BaseBehaviourPool>(m_GenericsKey))
	//	pool->UpdateBehaviours(deltaTime);
}

void Scene::UpdateComponents(float deltaTime)
{
	SONIC_PROFILE_FUNCTION("Scene::UpdateComponents");

	for (auto [entity, cameraComponent, t] : Group<Camera2DComponent, Transform2DComponent>())
	{
		cameraComponent->camera.SetPosition(t->GetPosition());
		cameraComponent->camera.SetRotation(t->GetRotation());

		if (cameraComponent->isSceneCamera)
			m_Camera = cameraComponent->camera;
	}
}

void Scene::Rebuffer()
{
	SONIC_PROFILE_FUNCTION("Scene::Rebuffer");

	Renderer2D::rebuffer(this, &m_Camera);
	UIRenderer::rebuffer(this);
	FontRenderer::rebuffer(this);
}

void Scene::Render()
{
	SONIC_PROFILE_FUNCTION("Scene::Render");

	Renderer2D::render();
	UIRenderer::render();
	FontRenderer::render();
}

void Scene::OnWindowResized(const WindowResizedEvent& e)
{
	UIRenderer::setViewportSize(e.width, e.height);
	FontRenderer::setViewportSize(e.width, e.height);
}

void Scene::OnRenderer2DComponentAdded(const ComponentAddedEvent<Renderer2DComponent>& e)
{
	if (HasComponent<Transform2DComponent>(e.entity))
		GetComponent<Transform2DComponent>(e.entity)->rendererDirty = GetComponent<Renderer2DComponent>(e.entity)->dirty;
}

void Scene::OnTransform2DComponentAdded(const ComponentAddedEvent<Transform2DComponent>& e)
{
	if (HasComponent<Renderer2DComponent>(e.entity))
		GetComponent<Transform2DComponent>(e.entity)->rendererDirty = GetComponent<Renderer2DComponent>(e.entity)->dirty;
}
