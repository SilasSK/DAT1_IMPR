
#include <stdio.h>

void swap(long int *a, long int *b);

int main() 
{
	/* Repeatedly prompt user for two non-negative integers: */
	for (;;) {
		printf("Indtast to ikke-negative heltal hvis største fælles divisor skal beregnes, eller indtast et eller flere negative tal for at afbryde programmet: ");
		long int a, b;
		scanf(" %ld %ld", &a, &b);

		/* Flush the input buffer, to prevent previous input from polluting future inputs */
		/* Stolen from https://stackoverflow.com/a/7898516 */
		int c;
		while ((c = getchar()) != '\n' && c != EOF) { }

		/* Exit the loop / program when given one or more negative numbers: */
		if ( a < 0 || b < 0 )
			break;

		/* Ensure that a <= b (used both to handle an edge case and in the algorithm) */
		if (b < a)
			swap(&a, &b);

		/* Handle edge cases: */
		if (a <= 1 || a == b) {
			printf("GCD(%ld, %ld) = %ld\n", a, b, b); /*depends on b being the larger of a and b*/
			continue;
		}

		/* Calculate GCD(a, b) by checking some or all integers n in [2, a], descending.
		If n divides both a and b, n is *a* common divisor.

		Since [2, a] is checked in descending order, if n is a common divisor of a and b,
		there are no GREATER common divisors of a and b, so n must be GCD(a, b).
		Break out of the loop at this point, because gcd has been found.
		
		If no gcd is found, gcd is 1. Don't check if it is, because 1 divides all integers.
		
		a should be <= b, because no number is divisible by any number greater than itself,
		so checking numbers in (a, b] is useless because they'll never divide a. */
		long int divisor;

		for (divisor = a; divisor >= 2; divisor--) 
		{
			/* If n divides a, then (a % n) == 0 */
			if ((a % divisor) == 0 && (b % divisor) == 0) 
				break;
		}

		printf("GCD(%ld, %ld) = %ld\n", a, b, divisor);
	}

	return 0;
}

void swap(long int *a, long int *b) 
{
	int temp = *a;
	*a = *b;
	*b = temp;
}