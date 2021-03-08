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


void Scene::Load()
{
	OnLoad();
}

void Scene::Init()
{
	ComponentRegistry::Init();

	EventDispatcher::addListener(this, &Scene::OnRenderer2DComponentAdded);
	EventDispatcher::addListener(this, &Scene::OnTransform2DComponentAdded);

	EventDispatcher::addListener(this, &Scene::OnScriptComponentAdded);
	EventDispatcher::addListener(this, &Scene::OnScriptComponentDeactivated);
	EventDispatcher::addListener(this, &Scene::OnScriptComponentReactivated);

	EventDispatcher::addListener(this, &Scene::OnEntityDeactivated);
	EventDispatcher::addListener(this, &Scene::OnEntityReactivated);

	EventDispatcher::addListener(this, &Scene::OnWindowResized);

	m_UIHandler.Init();

	View<ScriptComponent>().ForEach([=](auto entity, auto* scriptComponent) 
	{
		scriptComponent->script->Init(this, entity);
	});

	OnInit();
}

void Scene::Update(float deltaTime)
{
	SONIC_PROFILE_FUNCTION("Scene::Update");

	OnUpdate(deltaTime);

	UpdateComponents(deltaTime);

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
	SONIC_LOG_DEBUG("Resized: (", e.width, ", ", e.height, ")");
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

void Scene::OnScriptComponentAdded(const ComponentAddedEvent<ScriptComponent>& e)
{
	GetComponent<ScriptComponent>(e.entity)->script->Init(this, e.entity);	
}

void Scene::OnScriptComponentDeactivated(const ComponentDeactivatedEvent<ScriptComponent>& e)
{
	GetComponent<ScriptComponent>(e.entity)->script->OnDestroy();
}

void Scene::OnScriptComponentReactivated(const ComponentReactivatedEvent<ScriptComponent>& e)
{
	GetComponent<ScriptComponent>(e.entity)->script->OnInit();
}

void Scene::OnEntityReactivated(const EntityReactivatedEvent& e)
{
	if (HasComponent<ScriptComponent>(e.entity))
		GetComponent<ScriptComponent>(e.entity)->script->OnInit();
}

void Scene::OnEntityDeactivated(const EntityDeactivatedEvent& e)
{
	if (HasComponent<ScriptComponent>(e.entity))
		GetComponent<ScriptComponent>(e.entity)->script->OnDestroy();
}

void Scene::Destroy()
{
	ComponentRegistry::Destroy();
}
