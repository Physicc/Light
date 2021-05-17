#ifndef __BASE_H__
#define __BASE_H__

#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

#include "glad/glad.h"


#define BIT(x) (1 << x)
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

#endif // __BASE_H__