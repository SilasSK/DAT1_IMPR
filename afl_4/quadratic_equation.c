
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include "quadratic_equation.h"

/* 
Prints the quation and root(s) of (ax^2 + bx + c).
Assumes that a non-linear equation is given, throws an error otherwise. */
void 
solve_and_print_quadratic_equation(double a, double b, double c)
{
	double root1, root2;
    int num_roots;

	assert(a != 0 && "Attempt to compute quadratic roots of linear equation, pass non-zero 1st parameter."); /* The coefficients of a linear - not quadratic - equation were passed. */

	num_roots = quadratic_equation_roots(a, b, c, &root1, &root2);

    if (num_roots == 0)
        printf("There are no roots of (%gx^2 + %gx + %g).\n", a, b, c);
    else {
        printf("The root%s of (%gx^2 + %gx + %g) %s%g",
            (num_roots == 1 ? "" : "s"), 
            a, b, c, 
            (num_roots == 1 ? "is " : "are "),
            root1
        );

        if (num_roots == 2)
        {
        	printf(" and %g", root2);
        }

        printf(".\n");
    }
}

/* 
Computes the root(s) of (ax^2 + bx + c), returning the number of roots and
writing them into *root1 and *root2 if relevant.
Assumes that a non-linear equation is given, throws an error otherwise. */
int 
quadratic_equation_roots(double a, double b, double c, double *root1, double *root2) 
{
	double discriminant = _quadratic_equation_discriminant(a, b, c);
	int num_roots = _quadratic_equation_num_roots(discriminant);	

	assert(a != 0 && "Attempt to compute quadratic roots of linear equation, pass non-zero 1st parameter."); /* The coefficients of a linear - not quadratic - equation were passed. */

	switch(num_roots) {
		case 1:
			*root1 = _quadratic_equation_1st_root(a, b, discriminant);
			*root2 = *root1;
			break;
		case 2:
			*root1 = _quadratic_equation_1st_root(a, b, discriminant);
			*root2 = _quadratic_equation_2nd_root(a, b, discriminant);
			break;
	}
	
	return num_roots;
}

/* Assumes that the 1st root exists, throws an error otherwise */
double 
_quadratic_equation_1st_root(double a, double b, double discriminant)
{
	int num_roots = _quadratic_equation_num_roots(discriminant);
	double root;

	switch (num_roots) {
		case 2:
			root = (-b + sqrt(discriminant))/(2*a);
			break;
		case 1:
			root = -b/2 * a;
			break;
		default:
			perror("Attempt to compute 1st root of quadratic equation with no roots.");
	}

	return root;
}

/* Assumes that the 2nd root exists and that a non-linear equation is given, throws an error otherwise */
double
_quadratic_equation_2nd_root(double a, double b, double discriminant)
{
	assert(discriminant > 0);
	assert(a != 0 && "Attempt to compute quadratic roots of linear equation, pass non-zero 1st parameter."); /* The coefficients of a linear - not quadratic - equation were passed. */

	return (-b - sqrt(discriminant))/(2*a);
}

int 
_quadratic_equation_num_roots(double discriminant)
{
	return discriminant < 0 ? 0 : ( discriminant > 0 ? 2 : 1 );
}

/* Assumes that a non-linear equation is given, throws an error otherwise */
double 
_quadratic_equation_discriminant(double a, double b, double c)
{
	assert(a != 0);
	return b * b - 4 * a * c;
}