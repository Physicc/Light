#ifndef __APPSTATS_H__
#define __APPSTATS_H__

namespace Light
{
	struct AppStats
	{
		double m_fps; // Frames per second (Exponential Average). See https://stackoverflow.com/questions/4687430/c-calculating-moving-fps
		double m_mspf; // Milliseconds per frame
		const double m_alpha = 0.9; // Alpha for the exponential average
	};
} // namespace Light


#endif // __APPSTATS_H__
