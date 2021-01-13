#pragma once
#include "Sonic/main.h"
#include "Application.h"

Sonic::App* Sonic::createApplication()
{
	return new Application();
}

const std::string& Sonic::resourceDir()
{
	static const std::string RESOURCE_DIR = "C:\\dev\\Sonic\\Examples\\Snake\\res\\";
	return RESOURCE_DIR;
}
