/*
Name: Lionel Donato Louis
Date: 10/24/2021
Description: This program creates an island using manipulation of dynamic 2D arrays
Usage: No command line arguments
*/

//Issue with my code: For some reason, the output swaps width and height, apart from that it runs perfectly
//Extra credit of coloring output has been implemented

//Header Files
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "termcolor.hpp"
#include <fstream>

using namespace std;

//Prototyping functions
int** makeParticleMap(int, int, int, int, int, int, int, int);

bool moveExists(int**, int, int, int, int);

int findMax(int**, int, int);

void normalizeMap(int**, int, int, int);


int main(int argc, char** argv)
{
	srand(time(NULL));	//For random number generation

	//Declaring variables
	int i, j;
	int width, height;	//grid width and height
	int windowX, windowY;	//Position to define a drop window from
	int winWidth, winHeight;	//Width and height of drop window
	int numParticles, maxLife, waterLine;

	//PRINTING MENU TO THE USER AND COLLECTING INPUTS
	//as well as reprompting when provided invalid inputs using while loops

	cout << "Welcome to Lionel's island generator! \n" << endl;

	cout << "Enter grid width: ";
	cin >> width;
	while (width <= 0)
	{
		cout << "Invalid width. Enter a value greater than 0: ";
		cin >> width;
	}

	cout << "\nEnter grid height: ";
	cin >> height;
	while (height <= 0)
	{
		cout << "Invalid height. Enter a value greater than 0: ";
		cin >> height;
	}

	cout << "\nEnter drop-window x-coordinate (0 - " << width - 1 << "): ";
	cin >> windowX;
	while (windowX < 0 || windowX > width - 1)
	{
		cout << "Invalid coordinate. Enter a value between 0 and " << width - 1 << ": ";
		cin >> windowX;
	}

	cout << "\nEnter drop-window y-coordinate (0 - " << height - 1 << "): ";
	cin >> windowY;
	while (windowY < 0 || windowY > height - 1)
	{
		cout << "Invalid coordinate. Enter a value between 0 and " << height - 1 << ": ";
		cin >> windowY;
	}

	cout << "\nEnter drop-window width: (1 - " << width - windowX << "): ";
	cin >> winWidth;
	while (winWidth < 1 || winWidth > (width - windowX))
	{
		cout << "Invalid window width. Enter a value between 1 and " << width - windowX << ": ";
		cin >> winWidth;
	}

	cout << "\nEnter drop-window height: (1 - " << height - windowY << "): ";
	cin >> winHeight;
	while (winHeight < 1 || winHeight > (height - windowY))
	{
		cout << "Invalid window height. Enter a value between 1 and " << height - windowY << ": ";
		cin >> winHeight;
	}

	cout << "\nEnter number of particles to drop: ";
	cin >> numParticles;

	cout << "\nEnter max life of particles: ";
	cin >> maxLife;

	cout << "\nEnter value of waterline (40-200): ";
	cin >> waterLine;
	cout << endl;
	
	//This function builds the array for the map data and performs particle roll algorithm to populate the array with map data.
	int** map = makeParticleMap(width, height, windowX, windowY, winWidth, winHeight, numParticles, maxLife);

	//Polishing the island

	//Normalizing values to be between 0 and 255 always
	int maxVal = findMax(map, width, height);
	normalizeMap(map, width, height, maxVal);

	//Creating a 2D character array with same size as map array
	char** polishedMap = new char* [width];
	for (i = 0; i < width; i++)
		polishedMap[i] = new char[height];

	int landZone = 255 - waterLine;

	//Processing through values in map array and setting a character in 
	//polishedMap array based on value's relationship to water-line
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			//deep water
			if (map[i][j] < (int)(0.5 * waterLine))
				polishedMap[i][j] = '#';

			//shallow water
			else if ((map[i][j] >= (int)(0.5 * waterLine)) && (map[i][j] <= waterLine))
				polishedMap[i][j] = '~';

			//coast/beach
			else if (map[i][j] < waterLine + (int)(0.15 * landZone))
				polishedMap[i][j] = '.';

			//plains/grass
			else if ((map[i][j] >= waterLine + (int)(0.15 * landZone)) && (map[i][j] < waterLine + (int)(0.4 * landZone)))
				polishedMap[i][j] = '-';

			//forests
			else if ((map[i][j] >= waterLine + (int)(0.4 * landZone)) && (map[i][j] < waterLine + (int)(0.8 * landZone)))
				polishedMap[i][j] = '*';

			//mountains
			else
				polishedMap[i][j] = '^';
		}
	}

	//Creating a new file called 'island.txt' for printing the output in it
	ofstream outfile;
	outfile.open("island.txt", ios::out);

	//Colors don't seem to work on the text file, still gave it a shot regardless

	//Printing out the map to the console and a file by using different colors
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			if (polishedMap[i][j] == '#')
			{
				cout << termcolor::blue << polishedMap[i][j];
				outfile << termcolor::blue << polishedMap[i][j];
			}
			else if (polishedMap[i][j] == '~')
			{
				cout << termcolor::bright_blue << polishedMap[i][j];
				outfile << termcolor::bright_blue << polishedMap[i][j];
			}
			else if (polishedMap[i][j] == '.')
			{
				cout << termcolor::yellow << polishedMap[i][j];
				outfile << termcolor::yellow << polishedMap[i][j];
			}
			else if (polishedMap[i][j] == '-')
			{
				cout << termcolor::bright_green << polishedMap[i][j];
				outfile << termcolor::bright_green << polishedMap[i][j];
			}	
			else if (polishedMap[i][j] == '*')
			{
				cout << termcolor::green << polishedMap[i][j];
				outfile << termcolor::green << polishedMap[i][j];
			}
			else
			{
				cout << termcolor::white << polishedMap[i][j];
				outfile << termcolor::white << polishedMap[i][j];
			}		
		}
		cout << termcolor::reset << endl;
		outfile << termcolor::reset << endl;
	}

	outfile.close();

	//De-allocating the memory from the heap
	for (i = 0; i < width; i++)
	{
		delete[] map[i];
		delete[] polishedMap[i];
	}
	delete[] map;
	delete[] polishedMap;

	return 0;
}

