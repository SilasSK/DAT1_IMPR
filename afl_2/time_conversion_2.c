
#include <stdio.h>
#include <stdlib.h>

#define SECONDS_TO_MINUTES(seconds, remainder_ptr) divide_and_write_remainder(seconds, 60, remainder_ptr)
#define MINUTES_TO_HOURS(minutes, remainder_ptr) divide_and_write_remainder(minutes, 60, remainder_ptr) 
#define HOURS_TO_DAYS(hours, remainder_ptr) divide_and_write_remainder(hours, 24, remainder_ptr)
#define DAYS_TO_WEEKS(days, remainder_ptr) divide_and_write_remainder(days, 7, remainder_ptr)

int divide_and_write_remainder(int dividend, int divisor, int *remainder_ptr) {
    // Returns integer division of dividend by divisor, and writes the remainder to *remainder_ptr.
    int quotient = (dividend / divisor); // Integer division results in trucation / flooring towards 0.
    *remainder_ptr = dividend % divisor;
    return quotient;
}

const char* OUTPUT_FORMAT_STRING = "%d uger, %d dage, %d timer, %d minutter og %d sekunder\n";

void print_seconds_as_wdhms(int seconds) {
    int minutes, hours, days, weeks;
    
    minutes = SECONDS_TO_MINUTES(seconds, &seconds);
    hours = MINUTES_TO_HOURS(minutes, &minutes);
    days = HOURS_TO_DAYS(hours, &hours);
    weeks = DAYS_TO_WEEKS(days, &days);

    // Write number of unit of time if not 0, with optional plural trailing "s"
    if (weeks > 0) {
        printf("%d week%s, ", weeks, weeks > 1 ? "s" : "");
    }
    if (days > 0) {
        printf("%d day%s, ", days, days > 1 ? "s" : "");
    }
    if (hours > 0) {
        printf("%d hour%s, ", hours, hours > 1 ? "s" : "");
    }
    if (minutes > 0) {
        printf("%d minute%s ", minutes, minutes > 1 ? "s" : "");
    }

    // Only write "and" if other units of time have already been mentioned:
    printf("%s%d seconds\n", minutes > 0 ? "and " : "" ,seconds);
}

int main(int argc, char *argv[]) {
    int seconds;

    if (argc > 1) { // Read input from command-line arguments
        seconds = atoi(argv[1]);    
    } else { // ... or interactively prompt user for input
        printf("Enter a number of seconds to be converted to weeks, days, hours, minutes and seconds: ");
        scanf("%d", &seconds);
    }

    print_seconds_as_wdhms(seconds);
    
    return 0;
}
