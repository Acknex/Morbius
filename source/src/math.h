#ifndef _MATH_H_
#define _MATH_H_

var ang_lerp_single(var a1, var a2, var f)
{
	var angle = ang(a1 - a2);
	return ang(a1 - (f * angle));
}

ANGLE* ang_lerp(ANGLE* a, ANGLE* a1, ANGLE* a2, var f)
{
	ANGLE ai;
	ai.pan = ang_lerp_single(a1->pan, a2->pan, f);
	ai.tilt = ang_lerp_single(a1->tilt, a2->tilt, f);
	ai.roll = ang_lerp_single(a1->roll, a2->roll, f);
	
	if(a != NULL)
	vec_set(a, &ai);
	
	return vector(ai.pan, ai.tilt, ai.roll);
}

float smootherstep(float edge0, float edge1, float x)
{
	// Scale, and clamp x to 0..1 range
	x = clamp((x - edge0)/(edge1 - edge0), 0.0, 1.0);
	// Evaluate polynomial
	return x*x*x*(x*(x*6 - 15) + 10);
}

#endif // #ifndef _MATH_H_