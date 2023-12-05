/*
File: main.c
Name: Kyle Halek
Assignment: Last lab: Web scraper for NFL teams
Section: CPE 2600 - 131

description: This program is designed to scrape the data from an NFL website
// that has the wins and losses for each team. It then displays the wins and losses
// to the user. End goal: To allow friends to make their NFL picks and calculate
// the total amount of correct choices made by the end of the season.

Time contraints: Due to the need to create a GUI and other sources to keep
// track of this information. There is not enough time to complete this goal.
// I have decided to leave the data with all scraped information for each team
// set into the struct.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>
#include <ctype.h>

// NFLteams is used to keep track of each teams wins and losses
typedef struct
{
	char teamName[256];
	int wins;
	int losses;
} NFLteams;

// winsAndLossesChecker:
// This function is used to calculate the wins and losses for each team.
// It takes the data from the parsed HTML and checks for the 1st (wins) and 2nd (losses) occurences.
// Returns: the wins and losses in a size 2 array.
int *winsAndLossesChecker(char WLratio[])
{
	int counter = 0;
	int check = 0;
	int *WL = (int *)malloc(2 * sizeof(int));
	while (WLratio[counter] != '\0')
	{
		if (isdigit(WLratio[counter]) && check == 0 && (WLratio[counter] != '4' || WLratio[counter + 1] != '9'))
		{
			// If we are here, we are at the first found digit, these are wins:
			WL[0] = WLratio[counter] - '0';
			if (isdigit(WLratio[counter + 1]))
			{
				WL[0] = WL[0] * 10 + (WLratio[counter + 1] - '0');
			}
			check++;
			counter++;
		}
		else if (isdigit(WLratio[counter]) && check == 1 && (WLratio[counter - 1] != '4' || WLratio[counter] != '9'))
		{
			// If we are here, 2nd digit, these are losses:
			WL[1] = WLratio[counter] - '0';
			if (isdigit(WLratio[counter + 1]))
			{
				WL[1] = WL[1] * 10 + (WLratio[counter + 1] - '0');
			}
			return WL;
		}
		else
		{
			counter++;
		}
	}

	WL[0] = 0;
	WL[1] = 0;
	return WL;
}

// write_callback: This funciton is called from CURL to writeback the information provided from the
// website
// 	*contents: data passed in
// 	realsize: size of each element in data (number of processed bytes)

size_t write_callback(void *contents, size_t size, size_t nmemb)
{
	size_t realsize = size * nmemb;
	char *html_content = (char *)contents;
	NFLteams teams[32]; // 32 NFL teams

	// start: will pull content when we find this instance:
	const char *start = "<div class=\"d3-o-club-shortname\">";

	// Will start pulling html content using strstr
	const char *current_start = strstr(html_content, start);

	// Loop over the HTML content
	int team_index = 0;
	while (current_start != NULL)
	{

		// Print the next 200 characters
		char next_200_chars[201]; // 201 to account for the null terminator
		strncpy(next_200_chars, current_start + strlen(start), 200);
		next_200_chars[200] = '\0'; // Add the null terminator
		// printf("%s\n\n", next_300_chars);

		// Extract the team's name, wins, and losses from the next 300 characters
		char team_name[32]; // 32 Total NFL teams
		sscanf(next_200_chars, "%s", team_name);

		// Below commented out, was used to confirm I parsed enough data.
		// printf("%s\n",next_200_chars);

		// WL = the wins and losses of each team in a size 2 array
		int *WL = winsAndLossesChecker(next_200_chars);

		// Store the team's name and its wins and losses in the teams array
		strcpy(teams[team_index].teamName, team_name);
		teams[team_index].wins = WL[0];
		teams[team_index].losses = WL[1];

		team_index++;

		// Find the start of the next desired content
		current_start = strstr(current_start + strlen(start) + 300, start);
		//printf("Team info: %s\n", teams[team_index - 1].teamName);
		//printf(" W: %d  L: %d\n", teams[team_index - 1].wins, teams[team_index - 1].losses);
		printf("%-15s %-5d %-5d\n", teams[team_index - 1].teamName, teams[team_index - 1].wins, teams[team_index - 1].losses);
	}

	return realsize;
}

int main(void)
{

	CURL *curl = curl_easy_init(); // Initialize curl

	// Set URL for request:
	curl_easy_setopt(curl, CURLOPT_URL, "https://www.nfl.com/standings/");

	// curl is used for the function to write back to:
	// write_callback is used to handle the received
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

	// this print statement is placed here to prevent repetition when requeest is triggered for write_callback
	printf("%-15s %-5s %-5s\n", "Team", "Wins", "Losses");
	printf("--------------- ----- -----\n");

	// the Above 2 lines are setting up for the request. Below will perform/trigger the request
	// specified by the options set by curl_easy_setopt and jump into the write_callback function.
	CURLcode request = curl_easy_perform(curl);
	if (request != CURLE_OK) // CURLE_OK = 0
	{
		fprintf(stderr, "Failed to perform request: %s\n", curl_easy_strerror(request));
	}
	return 0;
}
