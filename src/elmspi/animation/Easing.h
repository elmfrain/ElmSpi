#pragma once

namespace Easing
{
	typedef double (*Function)(double);

	enum Functions : unsigned char
	{
		LINEAR, INSTANT,

		IN_SINE, OUT_SINE, INOUT_SINE, OUTIN_SINE,

		IN_QUAD, OUT_QUAD, INOUT_QUAD, OUTIN_QUAD,

		IN_CUBIC, OUT_CUBIC, INOUT_CUBIC, OUTIN_CUBIC,

		IN_QUART, OUT_QUART, INOUT_QUART, OUTIN_QUART,

		IN_QUINT, OUT_QUINT, INOUT_QUINT, OUTIN_QUINT,

		IN_EXPO, OUT_EXPO, INOUT_EXPO, OUTIN_EXPO,

		IN_CIRC, OUT_CIRC, INOUT_CIRC, OUTIN_CIRC,

		IN_BACK, OUT_BACK, INOUT_BACK, OUTIN_BACK,

		IN_ELASTIC, OUT_ELASTIC, INOUT_ELASTIC, OUTIN_ELASTIC,

		IN_BOUNCE, OUT_BOUNCE, INOUT_BOUNCE,
	};

	Function getEasingFunction(Functions function);
}