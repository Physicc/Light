#ifndef __ENTRYPOINT_H__
#define __ENTRYPOINT_H__

extern Light::Application* Light::createApplication();

int main(int argc, char** argv)
{
	auto app = Light::createApplication();

	app->run();

	delete app;
}

#endif // __ENTRYPOINT_H__