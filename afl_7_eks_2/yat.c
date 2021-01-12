
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define DICE_TO_COUNT 5 /* No scoring combination can use more than this number of dice */
#define DICE_NUM_SIDES 6 /* Classic dice have 6 sides, but any number is possible */
#define MAX_DICE_IN_ROLL 100000 /* The largest number of dice that can be rolled */

	/*Function forward declarations*/

void play_game(int num_dice);

/* Fills array 'rolls' with 'num_dice' random numbers from 1 to 'DICE_NUM_SIDES' 
Precondtion: 'rolls' is an array of size 'num_dice' or more. */
void roll_multiple_dice(int num_dice, int rolls[]);

/* Returns a random integer in the range [min; max] */
int rand_between(int min, int max);

/* Returns the smaller of 'a' and 'b' */
int min(int a, int b);

/* Given an array of dice rolls, print that array up to the 'num_dice'th number. */
void print_roll(int roll[], int num_dice);

/* Given an array of numbers 'values', count the number of times that 'compareTo' 
occurs among those values, up the the 'nValues'th value. 
Precondition: 'values' is an array of size 'nValues' or more.*/
int count_occurences(int values[], int nValues, int compareTo);

/* Given an array of 'num_dice' dice rolls, calculates the score that roll is worth when rolling 
for 'num_groups' groups of size 'group_sizes', where all the dies in a group are the same (e.g. 6s).
See definition for details.
*/
int calculate_groups_score(int num_groups, int group_sizes, int rolls[], int num_dice, int dice_in_groups[]);

int main() {
	int stop_playing = 0;
	int c; /*Input variable for scanf, to prompt user to stop or continue playing*/

	int num_dice;

	/* Seed RNG based on system time, to get different rolls each time the program runs. */
	srand(time(NULL));

	do {
		/* Prompt user for the number of dice to roll */
		do {
			printf("Enter a number of dice >= 5 to play with: ");
			scanf(" %d", &num_dice);
			printf("%d\n", num_dice);
		} while( num_dice < 5 || num_dice > MAX_DICE_IN_ROLL );
		
		/* Play a game for the computer, printing rolls and scores for each roll. */
		play_game(num_dice);

		/* Prompt user to play again */
		printf("Enter '1' to play again, or anything else to quit: \n");
		scanf(" %d", &c);
		stop_playing = c != 1;
	} while(!stop_playing);

	return (0);
}

