//
//  ChessEngine.h
//  Scacchi7
//
//  Created by Tina Raissi on 19/03/14.
//  Copyright (c) 2014 Tina Raissi. All rights reserved.
//

#ifndef Scacchi7_ChessEngine_h
#define Scacchi7_ChessEngine_h


#import <stdbool.h>

#define PieceColor int
#define ERROR -1

//Index Types
#define ORIGINE 0
#define DESTINATION 1

// PC Piece Content defs
#define PC_EMPTY 0
#define PC_PAWN_WHITE 1
#define PC_PAWN_BLACK 2
#define PC_QUEEN_WHITE 3
#define PC_QUEEN_BLACK 4

// PT Player Turn defs
#define PT_WHITE 0
#define PT_BLACK 1


// MT Move Type defs
#define MT_NORMAL 1
#define MT_CAPTURE 2
#define MT_PROMOTION 3
#define MT_ILLEGAL 4
#define MT_CAPTURE_AND_PROMOTION 5

// VC Victory Check
#define VC_NO_VICTORY 0
#define VC_VICTORY 1
#define VC_DRAW 2



void initialize();
PieceColor getSquareContent (int x, int y);
PieceColor getCurrentPlayer();
PieceColor getOppositePlayer();

bool isMoveLegal (int originX, int originY, int destX, int destY);
int evaluateMove (int originX, int originY, int destX, int destY);
void executeMove (int originX, int originY, int destX, int destY);
void movePiece (int originX, int originY, int destX, int destY);
void capturePiece (int originX, int originY, int destX, int destY);
void promotePiece (int destX, int destY);
int getPiecesCount(int playerColor);
int checkForVictory();
void setColor();

int transf_rel2abs_X (int x_rel, int color);
int transf_rel2abs_Y (int y_rel, int color);
int transf_abs2array (int x_abs, int y_abs);
int transf_rel2array (int x_rel, int y_rel, int color);
void transf_index2rel (int index, int* x_rel, int* y_rel);
int transf_relCoordinate2abs (int y);

#endif