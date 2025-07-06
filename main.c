#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define ROWS 20
#define COLS 20

#define ALIVE 1
#define DEAD 0

//Specify num of COLS so the compiler generates Storage Mapping Function
void print_grid(char grid[][COLS],int tick)
{
	//ANSI Escape Code to clean the terminal
	//If another \033 is used as last element, the next element on screen
	//is interpreted as argument.
	printf("\033[2J");
	for(int i=0;i<ROWS;i++)
	{
		for(int j=0;j<COLS;j++)
		{
			printf(" %c",grid[i][j]);
		}
		printf("\n");
	}
	printf("Tick: %d",tick);
}

void seed_generation(char grid[][COLS])
{
	for(int i=0;i<ROWS;i++)
	{
		for(int j=0;j<COLS;j++)
		{
			grid[i][j]='.';
		}
	}
}
//An alive cell with < 2 live neighbours dies, for underpopulation.
//x and y are respectively row and column index of cell
int cell_next_status(char grid[][COLS],int x,int y)
{
	int cell_status=DEAD;
	if(grid[x][y]=='*')
		cell_status=ALIVE;
	else
		cell_status=DEAD;
	int alive_neighbours=0;
	//vertical, horizontal, oblique adiacent cells (8 neighbours)
	char cell=grid[x][y];
	for(int i=(x-1);i<(x+2);i++)
	{
		for(int j=(y-1);j<(y+2);j++)
		{
			if(i>=0 && j>=0)
			{
				if(i!=x || j!=y)
				{
					if(grid[i][j] == '*')
						alive_neighbours++;
				}
			}
		}	
	}
	if(cell_status==ALIVE && (alive_neighbours <2 || alive_neighbours >3))
		return DEAD;
	if(cell_status==ALIVE && (alive_neighbours == 2 || alive_neighbours == 3))
		return ALIVE;
	if(cell_status==DEAD && (alive_neighbours == 3))
		return ALIVE;
	else
		return cell_status;
}

void generation(char grid[][COLS],char new_grid[][COLS])
{
	for(int i=0;i<ROWS;i++)
	{
		for(int j=0;j<COLS;j++)
		{
			if(cell_next_status(grid,i,j) == ALIVE)
				new_grid[i][j] = '*';
			else
				new_grid[i][j] = '.';
		}
	}
}

int main()
{
	//game map, * = Alive, . = Dead
	char grid[ROWS][COLS];
	seed_generation(grid);
	grid[9][10]='*';
	grid[10][10]='*';
	grid[8][9]='*';
	grid[8][8]='*';
	grid[8][7]='*';
	grid[9][7]='*';
	grid[10][7]='*';
	grid[10][8]='*';
	grid[11][9]='*';
	int tick=0;
	while(1)
	{
		char new_grid[ROWS][COLS];
		seed_generation(new_grid);
		tick++;
		print_grid(grid,tick);
		sleep(1);
		generation(grid,new_grid);
		//I need a char(*) [20][20]
		//It's doable because the occupied memory space is the same
		//1600 bytes.
		memcpy(&grid,&new_grid,sizeof(grid));
	}
	return 0;
}
