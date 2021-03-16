#include "Sonic/Util/StringUtils.h"
#include "Sonic/Scene/Components.h"
#include "ClientSerialization.h"
#include "EntityDeserialization.h"

using namespace Sonic;


template<typename T>
static inline T getOr(const JSON& json, const String& name, const T& alt)
{
	if (json.contains(name))
		return json[name].get<T>();

	return alt;
}

static UISize getUISize(const JSON& json)
{
	UISize s;

	String mode = getOr<String>(json, "mode", "Absolute");
	if (mode == "Absolute") s.mode = UISize::Mode::Absolute;
	else if (mode == "WindowCenter") s.mode = UISize::Mode::WindowCenter;
	else if (mode == "RelativeToEntity") s.mode = UISize::Mode::RelativeToEntity;
	else if (mode == "RelativeToWindow") s.mode = UISize::Mode::RelativeToWindow;

	s.relativeValue = getOr(json, "relativeValue", 0.0f);

	return s;
}

static Color getColor(const JSON& json)
{
	Color color;

	if (json.is_string())
	{
		String colorString = json.get<String>();

		if (colorString.size() == 10)
			color = std::stoul(colorString, nullptr, 16);
		else if (colorString.size() == 8)
			color = std::stoul(colorString, nullptr, 16) | 0x000000ff;
	}
	else if (json.is_array())
	{
		color.r = json[0].get<float>() / 255.0f;
		color.g = json[1].get<float>() / 255.0f;
		color.b = json[2].get<float>() / 255.0f;
		if (json.size() > 3) color.a = json[3].get<float>() / 255.0f;
	}
	else
	{
		if (json.contains("r")) color.r = json["r"].get<float>() / 255.0f;
		if (json.contains("g")) color.g = json["g"].get<float>() / 255.0f;
		if (json.contains("b")) color.b = json["b"].get<float>() / 255.0f;
		if (json.contains("a")) color.a = json["a"].get<float>() / 255.0f;
	}

	return color;
}

Sprite getSprite(const JSON& json)
{
	return Sprite();
}

static UIRendererProperties getProperties(const JSON& json)
{
	UIRendererProperties properties;

	if (json.contains("sprite")) properties.sprite = getSprite(json["sprite"]);
	if (json.contains("color")) properties.color = getColor(json["color"]);
	if (json.contains("borderColor")) properties.borderColor = getColor(json["borderColor"]);
	if (json.contains("borderWeight")) properties.borderWeight = json["borderWeight"].get<int>();
	if (json.contains("edgeRadius")) properties.edgeRadius = json["edgeRadius"].get<int>();

	return properties;
}

static void deserializeComponentFromJson(Scene* scene, const std::map<String, Entity> entities, Entity entity, 
	const String& componentName, const JSON& componentData)
{
	if (Util::beginsWith(componentName, "ScriptComponent"))
	{
		size_t first = componentName.find_first_of('<') + 1;
		String scriptClass = componentName.substr(first, componentName.find_last_of('>') - first);

		std::unordered_map<String, String> scriptFields;
		for (auto& [fieldName, fieldValue] : componentData.items())
			scriptFields.emplace(fieldName, fieldValue);

		Optional<Script*> script = Serialization::createClientScript(scriptClass, scriptFields);

		if (script.HasValue())
			scene->AddComponent<ScriptComponent>(entity, script.GetValue());
		else
			SONIC_LOG_WARN("Could not deserialize ScriptComponent: Script class ", scriptClass, " not found");
	}
	else if (componentName == "UIComponent")
	{
		UIComponent component;

		if (componentData.contains("parent"))
			component.parent = componentData["parent"].get<Entity>();

		if (componentData.contains("xPattern"))
			component.xPattern = getUISize(componentData["xPattern"]);
		else if (componentData.contains("x"))
			component.x = componentData["y"].get<float>();
		if (componentData.contains("yPattern"))
			component.yPattern = getUISize(componentData["yPattern"]);
		else if (componentData.contains("y"))
			component.y = componentData["y"].get<float>();
		if (componentData.contains("widthPattern"))
			component.widthPattern = getUISize(componentData["widthPattern"]);
		else if (componentData.contains("width"))
			component.width = componentData["width"].get<float>();
		if (componentData.contains("heightPattern"))
			component.heightPattern = getUISize(componentData["heightPattern"]);
		else if (componentData.contains("height"))
			component.height = componentData["height"].get<float>();

		component.zIndex = getOr(componentData, "zIndex", 0.0f);

		scene->AddComponent<UIComponent>(entity, component);
	}
	else if (componentName == "UIRendererComponent")
	{
		UIRendererProperties properties;

		if (componentData.contains("properties"))
			properties = getProperties(componentData["properties"]);

		scene->AddComponent<UIRendererComponent>(entity, properties);
	}
}

static void deserializeComponentFromFile(Scene* scene, const std::map<String, Entity> entities, Entity entity, 
	const String& componentName, const String& filePath)
{
	InputFileStream file = InputFileStream(filePath);

	JSON json;

	try 
	{
		file >> json;
	}
	catch (nlohmann::detail::parse_error)
	{
		SONIC_LOG_WARN("Could not deserialize Component from file ", filePath, ": Could not extract JSON from file");
		return;
	}

	deserializeComponentFromJson(scene, entities, entity, componentName, filePath);
}

static Entity deserializeEntityFromJSON(Scene* scene, const std::map<String, Entity> entities, const JSON& entityData)
{
	Entity entity = scene->AddEntity();

	for (auto& [componentName, componentData] : entityData.items())
	{
		if (componentData.is_string())
			deserializeComponentFromFile(scene, entities, entity, componentName, componentData.get<String>());
		else if (componentData.is_object())
			deserializeComponentFromJson(scene, entities, entity, componentName, componentData);
	}

	return entity;
}

static Optional<Entity> deserializeEntityFromFile(Scene* scene, const std::map<String, Entity> entities, const String& filePath)
{
	InputFileStream file = InputFileStream(filePath);

	JSON json;

	try
	{
		file >> json;
	}
	catch (nlohmann::detail::parse_error)
	{
		SONIC_LOG_WARN("Could not deserialize Entity from file ", filePath, ": Could not extract JSON from file");
		return { };
	}

	return deserializeEntityFromJSON(scene, entities, json);
}

void Serialization::deserializeEntities(Scene* scene, const String& relativeFilePath, std::map<String, Entity>& entities, const JSON& data)
{
	for (auto& [entityID, entityData] : data["entities"].items())
	{
		if (entityData.is_string())
		{
			Optional<Entity> entity = deserializeEntityFromFile(scene, entities,
				resourceDir() + relativeFilePath + entityData.get<String>());

			if (entity.HasValue())
				entities.emplace(entityID, entity.GetValue());
		}
		else if (entityData.is_object())
		{
			entities.emplace(entityID, deserializeEntityFromJSON(scene, entities, entityData));
		}
	}
}
