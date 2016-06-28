//
//  ChessEngineTest.c
//  pawns
//
//  Created by Tina Raissi on 25/01/15.
//  Copyright (c) 2015 Gator Bytes. All rights reserved.
//

#include "ChessEngineTest.h"
#include "ChessEngine.h"
#include <stdio.h>

void main() {
    for (int i = 1; i < 9; i++) {
        initialize();
        executeMove(i, 1, i, 2);
        int sc = getSquareContent(i, 2);
        printf("%i %i",i, sc);
    }
}
