/*
This file contains all functions connected to computer behaviour. Some of the functions declared here are local to this file.
*/

#include "header.h"

void clear_probability_board(bool);                         //clears ai_data.probability_board, omits -1, unless bool = true
void find_max_probability_board(char*, char*);              //finds max value in ai_data.probability_board and modifies arguments to indexes of it
void fill_probability_board(bool);                          //fils ai_data.probability_board with probability data, max is the most probable,
                                                            //if bool is true (hitmode) only counts ships that are parts of targets already hit
void place_probability_board(char, char, short, bool, bool);//places probable ship position (+1 on every field it occupies), chars for coordinates, short for length,
                                                            //first bool true if horizontal(in right direction), false if vertical(in down direction),
                                                            //second bool if hitmode is active, then considers only ships that colide with already hit fields
bool is_hitmode();                                          //checks, if hitmode can still last, false when it has to end
void hitmode_reset();                                       //resets following ai_data variables: fields_hit_in_hitmode, fields_sunk_in_hitmode,
                                                            //fields_hit_coordinates, ai_data.hitmode

void board_init_computer(){
    for(short i = 0, ships_size = 5; i<5; i++, ships_size--){
        if(i == 3) ships_size = 3;
        ai_data.ships_alive[i] = ships_size;
    }//fills ai_data.ships_alive with correct ship's length
    clear_probability_board(true);
    hitmode_reset();
    char coordinates[4];
    bool ship_misplaced = true;
    bool first_ship_3 = true;
    computer.is_human = false;
    computer.ships_lost = 0;
    for(short i = 5; i>1; i--){
        if(i == 2 && first_ship_3){
            i++;
            first_ship_3 = false;
        }
        do{
            coordinates[0] = rand()%10, coordinates[1] = rand()%10;
            if(rand()%2){
                coordinates[3] = coordinates[1];
                if(rand()%2) coordinates[2] = coordinates[0] + i-1;
                else coordinates[2] = coordinates[0] - i-1;
            }
            else{
                coordinates[2] = coordinates[0];
                if(rand()%2) coordinates[3] = coordinates[1] + i-1;
                else coordinates[3] = coordinates[1] - i-1;
            }
            if(coordinates_correct(coordinates[0], coordinates[1]) && coordinates_correct(coordinates[2], coordinates[3]))
                if(board_place_ship(&computer, i, coordinates[0], coordinates[1], coordinates[2], coordinates[3], first_ship_3))
                    ship_misplaced = false;
        }while(ship_misplaced);
        ship_misplaced = true;
    }
}

bool move_computer_0(){//difficulty 0
    char coordinates[2];
    if(ai_data.hitmode){
        if(is_hitmode()){
            fill_probability_board(true);
            find_max_probability_board(&coordinates[0], &coordinates[1]);
            ai_data.probability_board[coordinates[0]][coordinates[1]] = -1;
        }
        else
            hitmode_reset();
    }
    if(!ai_data.hitmode) do{
        coordinates[0] = rand()%10;
        coordinates[1] = rand()%10;
    }while(!can_be_shoted(&human, coordinates[0], coordinates[1]));
    return shot(&human, coordinates[0], coordinates[1]);
}

bool move_computer_1(){//difficulty 1
    char coordinates[2];
    if(ai_data.hitmode){
        if(is_hitmode())
            fill_probability_board(true);
        else
            hitmode_reset();
    }
    if(!ai_data.hitmode) fill_probability_board(false);//not else, because previous 'if' can change ai_data.hitmode value
    find_max_probability_board(&coordinates[0], &coordinates[1]);
    ai_data.probability_board[coordinates[0]][coordinates[1]] = -1;
    return shot(&human, coordinates[0], coordinates[1]);
}

void clear_probability_board(bool first){
    for(short i = 0; i<BOARD_SIZE; i++)
        for(short j = 0; j<BOARD_SIZE; j++)
            if(ai_data.probability_board[i][j] >= 0 || first)
                ai_data.probability_board[i][j] = 0;
}

