/*
This file contains mainly bool functions used in other files.
*/

#include "header.h"

bool game(bool does_human_start){
    new_turn();
    bool game_continues = true;
    if(does_human_start) game_continues = move_human();
    if(game_continues && difficulty == 0) game_continues = move_computer_0();
    else if(game_continues && difficulty == 1) game_continues = move_computer_1();
    else game_continues = false;
    board_print(true, false);
    if(!does_human_start && game_continues) game_continues = move_human();
    return game_continues;
}

void board_start(){
    for(short i = 0; i<BOARD_SIZE; i++)
        for(short j = 0; j<BOARD_SIZE; j++)
            computer.board[i][j] = human.board[i][j] = EMPTY_SIGN;
}

bool coordinates_correct(char letter, char number){
    if(letter >= 0 && letter < BOARD_SIZE && number >= 0 && number < BOARD_SIZE)
        return true;
    return false;
}

bool board_place_ship(player* pla, short length, char letter1, char number1, char letter2, char number2, bool first_ship_3){
    char ship[length][2];
    if(letter1 - letter2 == length-1 && number1 == number2)
        for(short i = 0; letter2<=letter1; letter2++, i++){
            if(pla->board[letter2][number1] == EMPTY_SIGN)
                ship[i][0] = letter2, ship[i][1] = number1;
            else return false;
        }
    else if(letter2 - letter1 == length-1 && number1 == number2)
        for(short i = 0; letter1<=letter2; letter1++, i++){
            if(pla->board[letter1][number1] == EMPTY_SIGN)
                ship[i][0] = letter1, ship[i][1] = number1;
            else return false;
        }
    else if(number1 - number2 == length-1 && letter1 == letter2)
        for(short i = 0; number2<=number1; number2++, i++){
            if(pla->board[letter2][number2] == EMPTY_SIGN)
                ship[i][0] = letter2, ship[i][1] = number2;
            else return false;
        }
    else if(number2 - number1 == length-1 && letter1 == letter2)
        for(short i = 0; number1<=number2; number1++, i++){
            if(pla->board[letter2][number1] == EMPTY_SIGN)
                ship[i][0] = letter2, ship[i][1] = number1;
            else return false;
        }
    else return false;
    short ships_index;
    if(length == 5) ships_index = 0;
    else if(length == 4) ships_index = 1;
    else if(length == 3 && first_ship_3) ships_index = 2;
    else if(length == 3 && !first_ship_3) ships_index = 3;
    else if(length == 2) ships_index = 4;
    for(short i = 0; i<length; i++){
        pla->ships[ships_index][i][0] = ship[i][0];
        pla->ships[ships_index][i][1] = ship[i][1];
        pla->ships[ships_index][i][2] = 0;
        pla->board[ship[i][0]][ship[i][1]] = pla->is_human ? HUMAN_SHIP_SIGN : COMPUTER_SHIP_SIGN;
    }
    return true;            
}

bool can_be_shoted(player* pla, char letter, char number){
    if(pla->board[letter][number] == EMPTY_SIGN || pla->board[letter][number] == HUMAN_SHIP_SIGN || pla->board[letter][number] == COMPUTER_SHIP_SIGN)
        return true;
    return false;
}

bool ship_is_destroyed(player* pla, short index, short size){
    for(short i = 0; i<size; i++)
        if(pla->ships[index][i][2] != 1)
            return false;
    for(short i = 0; i<size; i++)
        pla->ships[index][i][2] = 2;
    pla->ships_lost++;
    return true;
}

bool shot(player* pla, char letter, char number){
    if(pla->board[letter][number] == HUMAN_SHIP_SIGN || pla->board[letter][number] == COMPUTER_SHIP_SIGN){
        pla->board[letter][number] = HIT_SIGN;
        for(short i = 0, ships_size = 5; i<5; i++, ships_size--){
            if(i == 3) ships_size = 3;
            for(short j = 0; j<ships_size; j++){
                if(pla->ships[i][j][0] == letter && pla->ships[i][j][1] == number){
                    pla->ships[i][j][2] = 1;
                    if(pla->is_human){
                        ai_data.hitmode = true;
                        ai_data.fields_hit_coordinates[ai_data.fields_hit_in_hitmode][0] = letter;
                        ai_data.fields_hit_coordinates[ai_data.fields_hit_in_hitmode][1] = number;
                        ai_data.fields_hit_in_hitmode++;
                        ai_data.probability_board[letter][number] = -2;
                    }
                    if(ship_is_destroyed(pla, i, ships_size)){
                        if(sunk){
                            print_shot_result(2, ships_size, !pla->is_human);
                            if(pla->is_human){
                                ai_data.ships_alive[i] = 0;
                                ai_data.fields_sunk_in_hitmode += ships_size;
                            }
                        }
                        else print_shot_result(1, ships_size, !pla->is_human);
                        if(pla->ships_lost == 5) return false;
                    }
                    else{
                        print_shot_result(1, ships_size, !pla->is_human);
                        if(!pla->is_human);
                    }
                    return true;
                }
            }
        }
            
    }
    else{
        pla->board[letter][number] = MISS_SIGN;
        print_shot_result(0, 0, !pla->is_human);
    }
    return true;
}
