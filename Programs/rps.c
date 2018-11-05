#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


//A game of Rock, Paper, Scissors where the user plays against the random
//output generating computer. Number of rounds can be chosen for a best of
//game.

//This structure saves the game state at any given moment, specifying how
//many rounds have been played, and who's won how many rounds

struct game
{
    int won[2][1];
    int rounds;
};
typedef struct game Game;

//Initialise constants for Rock, Paper and Scissors states
enum { R = 'R', P = 'P', S = 'S' };

//Functions

//Initiate structure for new game
void newGame(Game *g, int rounds)
{
  *g = (Game) { {{0}, {0}}, rounds};
}

//Decide what move the computer is about to make
char computerMove()
{
  char move = R;
  //Create an array containing options, then produce a pseudo random number
  //between 0 and 2 to choose which option the computer will pick
  time_t t;
  char options[3] = {R, P, S};
  //Using current time as a seed to produce the random number for increased
  //unpredictability
  srand((unsigned) time(&t));
  move = options[rand() % 3];
  return move;
}

//Functions to update the game state depending on the user or the computer
//winning
void ifUserWins(Game *g, int totalRounds)
{
  g->won[0][0] = (g->won[0][0] + 1);
  g->rounds = g->rounds - 1;
  printf("User wins this round.\n\n");
}

void ifComputerWins(Game *g, int totalRounds)
{
  g->won[1][0] = (g->won[1][0] + 1);
  g->rounds = g->rounds - 1;
  printf("Computer wins this round.\n\n");
}

//Check to see who won or determine whether the round was a draw
void outcome(Game *g, char* userInput, int totalRounds)
{
   char compMove = R;
   compMove = computerMove();

   //If both inputs are the same, round ends in draw.
   if (strcmp(&compMove, userInput) == 0)
   {
     printf("Draw. Try again.\n");
   }
   else if ((compMove == S) && (userInput[0] == R))
   {
     printf("\nUser chooses Rock. Computer chooses Scissors.\n");
     ifUserWins(g, totalRounds);
   }
   else if ((compMove == P) && (userInput[0] == R))
   {
     printf("\nUser chooses Rock. Computer chooses Paper.\n");
     ifComputerWins(g, totalRounds);
   }
   else if ((compMove == R) && (userInput[0] == P))
   {
     printf("\nUser chooses Paper. Computer chooses Rock.\n");
     ifUserWins(g, totalRounds);
   }
   else if ((compMove == S) && (userInput[0] == P))
   {
     printf("\nUser chooses Paper. Computer chooses Scissors.\n");
     ifComputerWins(g, totalRounds);
   }
   else if ((compMove == P) && (userInput[0] == S))
   {
     printf("\nUser chooses Scissors. Computer chooses Paper.\n");
     ifUserWins(g, totalRounds);
   }
   else if ((compMove == R) && (userInput[0] == S))
   {
     printf("\nUser chooses Scissors. Computer chooses Rock.\n");
     ifComputerWins(g, totalRounds);
   }
}

bool gameWon(Game *g, int totalRounds)
{
  int userWon = g->won[0][0];
  int compWon = g->won[1][0];
  //Check to see if either user or computer has won the majority of rounds. If
  //so, determine them the winner
  if (userWon == ((totalRounds + 1) / 2))
  {
    printf("User wins best of %d! %d rounds to %d\n\n",totalRounds, userWon, compWon);
    return true;
  }
  else if (compWon == ((totalRounds + 1) / 2))
  {
    printf("Computer wins best of %d! %d rounds to %d\n\n",totalRounds, compWon, userWon);
    return true;
  }
  else
    return false;
}

//Validation to ensure on the three characters are allowed
bool inputCheck(char user)
{
  if (user == R || user == P || user == S)
  {
    return true;
  }
  else
  {
    printf("Ensure you input a valid character.\n");
    return false;
  }
}

void play(Game *g)
{
  // Initialise userInput and allocate memory. Also initialise the total rounds
  //that will be played, as it will be required for when the current rounds
  //is stated.
  char *userInput;
  userInput = malloc(1);
  int totalRounds = g->rounds;

  //Loop the game until either party wins the game
  while (gameWon(g, totalRounds) == false)
  {
    //Take the user input
    printf("Time for Round %d\n", (totalRounds - (g->rounds - 1)));
    char *userInput;
    userInput = malloc(1);
    printf("Choose your move; R for Rock, P for Paper and S for Scissors\n\n");
    fgets(userInput, 4, stdin);

    //Only if the input is valid, then carry on with the game
    if (inputCheck(*userInput) == true)
    {
      outcome(g, userInput, totalRounds);
    }
  }
}

int main(int n, char *args[n])
{
    setbuf(stdout, NULL);
    int intRounds = (args[1][0] - '0');
    Game gdata = { {{0}, {0}}, 0 };
    Game *g = &gdata;

    //Ensures that there is only one argument entered, that is an integer,
    //that is odd and it is less than 10 as my game only allows up to 9 rounds
    //per game played.
    if (n == 2 && (isdigit(args[1][0]) && (intRounds < 10) && (intRounds % 2 == 1)))
    {
      *g = (Game) { {{0}, {0}}, intRounds };
      play(g);
    }

    else
    {
        fprintf(stderr, "Use: ./rps to test OR  ./rps followed by an odd number indicating the number of rounds (maximum 9) you would like to play\n");
        exit(1);
    }

    return 0;
}
