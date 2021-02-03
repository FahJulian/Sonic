#pragma once
#include "Sonic/Debug/Profiler/Profiler.h"
#include "Sonic/App.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/UI/Font/FontRenderer.h"
#include "Sonic/UI/UIRenderer.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Views.h"

using namespace Sonic;


Scene::Scene()
	: m_Camera(Camera2D(0, Window::getWidth(), 0, Window::getHeight())),
	m_UIHandler(this)
{
	AddListener(this, &Scene::OnRenderer2DComponentAdded);
	AddListener(this, &Scene::OnTransform2DComponentAdded);

	AddListener(this, &Scene::OnWindowResized);
}

Entity Scene::AddEntity()
{
	return Entity(this);
}

void Scene::RemoveEntity(EntityID entity)
{
	for (BaseBehaviourPool* pool : GenericContainer::GetAll<BaseBehaviourPool>(this))
		if (pool->HasEntity(entity))
			pool->RemoveEntity(entity);

	for (BaseComponentPool* pool : GenericContainer::GetAll<BaseComponentPool>(this))
		if (pool->HasEntity(entity))
			pool->RemoveEntity(entity);
}

Entity Scene::ToEntity(EntityID entity)
{
	return { this, entity };
}

void Scene::Init()
{
	Load();

	UpdatePools();

	OnInit();
}

void Scene::Update(float deltaTime)
{
	SONIC_PROFILE_FUNCTION("Scene::Update");

	OnUpdate(deltaTime);

	UpdateComponents(deltaTime);

	UpdateBehaviours(deltaTime);

	m_UIHandler.Update(deltaTime);

	UpdatePools();

	PollCollisionEvents();
}

void Scene::UpdatePools()
{
	SONIC_PROFILE_FUNCTION("Scene::UpdatePools");

	for (BaseBehaviourPool* pool : GenericContainer::GetAll<BaseBehaviourPool>(this))
		pool->UpdatePool();

	for (BaseComponentPool* pool : GenericContainer::GetAll<BaseComponentPool>(this))
		pool->OnUpdate();
}

void Scene::UpdateBehaviours(float deltaTime)
{
	SONIC_PROFILE_FUNCTION("Scene::UpdateBehaviours");

	for (BaseBehaviourPool* pool : GenericContainer::GetAll<BaseBehaviourPool>(this))
		pool->UpdateBehaviours(deltaTime);
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
