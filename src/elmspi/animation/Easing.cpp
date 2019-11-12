#include "Easing.h"
#define _USE_MATH_DEFINES
#include <math.h>

static void clamp(double& value)
{
	if (value > 1) value = 1.0;
	else if (value < 0) value = 0.0;
}

static double linear(double value)
{
	clamp(value);
	return value;
}
static double instant(double value)
{
	if (value < 1.0) return 0.0;
	else return 1.0;
}

static double inSine(double value)
{
	clamp(value);
	return sin((M_PI * value - M_PI) / 2.0) + 1.0;
}
static double outSine(double value)
{
	clamp(value);
	return sin((M_PI * value) / 2.0);
}
static double inoutSine(double value)
{
	clamp(value);
	return (sin(M_PI * value - (M_PI) / 2.0)) / 2.0 + 0.5;
}
static double outinSine(double value)
{
	clamp(value);
	if (value < 0.5) return sin(value * M_PI) / 2.0;
	else return -sin(value * M_PI) / 2.0 + 1;
}

static double inQuad(double value)
{
	clamp(value);
	return pow(value, 2.0);
}
static double outQuad(double value)
{
	clamp(value);
	return -1.0 * pow(value - 1.0, 2.0) + 1.0;
}
static double inoutQuad(double value)
{
	clamp(value);
	if (value <= 0.5) return 2.0 * pow(value, 2.0);
	else return -2.0 * pow(value - 1.0, 2.0) + 1.0;
}
static double outinQuad(double value)
{
	clamp(value);
	if (value < 0.5) return -2.0 * pow(value - 0.5, 2.0) + 0.5;
	else return 2.0 * pow(value - 0.5, 2.0) + 0.5f;
}

static double inCubic(double value)
{
	clamp(value);
	return pow(value, 3.0);
}
static double outCubic(double value)
{
	clamp(value);
	return pow(value - 1.0, 3.0) + 1.0;
}
static double inoutCubic(double value)
{
	clamp(value);
	if (value <= 0.5) return 4.0 * pow(value, 3.0);
	else return 4.0 * pow(value - 1.0, 3.0) + 1.0;
}
static double outintCubic(double value)
{
	clamp(value);
	return 4.0 * pow(value - 0.5, 3.0) + 0.5;
}

static double inQuart(double value)
{
	clamp(value);
	return pow(value, 4.0);
}
static double outQuart(double value)
{
	clamp(value);
	return -1.0 * pow(value - 1.0, 4.0) + 1.0;
}
static double inoutQuart(double value)
{
	clamp(value);
	if (value <= 0.5) return 8.0 * pow(value, 4.0);
	else return -8.0 * pow(value - 1.0, 4.0) + 1.0;
}
static double outinQuart(double value)
{
	clamp(value);
	if (value < 0.5) return -8.0 * pow(value - 0.5, 4.0) + 0.5;
	else return 8.0 * pow(value - 0.5, 4.0) + 0.5;
}

static double inQuint(double value)
{
	clamp(value);
	return pow(value, 5.0);
}
static double outQuint(double value)
{
	clamp(value);
	return pow(value - 1.0, 5.0) + 1.0;
}
static double inoutQuint(double value)
{
	clamp(value);
	if (value <= 0.5) return 16.0 * pow(value, 5.0);
	else return 16.0 * pow(value - 1.0, 5.0) + 1.0;
}
static double outinQuint(double value)
{
	clamp(value);
	return 16 * pow(value - 0.5, 5.0) + 0.5;
}

static double inExpo(double value)
{
	clamp(value);
	return pow(1000.0, value - 1.0);
}
static double outExpo(double value)
{
	clamp(value);
	return -1.0 * pow(0.001, value) + 1.0;
}
static double inoutExpo(double value)
{
	clamp(value);
	if (value <= 0.5) return pow(1000000.0, value - 0.5) / 2.0;
	else return -1.0 / (2.0 * pow(1000000.0, value - 0.5)) + 1.0;
}
static double outinExpo(double value)
{
	clamp(value);
	if (value < 0.5) return -pow(1000.0, -2.0 * value) / 2.0 + 0.5;
	else return pow(1000.0, 2.0 * value - 2.0) / 2.0 + 0.5;
}

