#pragma once
#include <list>

class Timeline
{
private:
	double duration;
	double currentFracTime;
	double speed;
	unsigned long tickTime;
	bool paused;
	bool stopped;
	bool rewinding;
public:
	Timeline(double duration);
	void tick();
	double getFracTime() const;
	double getTimePos() const;
	double getSpeed() const;
	bool isPaused() const;
	bool isRewinding() const;
	bool hasStopped() const;
	void setSpeed(double speed);
	void play();
	void rewind();
	void pause();
	void resume();
	void stop();
};