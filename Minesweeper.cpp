//Aaron Zhang 565556
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <conio.h>
#include <vector>
#include <windows.h>
#include <stdio.h>
using namespace std;

void colour(vector<vector<char> >v, int i, int j) // This function controls the colour of the text displayed (different colours for different numbers)
{
    HANDLE hConsole;
    hConsole = GetStdHandle (STD_OUTPUT_HANDLE);
    if (v[i][j] == '1' or v[i][j] == '4' or v[i][j] == '7'){
        SetConsoleTextAttribute
        (hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
    else if (v[i][j] == '2' or v[i][j] == '5' or v[i][j] == '8'){
        SetConsoleTextAttribute
        (hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }
    else if (v[i][j] == '3' or v[i][j] == '6'){
        SetConsoleTextAttribute
        (hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }
    else {
        SetConsoleTextAttribute
        (hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
    }
}


void printchar (vector<vector<char> >v) // This function prints the whole matrix/2D array
{
    for (int i = 0; i < v.size(); i++){
        for (int j = 0; j < v[i].size(); j++){
            colour(v, i, j);
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


bool checkwin (vector<vector<char> > &surround, vector<vector<char> > &display) // This function checks if the player has completed the game
{
    for (int i = 0; i < surround.size(); i++){
        for (int j = 0; j < surround[i].size(); j++){
            if (surround[i][j] != 'x' and (display[i][j] == '0' or display[i][j] == 'F')){
                return false;
            }
        }
    }
    return true;
}

void gotoxy(int x, int y)
{
  static HANDLE h = NULL;
  if(!h)
    h = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD c = { x, y };
  SetConsoleCursorPosition(h,c);
}

void checksurround (vector<vector<char> > &surround, int x) // This function adds up the number of mines arround each point
{
    for (int i=0; i<x; i++){
        for (int j=0; j<x; j++){
            if (surround[i][j] == 'x'){
                if (i < x-1){
                    if (surround[i+1][j] != 'x'){
                        surround[i+1][j]++;
                    }
                    if (j < x-1 and surround[i+1][j+1] != 'x'){
                        surround[i+1][j+1]++;
                    }
                }
                if (j < x-1){
                    if (surround[i][j+1] != 'x'){
                        surround[i][j+1]++;
                    }
                    if (i > 0 and surround[i-1][j+1] != 'x'){
                        surround[i-1][j+1]++;
                    }
                }
                if (i > 0){
                    if (surround[i-1][j] != 'x'){
                        surround[i-1][j]++;
                    }
                    if (j > 0 and surround[i-1][j-1] != 'x'){
                        surround[i-1][j-1]++;
                    }
                }
                if (j > 0){
                    if (surround[i][j-1] != 'x'){
                        surround[i][j-1]++;
                    }
                    if (i < x-1 and surround[i+1][j-1] != 'x'){
                        surround[i+1][j-1]++;
                    }
                }
            }
        }
    }
}

void floodfill(int x, int y, vector<vector<char> > &surround, int row, vector<vector<char> > &display, bool &lost) // This function uncovers parts of the display matrix based on its value
{
    if (surround[x][y] == '0'){
        surround[x][y] = '-';
        display[x][y] = '-';
        if (x<row-1){
            floodfill(x+1, y, surround, row, display, lost); // The function calls itself to continue to uncover connecting blank spots on the display
        }
        if (x>0){
            floodfill(x-1, y, surround, row, display, lost);
        }
        if (y>0){
            floodfill(x, y-1, surround, row, display, lost);
        }
        if (y<row-1){
            floodfill(x, y+1, surround, row, display, lost);
        }
    }
    else if (surround[x][y] == 'x'){
        lost = true;
    }
    display[x][y] = surround[x][y];
}

void spot(int &play_x, int &play_y, vector<vector<char> > &display, int x, int &choice) // This function controls and displays the players cursor and actions
{
    HANDLE hConsole;
    hConsole = GetStdHandle (STD_OUTPUT_HANDLE);
    char input;
    int prev_x = play_x, prev_y = play_y;
    while (true){
        if (kbhit()){
            input = _getch();
            if (int(input) == -32){ // These are all of the keyboard key click values
                input = _getch();
                if (int(input) == 80 and play_y < x-1){
                    play_y++;//down
                }
                if (int(input) == 72 and play_y > 0){
                    play_y--;//up
                }
                if (int(input) == 75 and play_x > 1){
                    play_x -= 2;//left
                }
                if (int(input) == 77 and play_x < x*2-2){
                    play_x += 2;//right
                }
                gotoxy(prev_x, prev_y);
                colour(display, prev_y, prev_x/2);
                cout << display[prev_y][prev_x/2];
                gotoxy(play_x, play_y);
                SetConsoleTextAttribute
                (hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE ); // This resets the colour to white until changed again
                cout << '*';
                prev_x = play_x;
                prev_y = play_y;
                }
            else if(int(input) == 49){
                choice = 1;
                break;
            }
            else if(int(input) == 50){
                choice = 2;
                break;
            }
        }
    }
}

int main()
{
    HANDLE hConsole;
    hConsole = GetStdHandle (STD_OUTPUT_HANDLE);

    srand(time(0));
    int x, play_x = 0, play_y = 0, choice, size, difficulty, pos_x, pos_y;
    double level;
    bool lost = false, end = false, first = true;
    char play;

    cout << "WELCOME TO MINESWEEPER!\nThe objective of the game is to uncover all the spots that are not mines!";
    cout << "\nThe numbers displayed on the board describe the number of mines surrounding it,\nexcept zeros '0' which show spots that have not yet been uncovered.";
    cout << "\nEmpty spots that are revealed are shown as dashes '-'.\nFeel free to flag any spots that you think are mines!";
    cout << "\nGood luck!\n\n";

    while (not end){
        first = true;
        play_x = 0;
        play_y = 0;
        cout << "SELECT BOARD SIZE\n1 for small 2 for medium 3 for large: "; // Player selects board size
        cin >> size;
        cout << "\nSELECT GAME DIFFICULTY\n1 for easy 2 for medium 3 for hard: "; // Player selects game difficulty
        cin >> difficulty;
        x = size*7;
        vector<char> thing (x, '0');
        vector<vector<char> >surround (x, thing);
        vector<vector<char> >display (x, thing);\

        system("CLS");

        if (difficulty == 1){
            level = 0.04;
        }
        else if (difficulty == 2){
            level = 0.08;            // These are the percentages of the board that are going to be mines
        }
        else if (difficulty ==3){
            level = 0.12;
        }
        else{
            level = 0.80;
        }

        for (int i=0; i<(x*x)*level; i++){ // This loop goes through based on the level, setting random coordinates as mines
            while (true){
                pos_x = rand()%(x-1)+1;
                pos_y = rand()%(x-1)+1;
                if (surround[pos_x][pos_y] != 'x'){
                    break;
                }
            }
            surround[pos_x][pos_y] = 'x';
        }

        while (true){
            printchar (display);
            SetConsoleTextAttribute
            (hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
            cout << "Use the arrow keys to move!\n";
            cout << "Press 1 to click or 2 to flag";
            spot(play_x, play_y, display, x, choice);
            if (choice == 1){
                if (first){
                    first = false;
                    surround[play_y][play_x/2] = '0'; // This ensures the first move is not a mine!
                    checksurround(surround, x);
                }
                floodfill (play_y, play_x/2, surround, x, display, lost);
            }
            else if (choice == 2){
                if (display[play_y][play_x/2] == 'F'){
                    display[play_y][play_x/2] = '0';
                }
                else if (display[play_y][play_x/2] == '0'){
                    display[play_y][play_x/2] = 'F';
                }
            }
            if (lost){
                lost = false;
                system("CLS");
                gotoxy(0,0);
                printchar(surround);
                gotoxy(0, x+1);
                SetConsoleTextAttribute
                (hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
                cout << "You lose!";
                cout << "\nPlay Again? (y/n): ";
                cin >> play;
                if (play == 'n'){
                    end = true;
                }
                system("CLS");
                break;
            }
            else if (checkwin(surround, display)){
                system("CLS");
                gotoxy(0,0);
                printchar(surround);
                gotoxy(0, x+1);
                SetConsoleTextAttribute
                (hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
                cout << "You win!";
                cout << "\nPlay Again? (y/n): ";
                cin >> play;
                if (play == 'n'){
                    end = true;
                }
                system("CLS");
                break;
            }
            system("CLS");
        }
    }
    cout << "Thanks for playing!";
    return 0;
}
