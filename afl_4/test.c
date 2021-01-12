
/* 
Repeatedly prompts the user for the coefficients of a quadratic equation 
and prints its roots, exiting when a 0 is entered as the first number. 
This was chosen because if a=0, the equation will be linear instead of 
quadratic.
*/

#include <stdio.h>
#include "quadratic_equation.h"

int main() {

	/* The coefficients of the quadratic equation (ax^2 + bx + c) */
	double a, b, c;

	/* Repeatedly prompt the user for input */
	do {
		printf("Enter numbers for a, b and c in the equation (ax^2 + bx + c), or enter 0 to exit: ");

		/* Read (a), breaking out of the loop if it's 0 */
		scanf(" %lf", &a);
		if (a == 0) {
			break;
		}

		scanf(" %lf %lf", &b, &c);

		solve_and_print_quadratic_equation(a, b, c);
	} while( a != 0 );

	return 0;
}