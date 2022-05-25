#include "Editor.hpp"
#include "core/entrypoint.hpp"

namespace Light
{
	Application* createApplication()
	{
		return new Editor();
	}
}
