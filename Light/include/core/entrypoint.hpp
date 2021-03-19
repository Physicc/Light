#pragma once


extern Light::Application* Light::createApplication();

int main(int argc, char** argv)
{
	auto app = Light::createApplication();

	app->run();

	delete app;
}