static double inCirc(double value)
{
	clamp(value);
	if (value == 0.0) return 0.0;
	else if (value == 1.0) return 1.0;
	else return 1.0 - sqrt(1.0 - pow(value, 2.0));
}
static double outCirc(double value)
{
	clamp(value);
	if (value == 0.0) return 0.0;
	else if (value == 1.0) return 1.0;
	else return sqrt(1.0 - pow(value - 1.0, 2.0));
}
static double inoutCirc(double value)
{
	clamp(value);
	if (value == 0.0) return 0.0;
	else if (value == 1.0) return 1.0;
	else if (value == 0.5) return 0.5;
	else if (value < 0.5) return 0.5 - sqrt(0.25 - pow(value, 2.0));
	else return sqrt(0.25 - pow(value - 1.0, 2.0)) + 0.5;
}
static double outinCirc(double value)
{
	clamp(value);
	if (value < 0.5) return sqrt(0.25 - pow((value - 0.5), 2.0));
	else return 1.0 - sqrt(0.25 - pow(value - 0.5, 2.0));
}

static double inBack(double value)
{
	clamp(value);
	return 2.0 * pow(value, 4.0) - pow(value, 2.0);
}
static double outBack(double value)
{
	clamp(value);
	return -2.0 * pow(value - 1.0, 4.0) + pow(value - 1.0, 2.0) + 1.0;
}
static double inoutBack(double value)
{
	clamp(value);
	if (value <= 0.5) return 16.0 * pow(value, 4.0) - 2.0 * pow(value, 2.0);
	else return -16.0 * pow(value - 1.0, 4.0) + 2.0 * pow(value - 1.0, 2.0) + 1.0;
}
static double outinBack(double value)
{
	clamp(value);
	if (value < 0.5) return -16.0 * pow(value - 0.5, 4.0) + 2.0 * pow(value - 0.5, 2.0) + 0.5;
	else return 16.0 * pow(value - 0.5, 4.0) - 2.0 * pow(value - 0.5, 2.0) + 0.5;
}

static double inElastic(double value)
{
	clamp(value);
	return sin(4.5 * (value - (5.0 / 9.0)) * M_PI) * pow(value, 4.0) + pow(value, 10.0);
}
static double outElastic(double value)
{
	clamp(value);
	return sin(4.5 * value * M_PI) * pow(value - 1.0, 4.0) - pow(value - 1.0, 10.0) + 1.0;
}
static double inoutElastic(double value)
{
	clamp(value);
	if (value < 0.5) return sin(9.0 * (value - (13.0 / 18.0)) * M_PI) * 8.0 * pow(value, 4.0) + 512.0 * pow(value, 10.0);
	else return sin(9.0 * (value - (7.0 / 18.0)) * M_PI) * -8.0 * pow(value - 1.0, 4.0) - 512.0 * pow(value - 1.0, 10.0) + 1.0;
}
static double outinElastic(double value)
{
	clamp(value);
	if (value < 0.5) return sin(9.0 * value * M_PI) * 8.0 * pow(value - 0.5, 4.0) - 512.0 * pow(value - 0.5, 10.0) + 0.5;
	else return -sin(9.0 * (value - (8.0 / 9.0)) * M_PI) * 8.0 * pow(value - 0.5, 4.0) + 512.0 * pow(value - 0.5, 10.0) + 0.5;
}

