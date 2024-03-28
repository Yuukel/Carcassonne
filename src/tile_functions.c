#include "tile_functions.h"

int WaitingForAction(){
    char ch;
    ch = getch();
    if(ch == ' ') return 1;

    return 0;
}

int MoveCamera(){
    // if(ch == KEY_UP && coordYMin > -71) coordYMin--;
    // if(ch == KEY_DOWN && coordYMin < 61) coordYMin++;
    // if(ch == KEY_LEFT && coordXMin > -71) coordXMin--;
    // if(ch == KEY_RIGHT && coordXMin < 61) coordXMin++;
    return 0;
}

TileStruct RotateTile(TileStruct t, int sens){
    char temp; 
    if(sens > 0){
        temp = t.cotes[3];
        t.cotes[3] = t.cotes[2];
        t.cotes[2] = t.cotes[1];
        t.cotes[1] = t.cotes[0];
        t.cotes[0] = temp;
    }
    else if(sens < 0){
        temp = t.cotes[0];
        t.cotes[0] = t.cotes[1];
        t.cotes[1] = t.cotes[2];
        t.cotes[2] = t.cotes[3];
        t.cotes[3] = temp;
    }

    return t;
}

// Pose Tuile