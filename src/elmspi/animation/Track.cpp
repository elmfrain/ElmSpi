#include "Track.h"

Track::Track()
{
	value = 0.0;
}
Track::Track(double startingValue)
{
	defaultKeys[0] = Keyframe(0.0, startingValue);
	addKeyframe(defaultKeys[0]);
	this->value = 0.0;
}
Track::Track(double startingValue, Easing::Function func)
{
	defaultKeys[0] = Keyframe(0.0, startingValue, func);
	addKeyframe(defaultKeys[0]);
	this->value = 0.0;
}
Track::Track(double startingValue, Easing::Functions func)
{
	defaultKeys[0] = Keyframe(0.0, startingValue, func);
	addKeyframe(defaultKeys[0]);
	this->value = 0.0;
}
Track::Track(double startingValue, double endingValue)
{
	defaultKeys[0] = Keyframe(0.0, startingValue);
	defaultKeys[1] = Keyframe(1.0, endingValue);
	addKeyframe(defaultKeys[0]);
	addKeyframe(defaultKeys[1]);
	this->value = 0.0;
}
Track::Track(double startingValue, double endingValue, Easing::Function func)
{
	defaultKeys[0] = Keyframe(0.0, startingValue, func);
	defaultKeys[1] = Keyframe(1.0, endingValue, func);
	addKeyframe(defaultKeys[0]);
	addKeyframe(defaultKeys[1]);
	this->value = 0.0;
}
Track::Track(double startingValue, double endingValue, Easing::Functions func)
{
	defaultKeys[0] = Keyframe(0.0, startingValue, func);
	defaultKeys[1] = Keyframe(1.0, endingValue, func);
	addKeyframe(defaultKeys[0]);
	addKeyframe(defaultKeys[1]);
	this->value = 0.0;
}

double Track::getValue()
{
	update(timelineHost ? timelineHost->getFracTime() : 0.0);
	return value;
}

void Track::addKeyframe(Keyframe& keyframe)
{
	for (std::list<Keyframe*>::iterator i = keyframes.begin(); i != keyframes.end(); i++)
	{
		Keyframe currentKeyframe = *(*i);
		if (currentKeyframe.timeFracStamp == keyframe.timeFracStamp)
		{
			*i = &keyframe;
			return;
		}
		i++;
		if (i == keyframes.end())
		{
			i--;
			continue;
		}
		Keyframe nextKeyframe = *(*i);
		if ((currentKeyframe.timeFracStamp < keyframe.timeFracStamp) && (keyframe.timeFracStamp < nextKeyframe.timeFracStamp))
		{
			keyframes.insert(i, &keyframe);
			return;
		}
		i--;
	}
	keyframes.insert(keyframes.end(), &keyframe);
}

void Track::appendTo(Timeline& timeline)
{
	timelineHost = &timeline;
}

void Track::update(double fracTime)
{
	getKeyframes(fracTime);

	Easing::Function easingFunc = currentKeyframe ? currentKeyframe->easingFunc : Easing::getEasingFunction(Easing::LINEAR);

	double k2v = nextKeyframe ? nextKeyframe->value : currentKeyframe ? currentKeyframe->value : 0.0;
	double k1v = currentKeyframe ? currentKeyframe->value : 0.0;
	double k2t = nextKeyframe ? nextKeyframe->timeFracStamp : currentKeyframe ? currentKeyframe->timeFracStamp + 1.0 : 1.0;
	double k1t = currentKeyframe ? currentKeyframe->timeFracStamp : 0.0;

	double partialFracTime = (fracTime - k1t) / (k2t - k1t);
	value = (k2v - k1v) * easingFunc(partialFracTime) + k1v;
}

void Track::getKeyframes(double fracTime)
{
	if (keyframes.empty())
	{
		currentKeyframe = nullptr;
		nextKeyframe = nullptr;
	}
	else
	{
		for (std::list<Keyframe*>::iterator i = keyframes.begin(); i != keyframes.end(); i++)
		{
			Keyframe currentKeyframe = *(*i);
			if (keyframes.size() == 1)
			{
				this->currentKeyframe = &currentKeyframe;
				this->nextKeyframe = nullptr;
				return;
			}
			i++;
			if (i == keyframes.end())
			{
				this->currentKeyframe = &currentKeyframe;
				this->nextKeyframe = nullptr;
				return;
			}
			Keyframe nextKeyframe = *(*i);
			if (currentKeyframe.timeFracStamp <= fracTime && fracTime < nextKeyframe.timeFracStamp)
			{
				this->currentKeyframe = &currentKeyframe;
				this->nextKeyframe = &nextKeyframe;
				return;
			}
			i--;
		}
	}
}