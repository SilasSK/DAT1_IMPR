
#include <stdio.h>
#include <math.h>

#define NUM_STEPS 200

typedef long double num_t; // also change string pattern to printf in main
typedef num_t (*MATH_FUNCTION)(num_t t);

num_t lerp(num_t, num_t, num_t);
num_t trapezoid_area(num_t width, num_t, num_t);
num_t trapezoid_area_under_curve(MATH_FUNCTION, num_t, num_t);
num_t approximate_area_under_curve(MATH_FUNCTION, num_t, num_t, int);
num_t h(num_t);

/*
Approximates the area under the curve defined by h and two limits.
h can be any function that takes a double and returns a double.
The approximation is done using a "sum of small trapezoids"- approach.
*/
int 
main()
{
	MATH_FUNCTION f = h;
	printf("Area: %.30Lg \n", approximate_area_under_curve(f, -2, 2, NUM_STEPS));

	return 0;
}

/*
Function defining a semi-circle corresponding to the top half of a circle 
centered on (0, 0) and with radius 2.
*/
num_t 
h(num_t x)
{
	return sqrt(4.0 - x*x);
}

/*
Returns the linear interpolation by progress between limit1 and limit2.
*/
num_t 
lerp(num_t limit1, num_t limit2, num_t progress)
{
	return limit1 + (limit2 - limit1) * progress;
}

/*
Returns the area of trapezoid with width width and heights height1 and height2.
*/
num_t 
trapezoid_area(num_t width, num_t height1, num_t height2)
{
	return width * (height1 + height2)/2.0;
}

/*
Returns the area of a trapezoid fitted under the curve defined by function f.
The heights of the trapezoid are f(limit1) and f(limit2).
*/
num_t 
trapezoid_area_under_curve(MATH_FUNCTION f, num_t limit1, num_t limit2)
{
	return trapezoid_area( limit2 - limit1, f(limit1), f(limit2) );
}

/*
Approximates the area under the curve of function f(x), 
between x = limit1 and x = limit2. A "sum of small trapezoids"- approach 
is used, where the actual area is divided into num_steps trapezoids 
each with width (limit2-limit1)/num_steps and heights cooresponding to
the value of f(x) at each side of the trapezoid.
*/
num_t 
approximate_area_under_curve(
	MATH_FUNCTION f, 
	num_t limit1, 
	num_t limit2, 
	int num_steps
)
{
	if (limit1 > limit2)
		perror("approximate_area_under_curve(f, limit1, limit2, num_steps): limit1 is greater than limit2.");

	num_t area = 0;
	int step;
	for (step = 0; step < num_steps; step++)
	{
		/* 
		Interpolating to find x every iteration avoids potential
		acccumulating floating-point error, which might arise if x
		was instead incremented each loop by a pre-calculated step-size.
		*/
		num_t x0 = lerp(limit1, limit2, (num_t)(step+0)/(num_t)num_steps);
		num_t x1 = lerp(limit1, limit2, (num_t)(step+1)/(num_t)num_steps);
		area += trapezoid_area_under_curve(f, x0, x1);
	}

	return area;
}