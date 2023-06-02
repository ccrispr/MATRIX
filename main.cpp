#include <cctype>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

const int SIZE = 5;
int rand1; // these two are initialized outside of main, as many functions
           // reference them
int rand2;
bool numberSeen = false; // boolean to see if any number is revealed
bool leftSeen = false;   // boolean to see if left is revealed
bool rightSeen = false;  // vice versa

string getName();
void genShowMatrix(int shownMatrix[][SIZE]);
void genHideMatrix(int rand1, int rand2, int hiddenMatrix[][SIZE]);
void showMatrix(int shownMatrix[][SIZE]);
void setDisplayLeft(int &numberCover1, int &startPoints);
void setDisplayRight(int &numberCover2, int &startPoints);
void eliminate(int row, int col, int shownMatrix, int hiddenMatrix);
bool allZeros(int shownMatrix[][SIZE]);
void guess(int &startPoints, bool numberSeen, int shownMatrix[][SIZE],
           int hiddenMatrix[][SIZE]);
void initialize(int &sP, int &nC1, int &nC2, int shownMatrix[][SIZE],
                int hiddenMatrix[][SIZE]);

void showMatrix2(
    int hiddenMatrix[][SIZE]); // this function is for testing only (feel free
                               // to enable it: go to case 3)

int main() {
  string intro = " Computer Science Project";
  string intro2 = "	JPoole AP Comp Sci";
  string intro3 = "	     ";
  // main heading
  cout << endl;
  cout << "-----------------------------------------------" << endl;
  cout << intro << endl;
  cout << intro2 << endl;
  cout << intro3 << endl;
  cout << "-----------------------------------------------" << endl;

  int shownMatrix[SIZE][SIZE];
  int hiddenMatrix[SIZE][SIZE];
  char restart;

  int startPoints = 100;
  int numberCover1 = -1;
  int numberCover2 = -1;

  enum gameOptions {
    displayLeft = 1,
    displayRight = 2,
    Guess = 3,
    Reset = 4,
    Exit = 5
  }; // uses enum as per project instruction

  initialize(startPoints, numberCover1, numberCover2, shownMatrix,
             hiddenMatrix);
  string playerName =
      getName(); // initializes playerName by calling function (see below)

  cout << "\nWelcome to Jeremy's Non-Rigged Game, " << playerName << "!"
       << endl;

  srand(time(NULL));

  while (startPoints >=
         0) { // main while loop of the program containing the switch case box
    int playerSelection;

    printf("\nGame Menu: %20s %d\n", "Points:", startPoints);
    printf("%d %8d\n", numberCover1, numberCover2);
    cout << "1. Display Left Number" << endl
         << "2. Display Right Number" << endl
         << "3. Guess a Number In-Between" << endl
         << "4. Reset" << endl
         << "5. Exit" << endl;
    // this part is the options screen for the player ("updating" the player
    // with the points balance by keeping it at the top using printf
    cout << "What do you want to do?" << endl;
    cin >> playerSelection; // reads in player selection

    switch (
        playerSelection) { // main switch case for the options player can choose
    case displayLeft:
      setDisplayLeft(numberCover1,
                     startPoints); // this case displays the left number/checks
                                   // if it is already shown, or if the right
                                   // number is shown through if-else statements
      break; // see function below
    case displayRight:
      setDisplayRight(numberCover2,
                      startPoints); // this case is exactly the same as
                                    // displayLeft but regarding the right side
      break; // see function below
    case Guess:
      showMatrix(shownMatrix);   // this function prints the shown matrix
      showMatrix2(hiddenMatrix); // remove the comment on this line to see the
                                 // hidden matrix too
      guess(startPoints, numberSeen, shownMatrix,
            hiddenMatrix);     // gets number from user (see function below)
      showMatrix(shownMatrix); // reprints the matrix after guess has been made
      if (allZeros(shownMatrix)) { // checks if all zeroes,
        cout << "Congratulations, you won the game!\nEnding balance: "
             << startPoints << " points. Good-bye." << endl;
        cout << "Do you want to play another game? (Y/N) ";
        cin >> restart;
        restart = toupper(restart);
        if (restart == 'Y') {
          initialize(startPoints, numberCover1, numberCover2, shownMatrix,
                     hiddenMatrix); // restarts the parameters of the game
          numberSeen = false;       // boolean to see if any number is revealed
          leftSeen = false;         // boolean to see if left is revealed
          rightSeen = false;        // vice versa
          startPoints -= 1;
          cout << "A 1 point penalty will be applied. Good luck!" << endl;
        } else if (restart == 'N') {
          cout << "\nThanks for playing, " << playerName << "!" << endl;
          cout << "You ended with " << startPoints << " points." << endl;
          return 0; // this case quits the program
        }
      } // this case reads in the player's guess and checks if it is correct,
        // updating startPoints accordingly
      break;
    case Reset:
      initialize(startPoints, numberCover1, numberCover2, shownMatrix,
                 hiddenMatrix);
      numberSeen = false; // boolean to see if any number is revealed
      leftSeen = false;   // boolean to see if left is revealed
      rightSeen = false;  // vice versa
      startPoints -= 1;
      cout << "A 1 point penalty will be applied. Good luck!" << endl;
      break;
    case Exit:
      cout << "\nThanks for playing, " << playerName << "!" << endl;
      cout << "You ended with " << startPoints << " points." << endl;
      return 0; // this case quits the program
    default:
      if (playerSelection > 5 || playerSelection < 1) {
        cout << "Please enter a number from 1-5." << endl;
      } else {
        cin >> playerSelection;
      } // should the player not follow directions, this default case will
        // re-prompt/reread their selection
    }
    cout << endl;
  }

  cout << "Oh no! You ran out of points. Better luck next time, " << playerName
       << "!" << endl;

  return 0;
}

