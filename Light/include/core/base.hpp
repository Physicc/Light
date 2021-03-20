#ifndef __BASE_H__
#define __BASE_H__

#include <memory>
#include <functional>
#include <string>
#include <vector>

#include <iostream>

#define BIT(x) (1 << x)
#define BIND_EVENT_FN(x) std::bind(x, this, std::placeholders::_1)

#endif // __BASE_H__