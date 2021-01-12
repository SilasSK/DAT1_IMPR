
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define NOT_FOUND -1

#define NUM_LETTER_CODEPOINTS 52 /*26 lowercase and uppercase*/
/* Most are ~50 chars, 100 seems safe. Only 1 description 
is in memory at a time, so there's no reason not to make it longer.*/
#define GAME_DESC_MAX_LEN 100 


typedef enum Weekday {
	MAN, TIR, ONS, TOR, FRE, LOR, SON
} Weekday;

typedef struct Team {
	char name[4]; /* Up to 3 letters + \0 */
	int goals_won;
	int goals_lost;
	int points;
} Team;

typedef struct Game {
	int date_weekday;
	int date_month;
	int date_day;
	int date_hour;
	int date_minute;
	Team *team1;
	Team *team2;
	int team1_goals;
	int team2_goals;
	int num_spectators;
} Game;


Team *teams;
int *populated_teams_hashes;
int n_teams = 0;


char *str_find_if(char *str, int(*comp)(int));
int parse_weekday_str(char str[]);
int team_name_hash(char n[], int n_letters);
int letter_num(char l);
int find_int(int *a, int n, int target);
Team *str_to_team(char *str);
Game parse_game_description(char *d);
int team_hash_comp(const void *h1_void, const void *h2_void);
int count_lines();


int main()
{
	/* Allocate memory for global variable arrays.
		How much depends on the hash function.  */
	int hash_max = team_name_hash("zzz", 3);
	teams = malloc( hash_max * sizeof(Team) );
	populated_teams_hashes = malloc( hash_max * sizeof(int) );

	/* Allocate memory for and read in game descriptions,
		parsing them as they're read. */
	int n_lines = count_lines();
	int n_games = 0;
	Game games[n_lines];

	char line[GAME_DESC_MAX_LEN];
	while (fgets(line, GAME_DESC_MAX_LEN, stdin))
	{
		games[n_games++] = parse_game_description( line );
	}

	/* Calculate team scores and goal sums based on Game array */
	int i;
	for (i = 0; i < n_games; i++)
	{
		Game g = games[i];

		g.team1->goals_won += g.team1_goals;
		g.team1->goals_lost += g.team2_goals;

		g.team2->goals_won += g.team2_goals;
		g.team2->goals_lost += g.team1_goals;

		if (g.team1_goals > g.team2_goals) {
			g.team1->points += 3;
		} else if (g.team2_goals > g.team1_goals) {
			g.team2->points += 3;
		} else {
			g.team1->points += 1;
			g.team2->points += 1;
		}
	}

	/* Sort teams according to points, or (goals won - goals lost)
		if they're tied. */
	qsort(populated_teams_hashes, n_teams, sizeof(int), team_hash_comp);
	
	/* Print a score-board */
	printf("%5s%8s%12s%12s\n", "Team", "Points", "Goals won", "Goals lost");
	
	int j;
	for (j = 0; j < n_teams; j++) {
		int h = populated_teams_hashes[j];
		printf(
			"%5s%8d%12d%12d\n", 
			teams[h].name, teams[h].points, teams[h].goals_won, teams[h].goals_lost
		);
	}
	
	return (0);
}

/* Map any 3-letter team name to a unique int,
	useful as a hash function for hash maps. */
int team_name_hash(char n[], int n_letters)
{
	int h = 0;

	int i;
	for (i = 0; i < n_letters; i++)
	{
		h += letter_num(n[i]) * pow(NUM_LETTER_CODEPOINTS, i);
	}

	return h;
}

/* Given a letter char, return its position among 
	the list of letter, in the same order as ASCII*/
int letter_num(char l)
{
	if (isupper(l))
	{
		return l - 'A';
	} else {
		return l - 'a';
	}
}

/* Count the number of lines in stdin, without affecting the file position */
int count_lines()
{
	long int fpos = ftell(stdin);

	char str[100];
	int l = 0;
	while (fgets(str, 100, stdin))
		l++;

	fseek(stdin, fpos, SEEK_SET); /* Reset file position to before call */
	return l;
}

