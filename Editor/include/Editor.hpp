#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "layers/editorlayer.hpp"

namespace Light{
	class Editor : public Application
	{
	public:
		Editor()
		{
			pushLayer(new EditorLayer());
		}
		~Editor() = default;
	};
}

#endif // __EDITOR_H__
