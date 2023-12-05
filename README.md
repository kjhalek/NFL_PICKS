# NFL_PICKS README

File: README.md
Name: Kyle Halek
Assignment: Web scraper for NFL teams
Section: CPE 2600 - 131

# Description
This project is designed to pull from a website to get the information of all the NFL teams and their wins and losses.
This is done by using the CURL function, which takes a website and returns the HTML contents it has. CURL's return
is quite large with much unneeded content. To parse through this, I used strstr() to start printing the contents of the 
file and stored the next 200 characters into an array. These 200 characters hold the data of team name wins and losses.
Once you run this program, you will see the output of all NFL teams along with their wins and losses as of today.


# Constrictions
Due to time contraints I was unable to get the product I wanted, which was allowing friends/users to make their NFL picks
for the week and compare the results against others. This result would show who made the correct amount of picks against
the others.

# How to build the program
- You will need a C/C++ compiler 
- Download the source code via clone or through the repository
- Change the directory in your terminal to the programs source code that you cloned (use cd)
- Use gcc to compile the program

# How to run the program and reproduce my results
- make the executable with 'make'
- Run the compiled executable './NFL'


