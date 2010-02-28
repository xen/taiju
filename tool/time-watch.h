#ifndef TAIJU_TIME_WATCH_H
#define TAIJU_TIME_WATCH_H

#include <ctime>

namespace taiju {

class TimeWatch
{
public:
	TimeWatch() : cl_(std::clock()) {}
	~TimeWatch() {}

	std::clock_t clock() const { return std::clock() - cl_; }
	double elapsed() const { return 1.0 * clock() / CLOCKS_PER_SEC; }

	void reset() { cl_ = std::clock(); }

private:
	std::clock_t cl_;

	// Disallows copies.
	TimeWatch(const TimeWatch &);
	TimeWatch &operator=(const TimeWatch &);
};

}  // namespace taiju

#endif  // TAIJU_TIME_WATCH_H
