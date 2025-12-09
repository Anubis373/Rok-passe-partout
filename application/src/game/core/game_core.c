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
    self->m_playerPosition = Vec2_set(2, 1);

    return(self);
}

Player* playerInit()
{
    Player* cube = calloc(1, sizeof(Player));
    cube->facePorte = VISAGE;
    cube->faceCiel = TETE;
    cube->faceOppPorte = DOS;
    cube->faceTerre = CREUX_CLE;
    cube->faceGauchePorte = CREUX_HACHE;
    cube->faceDroitePorte = BOUCLIER;

    return cube;
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
    if (!tryMove(direction, self)) return;
    if (direction == HAUT)
    {
        self->board[(int)(PlayerPos.x)][(int)(PlayerPos.y)] = VOID;
        self->board[(int)(PlayerPos.x)-1][(int)(PlayerPos.y)] = PLAYER;
        self->m_playerPosition = Vec2_add(PlayerPos, Vec2_left);
    }
    if (direction == BAS)
    {
        self->board[(int)(PlayerPos.x)][(int)(PlayerPos.y)] = VOID;
        self->board[(int)(PlayerPos.x) + 1][(int)(PlayerPos.y)] = PLAYER;
        self->m_playerPosition = Vec2_add(PlayerPos, Vec2_right);
    }
    if (direction == GAUCHE)
    {
        self->board[(int)(PlayerPos.x)][(int)(PlayerPos.y)] = VOID;
        self->board[(int)(PlayerPos.x)][(int)(PlayerPos.y)+1] = PLAYER;
        self->m_playerPosition = Vec2_add(PlayerPos, Vec2_up);
    }
    if (direction == DROITE)
    {
        self->board[(int)(PlayerPos.x)][(int)(PlayerPos.y)] = VOID;
        self->board[(int)(PlayerPos.x)][(int)(PlayerPos.y)-1] = PLAYER;
        self->m_playerPosition = Vec2_add(PlayerPos, Vec2_down);
    }
    Grid_Render(self->board);
    printf("%f %f", self->m_playerPosition.x, self->m_playerPosition.y);
}

bool tryMove(int direction, GameCore* self)
{
    switch (direction)
    {
    case HAUT:
        if (self->m_playerPosition.x == 0) return false;
        return true;
    case BAS:
        if (self->m_playerPosition.x == GAME_GRID_SIZE_Y-1) return false;
        return true;
    case DROITE:
        if (self->m_playerPosition.y == 0) return false;
        return true;
    case GAUCHE:
        if (self->m_playerPosition.y == GAME_GRID_SIZE_X - 1) return false;
        return true;
    }
}