// This function prompts the player for their name, and changes it based on the
// text
string getName() {
  string playerName;
  cout << "Enter your name, to start the game: "
       << endl;             // prompts user to enter name
  getline(cin, playerName); // read player name

  int i = 0; // used in the next while loop
  int charCount = 0;

  while (i <= playerName.length() -
                  1) { // makes sure there are no numbers/special characters
    char check = playerName[i]; // extracts the char being checked
    if (isspace(check) ||
        isalpha(check)) { // checks if the character is a letter
      i++;
    } else { // produces an error message if the requirements arent met
      cout << "Error: Your name cannot include numbers or special characters! "
              "Try again: ";
      getline(cin, playerName); // rereads the user input for error
      i = 0;
    }
  }

  playerName[0] =
      toupper(playerName[0]); // makes sure the first letter is uppercase

  for (int j = 1; j < playerName.length();
       j++) {                     // checks one by one to make correct case
    if (isspace(playerName[j])) { // makes uppercase after a space
      playerName[j + 1] = toupper(playerName[j + 1]);
    }
    if (!isspace(playerName[j - 1])) { // makes lowercase letters if its not the
                                       // first letter
      playerName[j] = tolower(playerName[j]);
    }
  }
  return playerName;
}

// this generates a simple matrix in which every spot is -1
void genShowMatrix(int shownMatrix[][SIZE]) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      shownMatrix[i][j] = -1;
    }
  }
}

// generates a random number in-between the two bounds and writes them to the
// hidden matrix
void genHideMatrix(int rand1, int rand2, int hiddenMatrix[][SIZE]) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      hiddenMatrix[i][j] = rand() % (rand2 - rand1) + rand1;
    }
  }

  int tempVal;
  int index1;
  int index2;

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      index1 = i;
      index2 = j;

      for (int k = i; k < 5; k++) {
        for (int h = j; h < 5; h++) {
          if (hiddenMatrix[k][h] < hiddenMatrix[index1][index2]) {
            index1 = k;
            index2 = h;
          }
        }
      }

      tempVal = hiddenMatrix[i][j];
      hiddenMatrix[i][j] = hiddenMatrix[index1][index2];
      hiddenMatrix[index1][index2] = tempVal;
    }
  }
}
/*

void genHideMatrix(int rand1, int rand2, int hiddenMatrix[][SIZE]){
        for(int i = 0; i<5; i++){
                for(int j = 0; j<5; j++){
                        hiddenMatrix[i][j] =rand()%(rand2-rand1)+rand1;}
        }

        int smallNum = hiddenMatrix[i][j];
        int tempVal;
        int index1;
        int index2;

        for(int i = 0; i<5; i++){
                for(int j = 0; j<5; j++){

                        smallNum = hiddenMatrix[i][j];

                        for(int k = 0; k<5; k++){
                                for(int h = 0; h<5; h++){

                                        if(smallNum > hiddenMatrix[i][j]){
                                                smallNum = hiddenMatrix[k][h];
                                                index1 = i;
                                                index2 = j;
                                        }
                                }
                        }
                tempVal = hiddenMatrix[i][j];
                hiddenMatrix[i][j] = hiddenMatrix[index1][index2];
                hiddenMatrix[index1][index2] = tempVal;
                }
        }
}
*/
// this function prints the shown matrix, with -1s or 0s
void showMatrix(int shownMatrix[][SIZE]) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      cout << shownMatrix[i][j] << "\t";
    }
    cout << endl;
  }
}

