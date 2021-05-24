#include "core/application.hpp"
#include "core/layer.hpp"
#include "rendering/shader.hpp"
#include "rendering/buffer.hpp"
#include "rendering/vertexarray.hpp"
#include "rendering/rendercommand.hpp"
#include "rendering/renderer.hpp"
#include "rendering/camera.hpp"
#include "rendering/editorcamera.hpp"
#include "rendering/texture.hpp"
#include "rendering/framebuffer.hpp"
#include "core/input.hpp"
#include "input/keycodes.hpp"
#include "input/mousecodes.hpp"
#include "core/timestep.hpp"
#include "core/logging.hpp"

#include "glm/gtc/type_ptr.hpp"

//--------ENTRY POINT-----------------
//#include "core/entrypoint.hpp"
//------------------------------------