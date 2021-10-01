#ifndef NDEBUG
	#if (defined(__GNUC__) && __GNUC__ > 8) || (defined(_MSC_VER) && _MSC_VER > 1920)
		#include <filesystem>
		namespace fs = std::filesystem;

		fs::path g_filesystemcpp(__FILE__);
	#endif
#endif

