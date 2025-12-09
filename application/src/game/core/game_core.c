/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game_core.h"

GameCore* GameCore_init()
{
    GameCore* self = (GameCore*)calloc(1, sizeof(GameCore));
    AssertNew(self);
    self->board = Board_create();
    return(self);
    self->m_playerPosition = Vec2_set(2, 1);
}

int** Board_create()
{
    int** board = calloc(GAME_GRID_SIZE_Y, sizeof(int*));
    for (int i = 0; i < GAME_GRID_SIZE_Y; i++)
    {
        board[i] = calloc(GAME_GRID_SIZE_X, sizeof(int));
    }
    board[1][1] = CRATE;
    board[2][1] = PLAYER;
    board[0][2] = 12;
    return(board);
}


void Grid_Render(int** board)
{
    for (int i = 0; i < GAME_GRID_SIZE_Y; i++)
    {
        for (int j = 0; j < GAME_GRID_SIZE_X; j++)
        {
            printf("[%d]  ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}


void MovePlayer(int direction, GameCore* self)
{
    Vec2 PlayerPos = self->m_playerPosition;
    if (direction == HAUT)
    {
        if (self->m_playerPosition.y == 0) return;
        self->board[(int)(PlayerPos.y)][(int)(PlayerPos.x)] = VOID;
        self->board[(int)(PlayerPos.y)+1][(int)(PlayerPos.x)] = PLAYER;
    }
    self->board[(int)(PlayerPos.y)][(int)(PlayerPos.x)];
}
