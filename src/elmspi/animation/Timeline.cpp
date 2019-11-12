#include "Timeline.h"
#include <chrono>s
#include <math.h>

static long getNanoTime()
{
	auto now = std::chrono::system_clock::now();
	auto now_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
	auto epoch = now_ns.time_since_epoch();
	auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch);
	return value.count();
}

Timeline::Timeline(double duration)
{
	this->duration = abs(duration);
	currentFracTime = 0.0;
	speed = 1.0;
	tickTime = getNanoTime();
	paused = false;
	stopped = true;
	rewinding = false;
}

void Timeline::tick()
{
	unsigned long prevTickTime = tickTime;
	tickTime = getNanoTime();
	double passedFracTime = (tickTime - prevTickTime) * 1e-9;
	double fracTimeTick = passedFracTime * speed / duration;

	if (!paused && !stopped)
	{
		if (!rewinding) currentFracTime += fracTimeTick;
		else currentFracTime -= fracTimeTick;
	}

	if (currentFracTime <= 0.0)
	{
		stopped = true;
		paused = false;
		currentFracTime = 0.0;
	}
	else if (currentFracTime >= 1.0)
	{
		stopped = true;
		paused = false;
		currentFracTime = 1.0;
	}
}

double Timeline::getFracTime() const
{
	return currentFracTime;
}

double Timeline::getTimePos() const
{
	return currentFracTime * duration;
}

double Timeline::getSpeed() const
{
	return speed;
}

bool Timeline::isPaused() const
{
	return paused;
}

bool Timeline::isRewinding() const
{
	return rewinding;
}

bool Timeline::hasStopped() const
{
	return stopped;
}

void Timeline::setSpeed(double speed)
{
	this->speed = speed;
}

void Timeline::play()
{
	paused = false;
	stopped = false;
	rewinding = false;
}

void Timeline::rewind()
{
	paused = false;
	stopped = false;
	rewinding = true;
}

void Timeline::pause()
{
	if (!stopped) paused = true;
}

void Timeline::resume()
{
	paused = false;
}

void Timeline::stop()
{
	stopped = true;
	currentFracTime = 0.0;
}