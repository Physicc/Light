#ifndef __ENTRYPOINT_H__
#define __ENTRYPOINT_H__

#include "core/logging.hpp"

#include "core/assert.hpp"

extern Light::Application* Light::createApplication();

int main(int argc, char** argv)
{
	Light::Logger::init();

	auto app = Light::createApplication();

	app->run();

	delete app;
}

#endif // __ENTRYPOINT_H__