/* Given a game description, parse, write to and return a Game struct. */
Game parse_game_description(char *d)
{
	Game g;

	/*Description format:
	A: letter/alpha
	D: digit
	p: punctuation
	AAA	DDpDD DDpDD AAA	p AAA	D p D*/

	/* Find positions in the description where parameters start*/
	char *weekday = d; /*First letter*/
	char *date_day = str_find_if(weekday, isdigit); /*Next number*/
	/* Next number after next punctuation ('/'') */
	char *date_month = str_find_if(str_find_if(date_day+1, ispunct), isdigit);
	/* Next number after next space*/
	char *date_hour = str_find_if(str_find_if(date_month+1, isspace), isdigit);
	/* Next number after next punctuation (':'') */
	char *date_minute = str_find_if(str_find_if(date_hour+1, ispunct), isdigit);
	/* Next letter */
	char *team1 = str_find_if(date_minute+1, isalpha);
	/* Next letter after next punctuation ('-') */
	char *team2 = str_find_if(str_find_if(team1+1, ispunct), isalpha);
	/* Next number */
	char *team1_goals = str_find_if(team2+1, isdigit);
	/* Next number after next punctuation ('-')*/
	char *team2_goals = str_find_if(str_find_if(team1_goals+1, ispunct), isdigit);

	/* Parse and write to struct Game 'g': */
	g.date_weekday = parse_weekday_str(weekday);
	g.date_day = atoi(date_day);
	g.date_month = atoi(date_month);
	g.date_hour = atoi(date_hour);
	g.date_minute = atoi(date_minute);
	g.team1 = str_to_team(team1);
	g.team2 = str_to_team(team2);
	g.team1_goals = atoi(team1_goals);
	g.team2_goals = atoi(team2_goals);

	return g;
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

/* Given a 3-letter weekday code, return the 
	corresponding enum Weekday */
int parse_weekday_str(char str[])
{
	int r = -1;

	switch(str[0])
	{
		case 'M':
			return MAN;
		case 'O':
			return ONS;
		case 'F':
			return FRE;
		case 'L':
			return LOR;
		case 'S':
			return SON;
		case 'T':
			return str[1] == 'i' ? TIR : TOR;
		default:
			perror("Invalid weekday string");
	}

	return r;
}

/* Given a 3-letter team name, return a Team* to
	a team in 'teams'. If this team hasn't been
	initialized yet, read the team name into said 
	Team, increment n_teams and insert the hash of
	said team in populated_team_hases. */
Team *str_to_team(char *str)
{
	char team_name_str[3];
	int i;
	for (i = 0; i < 3; i++)
	{
		if (isalpha(str[i]))
			team_name_str[i] = str[i];
		else
			break;
	}

	int h = team_name_hash(team_name_str, i);
	Team *t = &teams[h];

	/* Initialize if it hasn't been done before */
	if ( find_int(populated_teams_hashes, n_teams, h) == NOT_FOUND )
	{
		strncpy(t->name, team_name_str, i);
		t->name[i+1] = '\0';
		populated_teams_hashes[n_teams++] = h;
	}

	return t;
}

/* Given an int array 'a' of length 'n', return the 
	subscript that 'target' appears at, or -1.*/
int find_int(int *a, int n, int target)
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (a[i] == target)
		{
			return i;
		}
	}

	return NOT_FOUND;
}

/* Comparison function for sorting an array of 
	team hashes with qsort. First priority is team points, 
	second is (goals won - goals lost) */
int team_hash_comp(const void *h1_void, const void *h2_void)
{
	const int h1 = *(int *)h1_void;
	const int h2 = *(int *)h2_void;
	
	int p1 = teams[h1].points;
	int p2 = teams[h2].points;

	int gd1 = teams[h1].goals_won - teams[h1].goals_lost;
	int gd2 = teams[h2].goals_won - teams[h2].goals_lost;

	int result;
	if (p1 != p2) 
	{
		result = p1 > p2 ? -1 : 1;
	} else if (gd1 != gd2) {
		result = gd1 > gd2 ? -1 : 1;
	}
	return result;
}


