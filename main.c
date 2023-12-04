/*
File: Mandel.c
Name: Kyle Halek
Assignment: Multiprocessing
Section: CPE 2600 - 131
description: This program generates mandelbrot images based off user inputs.
	This program also allows the use of children and threading your input to
	make the process faster.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "jpegrw.h"
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>
#include <pthread.h>
#include <curl/curl.h>
#include <ctype.h>

typedef struct {
	char teamName[256];
	int wins;
	int losses;
} NFLteams;


int* winsAndLossesChecker(char WLratio[])
{
	int counter = 0;
	int check = 0;
	int* WL = (int*)malloc(2 * sizeof(int));
	while (WLratio[counter] != '\0') {
		if (isdigit(WLratio[counter]) && check == 0 && (WLratio[counter] != '4' || WLratio[counter + 1] != '9')) {
			// If we are here, we are at the first found digit, these are wins:
			WL[0] = WLratio[counter] - '0';
			if (isdigit(WLratio[counter + 1])) {
				WL[0] = WL[0] * 10 + (WLratio[counter + 1] - '0');
			}
			check++;
			counter++;
		} else if (isdigit(WLratio[counter]) && check == 1 && (WLratio[counter - 1] != '4' || WLratio[counter] != '9')) {
			// If we are here, 2nd digit, these are losses:
			WL[1] = WLratio[counter] - '0';
			printf("\nD: %d \n", WL[1]);
			if (isdigit(WLratio[counter + 1])) {
				WL[1] = WL[1] * 10 + (WLratio[counter + 1] - '0');
			}
			return WL;
		} else {
			counter++;
			
		}
	}


	WL[0] = 0; WL[1] = 0;
	return WL;
}

// Callback function to handle the response data
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	char *html_content = (char *)contents;
	NFLteams teams[32]; // 32 NFL teams

	// Define the start of the desired content
	const char *start = "<div class=\"d3-o-club-shortname\">";

	// Find the start of the desired content
	const char *current_start = strstr(html_content, start);

	// Loop over the HTML content
	int team_index = 0;
	while (current_start != NULL) {
		// Print the next 300 characters
		char next_200_chars[201]; // 301 to account for the null terminator
		strncpy(next_200_chars, current_start + strlen(start), 200);
		next_200_chars[200] = '\0'; // Add the null terminator
		//printf("%s\n\n", next_300_chars);

		// Extract the team's name, wins, and losses from the next 300 characters
		char team_name[32]; // Adjust the size as needed
		sscanf(next_200_chars, "%s", team_name);
		printf("%s\n",next_200_chars);

		
		int* WL = winsAndLossesChecker(next_200_chars);
		

		
		

		// Store the team's name and its wins and losses in the teams array
		strcpy(teams[team_index].teamName, team_name);
		teams[team_index].wins = WL[0];
		teams[team_index].losses = WL[1];

		team_index++;

		// Find the start of the next desired content
		current_start = strstr(current_start + strlen(start) + 300, start);
		printf("Team info: %s\n", teams[team_index-1].teamName);
		printf(" W: %d  L: %d\n", teams[team_index - 1].wins, teams[team_index - 1].losses);
	}

	return realsize;
}






// Function to find and print picks from HTML content
void find_and_print_picks(const char *html_content)
{
// const char *team_start = "<div class=\"d3-o-club-shortname\">";
// const char *team_info_start = strstr(html_content, team_start);
// if (team_info_start != NULL) {
//  const char *team_name_end = strstr(team_info_start, "</div>");
//  if (team_name_end != NULL) {
//    char team_name[team_name_end - team_info_start + 1];
//    strncpy(team_name, team_info_start, team_name_end - team_info_start);
//    team_name[team_name_end - team_info_start] = '\0'; // Add the null terminator
//    //printf("%s\n", team_name);
//  }
// }

}

int main(void)
{

	CURL *curl = curl_easy_init(); // Initialize curl

	// Set URL for request:
	curl_easy_setopt(curl, CURLOPT_URL, "https://www.nfl.com/standings/");

	// curl is used for the function to write back to:
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

	// Store data:
	char data[9000];

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "Failed to perform request: %s\n", curl_easy_strerror(res));
	}
	else
	{
		data[sizeof(data) - 1] = '\0'; // Need an end character.
		find_and_print_picks(data);
	}

	return 0;
}