//This function builds the array for the map data and performs particle roll algorithm to populate the array with map data.
int** makeParticleMap(int width, int height, int windowX, int windowY, int winWidth, int winHeight, int numParticles, int maxLife)
{
	int life = maxLife;	//Using another variable to use as life, so that maxLife doesn't get reset
	int i, j;
	//Creating dynamically allocated 2D array based on width and height
	int** map = new int* [width];
	for (i = 0; i < width; i++)
		map[i] = new int[height];

	//Making sure every element of the array is 0 at the start
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			map[i][j] = 0;
		}
	}
	
	//Loop for dropping each particle
	for (j = 0; j < numParticles; j++)
	{
		//Picking a random x,y location within the window to drop a particle
		//Syntax for generating a random number in the given range:
		//value = (rand() % range_value) + minimum_value;
		int x = (rand() % winWidth) + windowX;
		int y = (rand() % winHeight) + windowY;

		//Repeating the process until the particle runs out of life or there are no valid moves
		while (life > 0 && moveExists(map, width, height, x, y) == true)
		{
			//Dropping the particle in the chosen location, by incrementing by 1
			map[x][y] += 1;

			//Checking for if there is a valid move that the particle can make
			if (moveExists(map, width, height, x, y) == true)
			{
				//Since a valid move exists, we have to pick a random valid move
				int move[8];	//Array to represent all possible moves
				//If array element is 1, then valid move exits. Else that move is not valid
				for (i = 0; i < 8; i++)
				{
					move[i] = 0;	//Initializing all elements in this array to 0
				}

				//Now we check which moves are possible, and make that correspnding array element 1

				//upper left: (x-1, y-1)
				if ((x - 1 >= 0) && (y - 1 >= 0))
				{
					if (map[x - 1][y - 1] <= map[x][y])
						move[0] = 1;
				}
				//upper: (x, y-1)
				if (y - 1 >= 0)
				{
					if (map[x][y - 1] <= map[x][y])
						move[1] = 1;
				}
				//upper right: (x+1, y-1)
				if ((x + 1 < width) && (y - 1 >= 0))
				{
					if (map[x + 1][y - 1] <= map[x][y])
						move[2] = 1;
				}
				//left: (x-1, y)
				if (x - 1 >= 0)
				{
					if (map[x - 1][y] <= map[x][y])
						move[3] = 1;
				}	
				//right: (x+1, y)
				if (x + 1 < width)
				{
					if (map[x + 1][y] <= map[x][y])
						move[4] = 1;
				}
				//bottom left: (x-1, y+1)
				if ((x - 1 >= 0) && (y + 1 < height))
				{
					if (map[x - 1][y + 1] <= map[x][y])
						move[5] = 1;
				}	
				//bottom: (x, y+1)
				if (y + 1 < height)
				{
					if (map[x][y + 1] <= map[x][y])
						move[6] = 1;
				}	
				//bottom right: (x+1, y+1)
				if ((x + 1 < width) && (y + 1 < height))
				{
					if (map[x + 1][y + 1] <= map[x][y])
						move[7] = 1;
				}

				//Now we select a random move between 0 and 7
				int selection = rand() % 8;
				//If the random move chosen is not valid, then we try generating another random move
				//till we get a valid move
				while (move[selection] == 0)
					selection = rand() % 8;

				//Now that a valid move has been chosen at random, we update the particle's position
				switch (selection)
				{
				case 0:
				{
					x = x - 1;
					y = y - 1;
					break;
				}
				case 1:
				{
					y = y - 1;
					break;
				}
				case 2:
				{
					x = x + 1;
					y = y - 1;
					break;
				}
				case 3:
				{
					x = x - 1;
					break;
				}
				case 4:
				{
					x = x + 1;
					break;
				}
				case 5:
				{
					x = x - 1;
					y = y + 1;
					break;
				}
				case 6:
				{
					y = y + 1;
					break;
				}
				case 7:
				{
					x = x + 1;
					y = y + 1;
					break;
				}
				}

				//Now that a move has been made, particle's life is reduced
				life -= 1;
			}
		}
		//After a particle's life gets over, we reset the life to maxLife
		//to prepare for the next particle
		life = maxLife;
	}
	return map;
}