static double inBounce(double value)
{
	clamp(value);
	const double m = (-400 / 49.0);

	if (value <= 0.1) return m * value * (value - 0.1);
	else if (value <= 0.3) return m * (value - 0.1) * (value - 0.3);
	else if (value <= 0.65) return m * (value - 0.3) * (value - 0.65);
	else return m * (value - 0.65) * (value - 1.35);
}
static double outBounce(double value)
{
	clamp(value);
	const double m = (400.0 / 49.0);

	if (value <= 0.35) return m * (value + 0.35) * (value - 0.35) + 1.0;
	else if (value <= 0.7) return m * (value - 0.35) * (value - 0.7) + 1.0;
	else if (value <= 0.9) return m * (value - 0.7) * (value - 0.9) + 1.0;
	else return m * (value - 0.9) * (value - 1.0) + 1.0;
}
static double inoutBounce(double value)
{
	clamp(value);
	const double m = (800.0 / 49.0);

	if (value <= 0.05) return -m * value * (value - 0.05);
	else if (value <= 0.15) return -m * (value - 0.05) * (value - 0.15);
	else if (value <= 0.325) return -m * (value - 0.15) * (value - 0.325);
	else if (value <= 0.5) return -m * (value - 0.325) * (value - 0.675);
	else if (value <= 0.675) return m * (value - 0.325) * (value - 0.675) + 1.0;
	else if (value <= 0.85) return m * (value - 0.675) * (value - 0.85) + 1.0;
	else if (value <= 0.95) return m * (value - 0.85) * (value - 0.95) + 1.0;
	else return m * (value - 0.95) * (value - 1.0) + 1.0;
}

Easing::Function Easing::getEasingFunction(Functions function)
{
	switch (function)
	{
	case Easing::LINEAR:
		return linear;
	case Easing::INSTANT:
		return instant;
	case Easing::IN_SINE:
		return inSine;
	case Easing::OUT_SINE:
		return outSine;
	case Easing::INOUT_SINE:
		return inoutSine;
	case Easing::OUTIN_SINE:
		return outinSine;
	case Easing::IN_QUAD:
		return inQuad;
	case Easing::OUT_QUAD:
		return outQuad;
	case Easing::INOUT_QUAD:
		return inoutQuad;
	case Easing::OUTIN_QUAD:
		return outinQuad;
	case Easing::IN_CUBIC:
		return inCubic;
	case Easing::OUT_CUBIC:
		return outCubic;
	case Easing::INOUT_CUBIC:
		return inoutCubic;
	case Easing::OUTIN_CUBIC:
		return outintCubic;
	case Easing::IN_QUART:
		return inQuart;
	case Easing::OUT_QUART:
		return outQuart;
	case Easing::INOUT_QUART:
		return inoutQuart;
	case Easing::OUTIN_QUART:
		return outinQuart;
	case Easing::IN_QUINT:
		return inQuart;
	case Easing::OUT_QUINT:
		return outQuint;
	case Easing::INOUT_QUINT:
		return inoutQuint;
	case Easing::OUTIN_QUINT:
		return outinQuint;
	case Easing::IN_EXPO:
		return inExpo;
	case Easing::OUT_EXPO:
		return outExpo;
	case Easing::INOUT_EXPO:
		return inoutExpo;
	case OUTIN_EXPO:
		return outinExpo;
	case Easing::IN_CIRC:
		return inCirc;
	case Easing::OUT_CIRC:
		return outCirc;
	case Easing::INOUT_CIRC:
		return inoutCirc;
	case Easing::OUTIN_CIRC:
		return outinCirc;
	case Easing::IN_BACK:
		return inBack;
	case Easing::OUT_BACK:
		return outBack;
	case Easing::INOUT_BACK:
		return inoutBack;
	case Easing::OUTIN_BACK:
		return outinBack;
	case Easing::IN_ELASTIC:
		return inElastic;
	case Easing::OUT_ELASTIC:
		return outElastic;
	case Easing::INOUT_ELASTIC:
		return inoutElastic;
	case Easing::OUTIN_ELASTIC:
		return outinElastic;
	case Easing::IN_BOUNCE:
		return inBounce;
	case Easing::OUT_BOUNCE:
		return outBounce;
	case Easing::INOUT_BOUNCE:
		return inoutBounce;
	default:
		return linear;
	}
}
