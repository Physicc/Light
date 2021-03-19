#ifndef __APPLICATION_H__
#define __APPLICATION_H__

namespace Light
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	Application* createApplication();
}
#endif // __APPLICATION_H__