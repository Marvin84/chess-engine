//
//  ChessEngine.c
//  Scacchi7
//
//  Created by Tina Raissi on 19/03/14.
//  Copyright (c) 2014 Tina Raissi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "ChessEngine.h"

char *board;
int capturedPieces[] = {0,0};
int queensCounter[] = {0,0};

int currentColor;


//Initialize the board and the color
void initialize(){
    
    int x_rel, y_rel;
    board = (char*)malloc(sizeof(char)*64);
    
    currentColor = PT_WHITE;
    
    for(x_rel = 1; x_rel < 9; x_rel++){
        board[transf_rel2array(x_rel, 1, PT_BLACK)] = PC_PAWN_BLACK;
    }
    for(x_rel = 1; x_rel < 9; x_rel++){
        board[transf_rel2array(x_rel, 1, PT_WHITE)] = PC_PAWN_WHITE;
    }
    for(x_rel = 1; x_rel < 9; x_rel++){
        for(y_rel = 2; y_rel < 7; y_rel++){
            board[transf_abs2array(x_rel, y_rel)] = PC_EMPTY;
        }
    }
    return;
}

int getSquareContent (int x, int y) {
    
    int piece, index;
    
    if(y < 1 || y > 8)
        printf("transf_abs2array: parametro non valido: y_abs = %d", y);
    if(x < 1 || x > 8)
        printf("transf_abs2array: parametro non valido: x_abs = %d", x);
    
//    y = transf_relCoordinate2abs(y);
    index = transf_abs2array (x, y);
    
//    index = (x - 1) + ((y - 1) * 8);
    piece = board[index];
    
    return piece;
    
}

int getCurrentPlayer() {
    return currentColor;
}

int getOppositePlayer() {
    return (currentColor == PT_WHITE ? PT_BLACK : PT_WHITE);
}

bool isMoveLegal (int originX, int originY, int destX, int destY) {
    return (evaluateMove(originX, originY, destX, destY) == MT_ILLEGAL ? false : true);
}


int evaluateMove (int originX, int originY, int destX, int destY) {
    
    int diffX, diffY, origContent, destContent;
    
    diffX = abs(destX - originX);
    diffY = (destY - originY);
    
    origContent = getSquareContent(originX, originY);
    destContent = getSquareContent(destX, destY);
    
    //Control if the piece has the current color
    if (origContent != getCurrentPlayer()+1){
        printf("LIB: You are chosing the piece of opposite color!\n");
        return MT_ILLEGAL;
    }
    
    // General control on pawn move valid for every kind of move
    if (diffY < 1 || diffX > 1) return MT_ILLEGAL;
    if (originY == 8 && diffX > 0) return MT_ILLEGAL;
    if ((originX == 1 && ((destX - originX) < 0))|| (originX == 8 && ((destX - originX) > 0))) return MT_ILLEGAL;
    
    // First move control
    if (originY == 1) {
        if (diffX == 0) {
            if (diffY < 1) return MT_ILLEGAL;
            if (diffY == 1 && getSquareContent(destX, destY) != PC_EMPTY){
                printf("LIB: The square is not empty!\n");
                return MT_ILLEGAL;
            }
            if (diffY == 2 && !( getSquareContent(destX, destY-1) == PC_EMPTY && getSquareContent(destX, destY) == PC_EMPTY)){ printf("LIB: The square is not empty!\n");
                return MT_ILLEGAL;
            }
            if (diffY > 2) return MT_ILLEGAL;
            
            return MT_NORMAL;
        }
        else if (diffX == 1) {
            if (diffY != 1) return MT_ILLEGAL;

            if (getSquareContent(destX, destY) != getOppositePlayer()+1){
                printf("LIB: Trying to capture your own piece!\n");
                return MT_ILLEGAL;
            }
            
            return MT_CAPTURE;
        }
        
        else if (diffX > 1) {
            return MT_ILLEGAL;
        }
        
    }
    
    // Normal move control
    if (diffX == 0 && destY != 8) {
        if (diffY < 1 || diffY > 1) return MT_ILLEGAL;
        if (diffY == 1 && getSquareContent(destX, destY) != PC_EMPTY){
            printf("LIB: The square is not empty!\n");
            return MT_ILLEGAL;
        }
        
        return MT_NORMAL;
    }
    
    // Capture move control
    if (diffX == 1) {
        if (diffY != 1) {
            printf("LIB: Can't capture!");
            return MT_ILLEGAL;
        }
        if (getSquareContent(destX, destY) != getOppositePlayer()+1){
            printf("LIB: Trying to capture your own piece!\n");
            return MT_ILLEGAL;
        }
        
        if (originY < 7) {
            return MT_CAPTURE;
        }
        else if (originY == 7){
            return MT_CAPTURE_AND_PROMOTION;
        }
    }
    // Promotion control
    
    if (diffX == 0 && destY == 8) {
        if (diffY < 1 || diffY > 1) return MT_ILLEGAL;
        if (diffY == 1 && getSquareContent(destX, destY) != PC_EMPTY){
            printf("LIB: The square is not empty!\n");
            return MT_ILLEGAL;
            }
        if (getSquareContent(destX, destY) == PC_EMPTY) return MT_PROMOTION;
    }

    
    return MT_ILLEGAL;
}