/* Plays a game of yatzy. See specification for details. */
void play_game(int num_dice) {
	int score = 0;

	int rolls[MAX_DICE_IN_ROLL]; /* Can't use num_dice since VLAs are unsupported in C89 and generally unwanted. */

	/* Roll for dies showing numbers [1-6] */
	{
		int target;
		int num_target_rolls, score_increase;
		for (target = 1; target <= DICE_NUM_SIDES; target++) {
			printf("Rolling for %ds: ", target);
			roll_multiple_dice(num_dice, rolls);
			print_roll(rolls, num_dice);

			num_target_rolls = min(DICE_TO_COUNT, count_occurences(rolls, num_dice, target));
			score_increase = target * num_target_rolls;
			printf("Got %d %ds for %d points! ", num_target_rolls, target, score_increase);

			score += score_increase;
			printf("Score: %d\n", score);
		}
	}

	{ 
		if (score >= 63) {
			score += 50;
			printf("+50 points are awarded for going over 62 points so far.");
		} else {
			printf("0 points are awarded for going under 63 points so far.");
		}
		printf(" Score: %d\n", score);
	}

	/* Roll for specific numbers of groups of specific sizes */
	/* 1 pair = 1 group of 2 dice */
	{	
		int dice_in_groups[1]; /* Result of looking for 1 groups */
		int score_increase;

		printf("Rolling for one pair: ");
		roll_multiple_dice(num_dice, rolls);
		print_roll(rolls, num_dice);
		
		score_increase = calculate_groups_score(1, 2, rolls, num_dice, dice_in_groups);
		score += score_increase;
		if (score_increase > 0) {
			printf("1 pair of %ds for %d points!", dice_in_groups[0], score_increase);
			printf(" Score: %d\n", score);
		} else {
			printf("Got nothing...\n");
		}
	}

	/* 2 pairs = 2 groups of 2 dice */
	{
		int dice_in_groups[2]; /* Result of looking for 2 groups */
		int score_increase;

		printf("Rolling for two pairs: ");
		roll_multiple_dice(num_dice, rolls);
		print_roll(rolls, num_dice);
		
		score_increase = calculate_groups_score(2, 2, rolls, num_dice, dice_in_groups);
		score += score_increase;
		if (score_increase > 0) {
			printf("2 pairs of %ds and %ds for %d points!", dice_in_groups[0], dice_in_groups[1], score_increase);
			printf(" Score: %d\n", score);
		} else {
			printf("Got nothing...\n");
		}
	}

	/* 3 of a kind = 1 group of 3 dice */
	{
		int score_increase;
		int dice_in_groups[1]; /* Result of looking for 1 groups */

		printf("Rolling for three of a kind: ");
		roll_multiple_dice(num_dice, rolls);
		print_roll(rolls, num_dice);
		
		score_increase = calculate_groups_score(1, 3, rolls, num_dice, dice_in_groups);
		score += score_increase;
		if (score_increase > 0) {
			printf("Three %ds for %d points!", dice_in_groups[0], score_increase);
			printf(" Score: %d\n", score);
		} else {
			printf("Got nothing...\n");
		}	
	}

	/* 4 of a kind = 1 group of 4 die */
	{
		int score_increase;
		int dice_in_groups[1]; /* Result of looking for 1 groups */

		printf("Rolling for four of a kind: ");
		roll_multiple_dice(num_dice, rolls);
		print_roll(rolls, num_dice);
		
		score_increase = calculate_groups_score(1, 4, rolls, num_dice, dice_in_groups);
		score += score_increase;
		if (score_increase > 0) {
			printf("Four %ds for %d points!", dice_in_groups[0], score_increase);
			printf(" Score: %d\n", score);
		} else {
			printf("Got nothing...\n");
		}	
	}

	/* Roll for small straight (1, 2, 3, 4 and 5) */
	{
		int has_small_straight = 1;
		int i;

		printf("Rolling for a small straight: ");
		roll_multiple_dice(num_dice, rolls);
		print_roll(rolls, num_dice);
		
		for (i = 1; i < 5; i++) {
			if (count_occurences(rolls, num_dice, i) == 0) {
				has_small_straight = 0;
				break;
			}
		}
		if (has_small_straight) {
			printf("Small straight for 15 points!");
			score += 15;
			printf(" Score: %d\n", score);
		} else {
			printf("Got nothing...\n");
		}
	}

	/* Roll for large straight (2, 3, 4, 5 and 6) */
	{
		int has_large_straight = 1;
		int i;

		printf("Rolling for a large straight: ");
		roll_multiple_dice(num_dice, rolls);
		print_roll(rolls, num_dice);
		
		for (i = 2; i < 6; i++) {
			if (count_occurences(rolls, num_dice, i) == 0) {
				has_large_straight = 0;
				break;
			}
		}
		if (has_large_straight) {
			printf("Large straight for 20 points!");
			score += 20;
			printf(" Score: %d\n", score);
		} else {
			printf("Got nothing...\n");
		}
	}

	/* Roll for full house (3 of one type and 2 of another) */
	{
		int has_full_house = 0;
		int large_dice, small_dice;

		printf("Rolling for a full house: ");
		roll_multiple_dice(num_dice, rolls);
		print_roll(rolls, num_dice);
		
		for (large_dice = 6; large_dice > 2; large_dice--) {
			for (small_dice = 6; small_dice > 1; small_dice--) {
				if (large_dice != small_dice) {
					int num_large_dice = count_occurences(rolls, num_dice, large_dice);
					int num_small_dice = count_occurences(rolls, num_dice, small_dice);
					if (num_large_dice >= 3 && num_small_dice >= 2) {
						has_full_house = 1;
						break;
					}
				}
			}

			if (has_full_house)
				break;
		}

		if (has_full_house) {
			int score_increase = 3 * large_dice + 2 * small_dice;
			score += score_increase;
			printf("Got a full house with 3 %ds and 2 %ds for %d points!", large_dice, small_dice, score_increase);
			printf(" Score: %d\n", score);
		} else {
			printf("Got nothing...\n");
		}
	}

	/* Roll for chance (best 5 dice) */
	{
		int score_increase = 0;
		int num_dice_left = DICE_TO_COUNT;
		int target;

		printf("Rolling for chance: ");
		roll_multiple_dice(num_dice, rolls);
		print_roll(rolls, num_dice);
		
		printf("Got ");

		for (target = 6; target > 1; target--) {
			int num_target_occurences = min(num_dice_left, count_occurences(rolls, num_dice, target));
			score_increase += target * num_target_occurences;
			
			if (num_target_occurences > 0) {
				if (num_dice_left == DICE_TO_COUNT) {
					printf("%d %ds", num_target_occurences, target);
				} else if (num_dice_left - num_target_occurences > 0) {
					printf(", %d %ds", num_target_occurences, target);
				} else {
					printf(" and %d %ds", num_target_occurences, target);
					break;
				}
			}
			num_dice_left -= num_target_occurences;
		}
		printf(" for %d points!", score_increase);
		score += score_increase;
		printf(" Score: %d\n", score);
	}

	/* Roll for yatzy (5 of any kind of dice) */
	{
		int has_yatzy = 0;
		int yatzy_dice;
		int target;

		printf("Rolling for Yatzy: ");
		roll_multiple_dice(num_dice, rolls);
		print_roll(rolls, num_dice);
		
		for (target = 6; target > 1; target--) {
			if (count_occurences(rolls, num_dice, target) >= 5) {
				has_yatzy = 1;
				yatzy_dice = target;
				break;
			}
		}

		if (has_yatzy) {
			printf("Got a yatzy with 5 %ds for %d points!\n", yatzy_dice, 50);
			score += 50;
		} else {
			printf("Got nothing...\n");
		}
	}


	printf("Final score: %d\n", score);
}

