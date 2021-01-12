
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define CODE_MAX_LEN 30
#define MAX_WAREHOUSE_LEN 5
#define MAX_QUALIFIERS_LEN 15 
#define MAX_ID_LEN (CODE_MAX_LEN - (MAX_WAREHOUSE_LEN + MAX_QUALIFIERS_LEN))

typedef struct product_info {
	int 	id;
	char 	warehouse[MAX_WAREHOUSE_LEN + 1];
	char 	qualifiers_code[MAX_QUALIFIERS_LEN + 1];
} product_info;

/* Returns a pointer to the first character 'c' in 
	null-terminated string 'str' for which comp(c) 
	return true. */
char *str_find_if(char *str, int(*comp)(int));

/* Given a string 'code' MMOC code, parse it into a 
product_info struct according to the specification. */
product_info parse_product_code(char *code);

/* Print out the fields of a product_info */
void print_product_code(product_info p_i);

int main()
{
	product_info p_i; /* Forward-declared because C89 */

	/* Allocate a string for the code */
	char code[CODE_MAX_LEN];

	/* Dynamically create a format string of the form "%CODE_MAX_LENs" */
	char format[8];
	sprintf(format, "%%%ds", CODE_MAX_LEN);

	/* Use said format string to read into 'code' */
	printf("Enter a MMOC product code: ");
	scanf( format, code );

	/* Parse the code and print the resulting product_info struct */
	p_i = parse_product_code(code);
	print_product_code(p_i);

	return (0);
}

/* Returns a pointer to the first character 'c' in 
	null-terminated string 'str' for which comp(c) 
	return true. */
char *str_find_if(char *str, int(*comp)(int))
{
	char *c = str - 1; /* str - 1 is never deref'ed */
	while (c++) /* Evaluates to false when \0 is reached */
		if (comp(*c))
			break;
	return c;
}

/* Given a string 'code' MMOC code, parse it into a 
product_info struct according to the specification. */
product_info parse_product_code(char *code)
{
	product_info p_i;

	char *first_letter = code;
	char *first_digit = str_find_if(code, isdigit);
	char *next_letter = str_find_if(first_digit, isalpha);

	int n_warehouse_chars = first_digit  - first_letter;
	int n_code_chars = next_letter - first_digit;

	char code_str[MAX_ID_LEN]; /* Could have been length 
	'n_code_chars', but not in C89 because no VLAs */

	/* Parse first run of 1 or more letters as warehouse code */
	strncpy(p_i.warehouse, code, n_warehouse_chars);
	p_i.warehouse[n_warehouse_chars] = '\0';

	/* Parse subsequent run of 1 or more digits as product ID */
	strncpy(code_str, first_digit, n_code_chars);
	p_i.id = atoi(code_str);
	
	/* Parse subsequent 1 capital letter followed by run of any 
		characters as qualifiers code */
	strcpy(p_i.qualifiers_code, next_letter);

	return p_i;
}

/* Print out the fields of a product_info */
void print_product_code(product_info p_i)
{
	printf(
		"Warehouse: %s\nProduct: %d\nQualifiers: %s\n", 
		p_i.warehouse, p_i.id, p_i.qualifiers_code
	);
}