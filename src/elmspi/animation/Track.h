#pragma once
#include "Easing.h"
#include "Timeline.h"
#include <list>

struct Keyframe
{
	Easing::Function easingFunc;
	double timeFracStamp;
	double value;
	Keyframe(double timeStamp, double value, Easing::Function func)
	{
		this->easingFunc = func;
		this->value = value;
		this->timeFracStamp = timeStamp;
	}
	Keyframe(double timeStamp, double value, Easing::Functions func)
	{
		this->easingFunc = Easing::getEasingFunction(func);
		this->value = value;
		this->timeFracStamp = timeStamp;
	}
	Keyframe(double timeStamp, double value)
	{
		this->easingFunc = Easing::getEasingFunction(Easing::LINEAR);
		this->value = value;
		this->timeFracStamp = timeStamp;
	}
	Keyframe()
	{
		this->easingFunc = Easing::getEasingFunction(Easing::LINEAR);
		this->value = 0.0;
		this->timeFracStamp = 0.0;
	}
};

class Track
{
private:
	std::list<Keyframe*> keyframes;

	Timeline* timelineHost = nullptr;

	Keyframe* currentKeyframe = nullptr;
	Keyframe* nextKeyframe = nullptr;
	Keyframe defaultKeys[2];

	double value;

	void update(double fracTime);;
	void getKeyframes(double fracTime);
public:
	Track();
	Track(double startingValue);
	Track(double startingValue, Easing::Function func);
	Track(double startingValue, Easing::Functions func);
	Track(double startingValue, double endingValue);
	Track(double startingValue, double endingValue, Easing::Function func);
	Track(double startingValue, double endingValue, Easing::Functions func);
	double getValue();
	void addKeyframe(Keyframe& keyframe);
	void appendTo(Timeline& timeline);
};
