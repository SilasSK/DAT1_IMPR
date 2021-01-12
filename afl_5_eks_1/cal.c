/*
Eksamens- formalia:
Navn: Silas Simmelsgaard Kristensen
E-mail: sskr19@student.aau.dk
Gruppe: A302 
Studieretning: Datalogi
*/

/*
Til udviklingen af dette program har jeg modtaget hjælp fra:
https://en.cppreference.com/
*/

#include <stdio.h>
#include <math.h>

typedef unsigned int bool;

#define UNKNOWN_OPERATOR -1 /* Operators with less than 0 operands is nonsensical, so it's safe to use as an error value */

double run_calculator();
int operator_num_operands(char o);
double do_next_op(char operator, double lhs, double rhs);
bool is_valid_operator(char operand);
int scan_data(char* operator, double* operand);
void print_prompt(double *lhs);

int main() 
{
	/* Instruct the user in the usage of the calculator, and enter the prompt-calculate loop. */
	printf("Enter an operator (+ - * / \\ ^ ! # %% q) followed by an operand, if necessary.\n"); 
	printf("Final result: %.15g\n", run_calculator());	

	return (0);
}

/* Repeatedly prompt the user for operators and operands if necessary, 
	performing the associated calculation or qutting the program. */
double run_calculator()
{
	/* Running result, and left-hand operand to any operation. */
	double accumulator = 0.0; 

	/* Used as input variables for scan_data */
	char operator;
	double operand = 0.0;

	
	print_prompt(&accumulator); 
	/* Loop exits when 'q' is entered, causing scan_data to return 0 (number of operands of "quit operator") */
	while ( scan_data(&operator, &operand) ) 
	{
		accumulator = do_next_op(operator, accumulator, operand);
		print_prompt(&accumulator);
	}

	return accumulator;
}

void print_prompt(double *accumulator)
{
	/* Pretty- print up to 15 significant digits, using decimal or engineering notation, whichever is shortest. */
	printf("%.15g: ", *accumulator);
}

/* 
Reads an operator and optionally an operand from stdin into *operator and *operand.
 Returns the number of operands the operator takes (0, 1 or 2).
 If a unary operator is entered, the operand is set to 0 instead.
*/
int scan_data(char* operator, double* operand)
{
	int num_operands; 
	
	/* Read input one character at a time until we reach a valid operator. */
	char input_char;
	while (scanf("%c", &input_char) && !is_valid_operator(input_char))
		;

	if (is_valid_operator(input_char))
		*operator = input_char;
	else 
		perror("couldn't find valid operator"); /* Could occur if scanf reached EOF */

	/* If the operator is unary don't bother reading an operand */
	num_operands = operator_num_operands(*operator);
	if (num_operands <= 1)
	{
		*operand = 0.0; /* Strictly not necessary, but required by specification */
		return num_operands;
	}

	/* Repeatedly try reading a double into *operand until it's successful. */
	while (!scanf(" %lf", operand)) /* Loop exits when scanf returns 1, i.e. when 1 double was successfully read */
		;

	return num_operands;
}

/*
Returns the number of operands of an operator (0, 1, or 2),
or UNKNOWN_OPERATOR if an invalid/unknown operator was passed.
*/
int operator_num_operands(char o)
{
	if (o == '#' || o == '%' || o == '!')
		return 1; 
	else if (o == '+' || o == '-' || o == '*' || o == '/' || o == '\\' || o == '^')
		return 2;
	else if (o == 'q')
		return 0;
	
	return UNKNOWN_OPERATOR;
}

/*
Returns TRUE or FALSE indicating if a character is a valid / known operator
*/
bool is_valid_operator(char operator)
{
	return (operator_num_operands(operator) != UNKNOWN_OPERATOR);
}

/*
Given an operator and two operators, perform the associated operation with 
	lhs as left-hand-side and rhs as right-hand-side, 
	and return the result. If a unary operator is given, rhs is ignored.
*/
double do_next_op(char operator, double lhs, double rhs)
{
	double result = lhs;
	
	if (operator == '+')
		result += rhs;
	else if (operator == '-')
		result -=  rhs;
	else if (operator == '*')
		result *= rhs;
	else if (operator == '\\' || operator == '/')
	{
		if (rhs != 0) /* Division by 0 is undefined, ignore command */
			result /= rhs;
	}
	else if (operator == '^'){
		result = pow(lhs, rhs);}
	else if (operator == '#') 
	{
		if (lhs >= 0)
			result = sqrt(lhs);
	}
	else if (operator == '%')
		result *= -1;
	else if (operator == '!') 
	{
		if (lhs != 0)
			result = 1/lhs;
	}
	else if (operator == 'q')
		; /* The quit operator doesn't affect the accumulator, do nothing.*/
	else
		perror("unknown operator"); /* Only happens if do_next_op was called incorrectly, no user input should cause this. */

	return (result);
}