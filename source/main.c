/*
This file contains int main() function and all functions connected to input-output operations.
*/
#include "header.h"

short difficulty = STARTING_DIFFICULTY;
short sunk = STARTING_SUNK;
aidata ai_data;
player human;
player computer;

int main(){
    srand(time(NULL));
    start();//starting menu
    board_start();//fills boards of players in EMPTY_SIGN
    board_init_human();//human places their ships on the board
    board_init_computer();//computer places their ships
    bool does_human_start = rand()%2;
    board_print(true, false);
    if(does_human_start) puts("YOU MOVE FIRST");
    else puts("COMPUTER BEGINS");
    while(game(does_human_start));
    for(short i = 0; i<BOARD_SIZE*3; i++)//prints game over
        putchar('-');
    if(human.ships_lost==5) printf(" COMPUTER WINS ");
    else printf("  HUMAN WINS  -");
    for(short i = 0; i<BOARD_SIZE*3; i++)
        putchar('-');
    putchar('\n');
    board_print(true, true);
    return 0;
}

void start(){
    char input;
    printf("Welcome to Battleship!\n");
    printf("Type 'v' if you want to view current game settings;\n");
    printf("Type 's' if you want to change game settings;\n");
    printf("Type 'p' if you want to start playing.\n");
    printf("Remember, that you can exit game at any time with Cltr + C\n");
    do{
        scanf(" %c", &input);
        switch(input){
            case 's':
            printf("Type desired difficulty (0 - easy, 1 - hard):");
            scanf(" %hd", &difficulty);
            printf("Do players receive sunk report (0 - no, 1 - yes with ship length):");
            scanf(" %hd", &sunk);
            case 'v':
            printf("difficulty: %hd, sunk report: %hd, type p if you want to start playing.\n", difficulty, sunk);
            break;
            case 'p':
            return;
        }
    }while(1);
}

void board_init_human(){
    char coordinates[10];
    bool ship_misplaced = true;
    printf("You can place ships of following lengths: 5, 4, 3, 3, 2\n");
    bool first_ship_3 = true;
    human.is_human = true;
    human.ships_lost = 0;
    for(short i = 5; i>1; i--){
        if(i == 2 && first_ship_3){
            i++;
            first_ship_3 = false;
        }
        board_print(false, false);
        do{
            printf("Place ship of the length of %d in the following format: A0A%d\n", i, i-1);
            scanf("%s", &coordinates);
            if(coordinates_correct(coordinates[0] - 'A', coordinates[1] - '0') && coordinates_correct(coordinates[2] - 'A', coordinates[3] - '0'))
                if(board_place_ship(&human, i, coordinates[0] - 'A', coordinates[1] - '0', coordinates[2] - 'A', coordinates[3] - '0', first_ship_3))
                    ship_misplaced = false;
        }while(ship_misplaced);
        ship_misplaced = true;
    }
}

void board_print(bool both, bool end_board){
    printf("\nHUMAN:");
    if(both){
        for(short i = 0; i<3*BOARD_SIZE+5; i++)
            putchar(' ');
        printf("COMPUTER:");
    }                                           //Human: Computer: printed
    printf("\n  ");
    for(short i = 0; i<BOARD_SIZE; i++)
        printf(" %c ", i+'0');
    if(both){
        printf("           ");//11*SPACE
        for(short i = 0; i<BOARD_SIZE; i++)
            printf(" %c ", i+'0');
    }
    printf("\n ");                              //first line printed
    for(short i = 0; i<3*BOARD_SIZE+2; i++)
        putchar(BORDER_SIGN);
    if(both){
        printf("         ");//9*SPACE
        for(short i = 0; i<3*BOARD_SIZE+2; i++)
            putchar(BORDER_SIGN);
    }
    putchar('\n');                              //second line printed
    for(short i = 0; i<BOARD_SIZE; i++){
        putchar(i+'A'), putchar(BORDER_SIGN);
        for(short j = 0; j<BOARD_SIZE; j++)
            printf(" %c ", human.board[i][j]);
        if(both){
            printf("%c%c       %c%c", BORDER_SIGN, i+'A', i+'A', BORDER_SIGN);//7*SPACE
            for(short j = 0; j<BOARD_SIZE; j++){
                if(end_board || (computer.board[i][j] != COMPUTER_SHIP_SIGN)) printf(" %c ", computer.board[i][j]);
                else printf(" %c ", EMPTY_SIGN);//we dont want human to see enemy ship
            }
        }
        printf("%c%c\n", BORDER_SIGN, i+'A');   //next line printed
    }
    putchar(' ');
    for(short i = 0; i<3*BOARD_SIZE+2; i++)
        putchar(BORDER_SIGN);
    if(both){
        printf("         ");//9*SPACE
        for(short i = 0; i<3*BOARD_SIZE+2; i++)
            putchar(BORDER_SIGN);
    }
    putchar('\n');                              //second-to-last line printed
    printf("  ");
    for(short i = 0; i<BOARD_SIZE; i++)
        printf(" %c ", i+'0');
    if(both){
        printf("           ");//11*SPACE
        for(short i = 0; i<BOARD_SIZE; i++)
            printf(" %c ", i+'0');
    }
    putchar('\n');                              //last line printed
    putchar('\n');
}

bool move_human(){
    char coordinates[10];
    do{
        puts("Where would you like to shoot?");
        scanf("%s", &coordinates);
    }while(!coordinates_correct(coordinates[0]-'A', coordinates[1]-'0') || !can_be_shoted(&computer, coordinates[0]-'A', coordinates[1]-'0'));
    return shot(&computer, coordinates[0]-'A', coordinates[1]-'0');
}

void print_shot_result(short i, short length, bool is_human){
    if(is_human) printf("COMPUTER: ");
    else printf("HUMAN: ");
    if(i == 2){
        printf("SHIP OF LENGTH %hd SUNK!\n", length);
        putchar(7);//sound
    }
    else if(i == 1) printf("HIT!\n");
    else printf("MISS!\n");
}

void new_turn(){
    static short turn = 0;
    turn++;
    for(short i = 0; i<BOARD_SIZE*3; i++)
        putchar('-');
    printf(" TURN NUMBER %hd ", turn);
    for(short i = 0; i<BOARD_SIZE*3; i++)
        putchar('-');
    putchar('\n');
}