//This function looks a the 8 spots around x,y and determines if a valid move is possible.
bool moveExists(int** map, int width, int height, int x, int y)
{
	//upper left: (x-1, y-1)
	if ((x - 1 >= 0) && (y - 1 >= 0))
	{
		if (map[x - 1][y - 1] <= map[x][y])
			return true;
	}

	//upper: (x, y-1)
	else if (y - 1 >= 0)
	{
		if (map[x][y - 1] <= map[x][y])
			return true;
	}

	//upper right: (x+1, y-1)
	else if ((x + 1 < width) && (y - 1 >= 0))
	{
		if (map[x + 1][y - 1] <= map[x][y])
			return true;
	}

	//left: (x-1, y)
	else if (x - 1 >= 0)
	{
		if (map[x - 1][y] <= map[x][y])
			return true;
	}

	//right: (x+1, y)
	else if (x + 1 < width)
	{
		if (map[x + 1][y] <= map[x][y])
			return true;
	}

	//bottom left: (x-1, y+1)
	else if ((x - 1 >= 0) && (y + 1 < height))
	{
		if (map[x - 1][y + 1] <= map[x][y])
			return true;
	}

	//bottom: (x, y+1)
	else if (y + 1 < height)
	{
		if (map[x][y + 1] <= map[x][y])
			return true;
	}

	//bottom right: (x+1, y+1)
	if ((x + 1 < width) && (y + 1 < height))
	{
		if (map[x + 1][y + 1] <= map[x][y])
			return true;
	}

	//no valid move
	else
		return false;
}

//This function finds the maximum value in the map and returns it.
int findMax(int** map, int width, int height)
{
	int i, j;
	int max = map[0][0];
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			if (map[i][j] > max)
			{
				max = map[i][j];
			}
		}
	}
	return max;
}

//Performs the normalization operation on the map data.
void normalizeMap(int** map, int width, int height, int maxVal)
{
	int i, j;
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			//Diving every value by maximum and then multiplying by 255
			map[i][j] = (int)(((float)map[i][j] / (float)maxVal) * 255);
		}
	}
}