/* Fills array 'rolls' with 'num_dice' random numbers from 1 to 'DICE_NUM_SIDES' 
Precondtion: 'rolls' is an array of size 'num_dice' or more. */
void roll_multiple_dice(int num_dice, int rolls[])
{
	int i;
	for (i = 0; i < num_dice; i++)
		rolls[i] = rand_between(1, DICE_NUM_SIDES);
}

/* Returns a random integer in the range [min; max] */
int rand_between(int min, int max) 
{
	return min + (rand()%(1 + max - min));
}

/* Returns the smaller of 'a' and 'b' */
int min(int a, int b) {
	return a < b ? a : b;
}

/* Given an array of dice rolls, print that array up to the 'num_dice'th number. */
void print_roll(int roll[], int num_dice)
{	
	int i;
	printf("%d", roll[0]);
	
	for (i = 1; i < num_dice; i++) {
		printf(", %d", roll[i]);
	}
	printf("\n");
}


/* Given an array of numbers 'values', count the number of times that 'compareTo' 
occurs among those values, up the the 'nValues'th value. 
Precondition: 'values' is an array of size 'nValues' or more.*/
int count_occurences(int values[], int nValues, int compareTo) {
	int c = 0;

	int i;
	for (i = 0; i < nValues; i++)
		if (compareTo == values[i])
			c++;

	return c;
}

/* Given an array of 'num_dice' dice rolls, calculates the score that roll is worth when rolling 
for 'num_groups' groups of size 'group_sizes', where all the dies in a group are the same (e.g. 6s).
For each n' group, write the value of each die in that group to 'dice_in_groups[n]'. 
Precondition: dice_in_groups is of size num_groups or more, or is NULL in which case the chosen dies
aren't written.
The score for a group is the value of dies in the group times the size of the group. Two groups
cannot have the same dies in them 
Examples:
When looking for 2 groups of size 1, the roll (6 6 5) gives groups ((6) (5)) for a score of 11.
When rolling for num_groups = 2 groups of group_sizes = 3, the roll
(1 1 1 1 2 2 2 3 4 4 4 4 4 4 5 5) results in groups ((4 4 4) (2 2 2)), for a score of
4*3 + 2*3 = 18. Of the possible groups, the ones with the highest die numbers are chosen.
*/
int calculate_groups_score(int num_groups, int group_sizes, int rolls[], int num_dice, int dice_in_groups[])
{
	int score = 0;
	int num_found_groups = 0; /* How many groups we found so far. */
	
	/* For each possible die result (from highest to lowest, to pick the best groups)... */
	int dice_face;
	for (dice_face = DICE_NUM_SIDES; dice_face > 1; dice_face--)
	{
		/* ... if that die occurs 'group_sizes' or more times... */
		if (count_occurences(rolls, num_dice, dice_face) >= group_sizes) {
			/* ... give score for that group */
			score += dice_face * group_sizes;

			/* Writing the chosen groups to an output array is optional, 
				and ignored if NULL is passed to dice_in_groups. */
			if (dice_in_groups != NULL)
				dice_in_groups[num_found_groups] = dice_face;
			
			/* Stop looking for groups if the target number is reached */
			num_found_groups++;
			if (num_found_groups == num_groups)
				break;
		}		
	}

	/* If we didn't find enough groups, any found groups are worth nothing */
	if (num_found_groups < num_groups)
		score = 0;

	return score;
}