// shows the matrix of hidden numbers (enable this function under case3)
void showMatrix2(int hiddenMatrix[][SIZE]) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      cout << hiddenMatrix[i][j] << "\t";
    }
    cout << endl;
  }
}

// Changes the left display based on user-input
void setDisplayLeft(int &numberCover1, int &startPoints) {
  if (numberSeen == true && leftSeen == true) {
    cout << "You can already see the bound! Try again." << endl << endl;
  } else if (numberSeen == true) {
    cout << "You can only view one bound! Try again." << endl << endl;
  } else if (numberSeen == false) {
    numberCover1 = rand1;
    cout << "You can only earn 1 point if you guess correctly. You have "
         << startPoints << " points." << endl;
    numberSeen = true;
    leftSeen = true;
  }
}

// Changes the right display based on user-input
void setDisplayRight(int &numberCover2, int &startPoints) {
  if (numberSeen == true && rightSeen == true) {
    cout << "You can already see the bound! Try again." << endl << endl;
  } else if (numberSeen == true) {
    cout << "You can only view one bound! Try again." << endl << endl;
  } else if (numberSeen == false) {
    numberCover2 = rand2;
    cout << "You can only earn 1 point if you guess correctly. You have "
         << startPoints << " points." << endl;
    numberSeen = true;
    rightSeen = true;
  }
}

// changes the row/column of the guessed number
void eliminate(int row, int col, int shownMatrix[][SIZE],
               int hiddenMatrix[][SIZE]) {
  for (int i = 0; i < 5; i++) {
    shownMatrix[col][i] = 0;
    shownMatrix[i][row] = 0;
    hiddenMatrix[col][i] = 0;
    hiddenMatrix[i][row] = 0;
  }
}

// checks if the shownmatrix is all zeros
bool allZeros(int shownMatrix[][SIZE]) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (shownMatrix[i][j] != 0) {
        return false;
      }
    }
  }
  return true;
}

// the longest function, this one checks if the guess was correct. After reading
// the player guess, it assigns points based on the other conditions and if the
// guess was correct.
void guess(int &startPoints, bool numberSeen, int shownMatrix[][SIZE],
           int hiddenMatrix[][SIZE]) {
  int playerGuess;
  bool match = false;
  cout << "Enter your guess: ";
  cin >> playerGuess;
  cout << endl;

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (playerGuess == hiddenMatrix[i][j] && numberSeen == false) {
        int col = i;
        int row = j;
        eliminate(row, col, shownMatrix, hiddenMatrix);
        startPoints += 5;
        match = true;
        cout << "Correct! You earned 5 points." << endl;
      } else if (playerGuess == hiddenMatrix[i][j] && numberSeen == true) {
        int col = i;
        int row = j;
        match = true;
        eliminate(row, col, shownMatrix, hiddenMatrix);
        startPoints += 1;
        cout << "Correct! You earned 1 point." << endl;
      }
    }
  }
  if (match == false && numberSeen == true) {
    startPoints = startPoints - 10;
    cout << "Incorrect! You earned -10 points." << endl;
  } else if (match == false && numberSeen == false) {
    startPoints = startPoints - 5;
    cout << "Incorrect! You earned -5 points." << endl;
  }
}

// this function creates the starting conditions for the game
void initialize(int &sP, int &nC1, int &nC2, int shownMatrix[][SIZE],
                int hiddenMatrix[][SIZE]) {
  int impossibleGuess = 1;
  srand(time(NULL));
  while (rand1 >= rand2) { // regenerates random numbers until the first number
                           // is smaller than the >
    rand1 = rand() % 101 + 100;
    rand2 = rand() % 101 + 200;
    if (rand2 - rand1 == impossibleGuess) {
      rand2 += 1;
    } // prevents the number from being impossible to guess (i.e. between >
  }
  genHideMatrix(rand1, rand2, hiddenMatrix);
  genShowMatrix(shownMatrix);
  nC1 = -1;
  nC2 = -1;
}
