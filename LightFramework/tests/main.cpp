#include "core/logging.hpp"

#include "gtest/gtest.h"

int main(int argc, char** argv)
{
	Light::Logger::init();

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