void place_probability_board(char letter, char number, short ship_size, bool horizontal, bool hit_mode){
    bool hit_mode_colide = false;
    if(horizontal){
        for(short i = 0; i<ship_size; i++){
            if(ai_data.probability_board[letter][number+i] == -1) return;
            if(hit_mode)
                if(ai_data.probability_board[letter][number+i] == -2) hit_mode_colide = true;
        }
        if(hit_mode && !hit_mode_colide) return;
        for(short i = 0; i<ship_size; i++)
            if(ai_data.probability_board[letter][number+i] != -2)
                ai_data.probability_board[letter][number+i]++;
    }
    else{
        for(short i = 0; i<ship_size; i++){
            if(ai_data.probability_board[letter+i][number] == -1) return;
            if(hit_mode)
                if(ai_data.probability_board[letter+i][number] == -2) hit_mode_colide = true;
        }
        if(hit_mode && !hit_mode_colide) return;
        for(short i = 0; i<ship_size; i++)
            if(ai_data.probability_board[letter+i][number] != -2)
                ai_data.probability_board[letter+i][number]++;
    }
}

void fill_probability_board(bool hit_mode){
    clear_probability_board(false);
    for(short i = 0, ships_size = 5; i<5; i++, ships_size--){//makes probability board
        if(i == 3) ships_size = 3;
        if(ai_data.ships_alive[i]){//if ship is destroyed, it is false (= 0)
            short board_size_mod = BOARD_SIZE - ai_data.ships_alive[i];
            for(short j = 0; j<BOARD_SIZE; j++)//horizontal placement
                for(short k = 0; k<=board_size_mod; k++)
                    place_probability_board(j, k, ships_size, true, hit_mode);
            for(short j = 0; j<=board_size_mod; j++)//vertical placement
                for(short k = 0; k<BOARD_SIZE; k++)
                    place_probability_board(j, k, ships_size, false, hit_mode);
        }
    }
}

void find_max_probability_board(char* letter, char* number){
    *letter = *number = 0;
    short number_of_max = 0;
    for(short i = 0; i<BOARD_SIZE; i++)
        for(short j = 0; j<BOARD_SIZE; j++){
            if(ai_data.probability_board[i][j] > ai_data.probability_board[*letter][*number]){
                *letter = i, *number = j;
                number_of_max = 1;
                }
            if(ai_data.probability_board[i][j] == ai_data.probability_board[*letter][*number]){
                number_of_max++;
                if(!(rand()%number_of_max))//choses random max number from max numbers with equal probability
                    *letter = i, *number = j;
            }
                
        }
}

bool is_hitmode(){
    if(sunk){
        if(ai_data.fields_hit_in_hitmode == ai_data.fields_sunk_in_hitmode) return false;
        else return true;
    }
    else{
        for(short i = 0; i<ai_data.fields_hit_in_hitmode; i++){
            if(coordinates_correct(ai_data.fields_hit_coordinates[i][0] + 1, ai_data.fields_hit_coordinates[i][1] + 1))
                if(can_be_shoted(&human, ai_data.fields_hit_coordinates[i][0] + 1, ai_data.fields_hit_coordinates[i][1] + 1)) return true;
            if(coordinates_correct(ai_data.fields_hit_coordinates[i][0] + 1, ai_data.fields_hit_coordinates[i][1] - 1))
                if(can_be_shoted(&human, ai_data.fields_hit_coordinates[i][0] + 1, ai_data.fields_hit_coordinates[i][1] - 1)) return true;
            if(coordinates_correct(ai_data.fields_hit_coordinates[i][0] - 1, ai_data.fields_hit_coordinates[i][1] + 1))
                if(can_be_shoted(&human, ai_data.fields_hit_coordinates[i][0] - 1, ai_data.fields_hit_coordinates[i][1] + 1)) return true;
            if(coordinates_correct(ai_data.fields_hit_coordinates[i][0] - 1, ai_data.fields_hit_coordinates[i][1] - 1))
                if(can_be_shoted(&human, ai_data.fields_hit_coordinates[i][0] - 1, ai_data.fields_hit_coordinates[i][1] - 1)) return true;
        }
    }
    return false;
}

void hitmode_reset(){
    ai_data.hitmode = false;
    ai_data.fields_hit_in_hitmode = 0;
    ai_data.fields_sunk_in_hitmode = 0;
    for(short i = 0; i<17; i++)
        ai_data.fields_hit_coordinates[i][0] = ai_data.fields_hit_coordinates[i][1] = 0;
    for(short i = 0; i<BOARD_SIZE; i++)//replaces -2 probability mode (hit in current hitmode) to -1 (cannot be shot)
        for(short j = 0; j<BOARD_SIZE; j++)
            if(ai_data.probability_board[i][j] == -2)
                ai_data.probability_board[i][j] = -1;
}