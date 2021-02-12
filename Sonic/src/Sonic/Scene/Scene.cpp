#pragma once
#include "Sonic/Debug/Profiler/Profiler.h"
#include "Sonic/App.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Renderer/2D/Renderer2D.h"
#include "Sonic/Renderer/Font/FontRenderer.h"
#include "Sonic/Renderer/UI/UIRenderer.h"
#include "Sonic/Scene/Components/Scripting/Script.h"
#include "Components.h"
#include "Scene.h"

using namespace Sonic;


Entity Scene::AddEntity()
{
	return m_NextEntity++;
}

Entity Scene::AddEntity(EntityGroup group)
{
	Entity entity = m_NextEntity++;
	m_EntityGroups[group].push_back(entity);
	return entity;
}

EntityGroup Scene::AddEntityGroup()
{
	return m_NextEntity++;
}

void Scene::AddToGroup(EntityGroup group, Entity entity)
{
	m_EntityGroups[group].push_back(entity);
}

std::vector<Entity>* Scene::GetGroup(EntityGroup group)
{
	return &m_EntityGroups[group];
}

void Scene::DeactivateEntity(Entity entity)
{
	DispatchEvent(EntityDeactivatedEvent(entity));
	if (HasComponent<UIRendererComponent>(entity))
	{
		UIRenderer::markDirty();
		FontRenderer::markDirty();
	}

	m_Registry.DeactivateEntity(entity);
}

void Scene::DeactivateEntities(EntityGroup group)
{
	for (Entity entity : m_EntityGroups[group])
		DeactivateEntity(entity);
}

void Scene::ReactivateEntity(Entity entity)
{
	m_Registry.ReactivateEntity(entity);
	DispatchEvent(EntityReactivatedEvent(entity));

	if (HasComponent<UIRendererComponent>(entity))
	{
		UIRenderer::markDirty();
		FontRenderer::markDirty();
	}
}

void Scene::ReactivateEntities(EntityGroup group)
{
	for (Entity entity : m_EntityGroups[group])
		ReactivateEntity(entity);
}

void Scene::RemoveEntity(Entity entity)
{
	DispatchEvent(EntityRemovedEvent(entity));
	m_Registry.RemoveEntity(entity);

	if (HasComponent<UIRendererComponent>(entity))
	{
		UIRenderer::markDirty();
		FontRenderer::markDirty();
	}
}

void Scene::Load()
{
	OnLoad();
}

void Scene::Init()
{
	AddListener(this, &Scene::OnRenderer2DComponentAdded);
	AddListener(this, &Scene::OnTransform2DComponentAdded);

	AddListener(this, &Scene::OnWindowResized);

	OnInit();

	View<ScriptComponent>().ForEach([=](auto entity, auto* scriptComponent) 
	{
		scriptComponent->script->Init(this, entity);
	});
}

void Scene::Update(float deltaTime)
{
	SONIC_PROFILE_FUNCTION("Scene::Update");

	OnUpdate(deltaTime);

	UpdateComponents(deltaTime);

	m_UIHandler.Update(deltaTime);

	PollCollisionEvents();
}

void Scene::UpdateComponents(float deltaTime)
{
	SONIC_PROFILE_FUNCTION("Scene::UpdateComponents");

	ViewComponents<ScriptComponent>().ForEach([=](auto* scriptComponent) 
	{
		scriptComponent->script->OnUpdate(deltaTime);
	});
}

void Scene::RebufferRenderers()
{
	SONIC_PROFILE_FUNCTION("Scene::RebufferRenderers");

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

void Scene::Destroy()
{
	m_NextEntity = 1;
	m_EntityGroups.clear();
	m_UIHandler.Destroy();
	m_Registry.Destroy();
}