void movePiece (int originX, int originY, int destX, int destY) {
    
    if(destY != 8){
        board[transf_abs2array(destX, transf_relCoordinate2abs(destY))] = getSquareContent(originX, originY);
    }

    board[transf_abs2array(originX, transf_relCoordinate2abs(originY))] = PC_EMPTY;
}

void capturePiece (int originX, int originY, int destX, int destY) {
    movePiece(originX, originY, destX, destY);
    capturedPieces[getCurrentPlayer()]++;
}

void promotePiece(int destX, int destY) {
    
    queensCounter[getCurrentPlayer()]++;
    
    if (getCurrentPlayer() == PT_WHITE) {
        board[transf_abs2array(destX, destY)] = PC_QUEEN_WHITE;
    }
    else {
        board[transf_abs2array(destX, transf_relCoordinate2abs(destY))] = PC_QUEEN_BLACK;
    }
    
    
}

void executeMove (int originX, int originY, int destX, int destY) {
    switch (evaluateMove(originX, originY, destX, destY)) {

            
        case MT_NORMAL:
            movePiece(originX, originY, destX, destY);
            setColor();
            break;
        case MT_CAPTURE:
            capturePiece(originX, originY, destX, destY);
            setColor();
            break;
        case MT_PROMOTION:
            promotePiece(destX, destY);
            movePiece(originX, originY, destX, destY);
            setColor();
            break;
        case MT_CAPTURE_AND_PROMOTION:
            promotePiece(destX, destY);
            movePiece(originX, originY, destX, destY);
            setColor();
            break;
        case MT_ILLEGAL:
            break;
        default:
            break;
    }
    
}

int getPiecesCount (int playerColor) {
    return 8 - capturedPieces[getCurrentPlayer()];
}


int checkForVictory () {
    
    if (capturedPieces[getOppositePlayer()] == 8) return VC_VICTORY;
    if (queensCounter[getCurrentPlayer()] > 4) return VC_VICTORY;
    if (queensCounter[getCurrentPlayer()] == queensCounter[getOppositePlayer()] == 4) return VC_DRAW;
    
    return VC_NO_VICTORY;
    
}

//Changes player turn and current color
void setColor() {
    if (currentColor == PT_WHITE){
        currentColor = PT_BLACK;
    }
    else currentColor = PT_WHITE;
}


// Transforms the relative coordination in absolute
int transf_rel2abs_X (int x_rel, int color){
    
    if(x_rel < 1 || x_rel > 8)
        printf("transf_rel2abs_X: parametro non valido: x_rel = %d", x_rel);
    if(color != PT_WHITE && color != PT_BLACK)
        printf("transf_rel2abs_X: parametro non valido: color = %d", color);
    
    if(color == PT_BLACK){
        return 9 - x_rel;
    }
    else{
        return x_rel;
    }
}

// Transforms the y_rel in absolute
int transf_rel2abs_Y (int y_rel, int color){
    
    if(y_rel < 1 || y_rel > 8)
        printf("transf_rel2abs_Y: parametro non valido: y_rel = %d", y_rel);
    if(color != PT_WHITE && color != PT_BLACK)
        printf("transf_rel2abs_Y: parametro non valido: color = %d", color);
    
    if(color == PT_BLACK){
        return 9 - y_rel;
    }
    else{
        return y_rel;
    }
}

//Transforms the absolute coordination in array's index
int transf_abs2array (int x_abs, int y_abs){
    
    if(y_abs < 1 || y_abs > 8)
        printf("transf_abs2array: parametro non valido: y_abs = %d", y_abs);
    if(x_abs < 1 || x_abs > 8)
        printf("transf_abs2array: parametro non valido: x_abs = %d", x_abs);
    
    return (x_abs - 1) + ((y_abs - 1) * 8);
}

//Transforms the relative coordination in array's index
int transf_rel2array (int x_rel, int y_rel, int color){
    return transf_abs2array(transf_rel2abs_X(x_rel, color), transf_rel2abs_Y(y_rel, color));
}

//Transforms the array's index in relative coordination
void transf_index2rel (int index, int* x_rel, int* y_rel){
    *x_rel = (index % 8) + 1;
    if (currentColor == PT_BLACK){
        *y_rel = 8 - (index / 8);}
    else { *y_rel = (index / 8) + 1;}
    
    if(*x_rel < 1 || *x_rel > 8)
        printf("transf_rel2abs_X: parametro non valido: x_rel = %d", *x_rel);
    if(currentColor != PT_WHITE && currentColor != PT_BLACK)
        printf("transf_rel2abs_X: parametro non valido: color = %d", currentColor);
    if(*y_rel < 1 || *y_rel > 8)
        printf("transf_rel2abs_Y: parametro non valido: y_rel = %d", *y_rel);
    if(currentColor != PT_WHITE && currentColor != PT_BLACK)
        printf("transf_rel2abs_Y: parametro non valido: color = %d", currentColor);
    
    
    return;
}

//Transforms the y coordinate of black for the functions that need absolute coordinate
int transf_relCoordinate2abs (int y){
    if (getCurrentPlayer() == PT_BLACK)
        y = 9 - y;
    return y;
    
}



