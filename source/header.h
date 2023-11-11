#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//ships of length 5, 4, 3, 3, 2 are hardcoded and cannot be changed
#define BOARD_SIZE 10//cannot be more than 10
#define BORDER_SIGN '/'
#define HIT_SIGN '#'
#define MISS_SIGN '+'
#define EMPTY_SIGN ' '
#define HUMAN_SHIP_SIGN '@'
#define COMPUTER_SHIP_SIGN '^'
#define STARTING_DIFFICULTY 1
#define STARTING_SUNK 1

typedef struct player{
    char board[BOARD_SIZE][BOARD_SIZE]; //player's board, contains data about ships' placement and fields' status
    short ships[5][5][3];               //first dimension is a number of the ship [0] for 5, [1] for 4, [2] and [3] for 3, [4] for 2;
                                        //for every field of the ship first two cells holds coordinates, the third one holds data, 
                                        //0 = undamaged field, 1 = damaged, 2 = destroyed (when all ship is 1 it is destroyed)
    short ships_lost;                   //number of lost ships
    bool is_human;
}player;

typedef struct aidata{
    char probability_board[BOARD_SIZE][BOARD_SIZE];
    short ships_alive[5];
    bool hitmode;                       //false if disabled, true if enabled
    short fields_hit_in_hitmode;        //number of fields hit in current hit-mode
    short fields_sunk_in_hitmode;       //number of fields sunk in current hit-mode (if ship of length 5 is sunk completly, +=5)
    char fields_hit_coordinates[17][2]; //coordinates of fields hit in current hit-mode, max 17, second dimension contains coordinates,
                                        //first dimension is index number (max 17, because that's the number of all fields occupied by ships)
}aidata;

void start();                                   //starting menu
void board_start();                             //fills boards of players with ' '
bool coordinates_correct(char, char);           //checks whether coordinates can exists
bool board_place_ship(player*, short, char, char, char, char, bool);//checks if player can place a ship of the given length between coordinates,
                                                                    //additional bool because 2 ships have length 3, returns true if can
void board_init_human();                        //requires human to give input that places ships on the board
void board_init_computer();                     //places computer ships on the board at random positions
void board_print(bool, bool);                   //prints boards in terminal, hides computer's ships, if first bool false prints only human's board,
                                                //if second true prints with computer ships revealed
void new_turn();                                //prints 'turn number x' line
bool can_be_shoted(player*, char, char);        //can this place be shoted
bool ship_is_destroyed(player*, short, short);  //checks whether ship is destroyed and adds 1 to player.ships_lost if it is
bool shot(player*, char, char);                 //shots at the given location, returns false when player has lost all of the ships
bool move_human();                              //human makes his move
bool move_computer_0();                         //computer makes his move on difficulty 0
bool move_computer_1();                         //computer makes his move on difficulty 1
bool game(bool);                                //main game loop function, takes if human starts, returns false when the game ends, true if it continues
void print_shot_result(short, short, bool);     //prints shot result based on first input 1 = HIT, 2 = SUNK, else = MISS, second is ship's length, bool is player?

extern short difficulty;                        //0 computer shots at random places (with hit-mode), computer uses algorythm
extern short sunk;                              //0 if no sunk report, 1 if sunk report with ship length
extern aidata ai_data;
extern player human;
extern player computer;

#endif