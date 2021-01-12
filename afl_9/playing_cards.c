
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

enum Suit {
	Clubs, Diamonds, Hearts, Spades, Joker
};

typedef struct Card {
	enum Suit suit;
	int value;
} Card;

/* LUT for converting from a card value number to a string representing (value + 1). 
1 and 14 are both Ace  */
char *value_strs[14] = {
	"Ace", "Two", "Three", "Four", "Five",
	"Six", "Seven", "Eight", "Nine", "Ten",
	"Jack", "Queen", "King", "Ace"
};

	/* Function forward declarations */
static char* suit_str(enum Suit s);
void print_card(Card *c);
void print_cards(Card cards[], int n_cards);
Card *build_normal_deck();
void add_new_card(Card *deck_position, enum Suit suit, int value);
int comp_card_values(int v1, int v2);
int comp_cards_suit_value_joker(const void *c1_void, const void *c2_void);
void sort_cards_suit_value_joker(Card cards[], int n_cards);
int rand_between(int a, int b);
void shuffle_cards(Card *cards, int n_cards, int times);

int main()
{
	/* Dynamically allocates memory for returned array
		must be free'd later */
	Card *cards = build_normal_deck(); 

	/* Init RNG */
	srand(time(0));

	/* Shuffle and print */
	shuffle_cards(cards, 13*4 + 3, 10);
	print_cards(cards, 13*4 + 3);
	
	/* Sort and print */
	printf("%s\n", "-------------");
	sort_cards_suit_value_joker(cards, (13*4 + 3));
	print_cards(cards, 13*4 + 3);

	free(cards); /* Strictly not necssary since OS probably cleans up after program exit. */

	return (0);
}

/* Convert from enum Suit to string describing that suit. */
static char* suit_str(enum Suit s)
{ 
	switch (s) {
		case Clubs:
			return "clubs";
		case Spades:
			return "spades";
		case Diamonds:
			return "diamonds";
		case Hearts:
			return "hearts";
		default:
			return "INVALID SUIT";
	}
}

/* Given a Card, print it */
void print_card(Card *c)
{
	if (c->suit == Joker)
	{
		printf("Joker\n");
	} else {
		char *_value_str = value_strs[c->value - 1];
		char *_suit_str = suit_str(c->suit);
		printf("%s of %s\n", _value_str, _suit_str);
	}
}

/* Prints an array of 'n_cards' Cards */
void print_cards(Card cards[], int n_cards)
{
	int i;
	for (i = 0; i < n_cards; i++)
	{
		printf("%d: ", i + 1);
		print_card(&cards[i]);
	}
}

/* Creates a new Card with suit 'suit' and value 'value', then writes it to '(*deck_position*)', 
e.g. the next empty position in a deck (i.e. an array) of cards. */
void add_new_card(Card *deck_position, enum Suit suit, int value)
{
	/* Create card */
	Card new_card;
	new_card.suit = suit;
	new_card.value = value;
	
	/* Put in deck */
	*deck_position = new_card;
}

/* Builds a standard deck in an array of cards, dynamically 
allocated and returned to caller. Has values [2; 14] of each suit and 
3 jokers for a total of 55 cards. */
Card *build_normal_deck()
{
	Card *deck = malloc((4 * 13 + 3) * sizeof(Card) );
	Card *current_card = deck;

	enum Suit suit;
	int value;	
	for (suit = 0; suit <= Joker; suit++)
	{
		if (suit != Joker) 
		{
			for (value = 2; value <= 14; value++)
			{
				add_new_card(current_card, suit, value);
				current_card++;
			}
		} else {
			int i;
			for (i = 0; i < 3; i++) 
			{
				/* Card "value" does nothing for Jokers (they don't *have* a value), 
					so a default of 2 is chosen (the lowest value since Ace is 14). */
				add_new_card(current_card, suit, 2);
				current_card++;
			}
		}
	}

	return deck;
}

/* Given two card values, return -1 if the first is lower, 
0 if they're equal or 1 if the first is greater. 
Ace is 14, not 1.*/
int comp_card_values(int v1, int v2)
{
	if (v1 == 1)
		v1 = 14;

	if (v2 == 1)
		v2 = 14;

	return v1 < v2 ? -1 : ( v1 == v2 ? 0 : 1 );
}

/* Given two cards, return -1 if the first is lower, 
0 if they're equal or 1 if the first is greater. Compared
with suit as highest priority, then value, with jokers always
being last. Suits are in the order defined by enum Suit.*/
int comp_cards_suit_value_joker(const void *c1_void, const void *c2_void)
{
	const Card *c1 = c1_void;
	const Card *c2 = c2_void;

	if (c1->suit == c2->suit)
	{
		if (c1->suit == Joker)
			return 0;

		if (c1->value == c2->value)
		{
			return 0;
		} else {
			return comp_card_values(c1->value, c2->value);
		}
	} else {
		return c1->suit < c2->suit ? -1 : 1;
	}
}

/* Sorts an array of cards according to comp_cards_suit_value_joker */
void sort_cards_suit_value_joker(Card cards[], int n_cards)
{
	qsort(cards, n_cards, sizeof(Card), comp_cards_suit_value_joker);
}

/*Returns a random int in [a; b].*/
int rand_between(int a, int b)
{
	int r = rand() % (b - a);
	return a + r;
}

/* Shuffles an array of 'n_cards' Cards 'times' times. */
void shuffle_cards(Card *cards, int n_cards, int times)
{
	int t;
	for (t = 0; t < times; t++) {
		int i;
		for (i = 0; i < n_cards; i++)
		{
			int j = rand_between(i, n_cards);

			Card temp = cards[i];
			cards[i] = cards[j];
			cards[j] = temp;
		}
	}
}