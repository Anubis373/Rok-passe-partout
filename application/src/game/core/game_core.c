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
    if (direction == HAUT)
    {
        if (self->m_playerPosition.y == 0) return;
        self->board[(int)(PlayerPos.y)][(int)(PlayerPos.x)] = VOID;
    }
}

void rotationDeplacement(Player* self, int direction)
{
    int tmp;
    switch (direction)
    {
    case HAUT:
        tmp = self->facePorte;
        self->facePorte = self->faceCiel;
        self->faceCiel = self->faceOppPorte;
        self->faceOppPorte = self->faceTerre;
        self->faceTerre = tmp;
        break;
    case BAS:
        tmp = self->facePorte;
        self->facePorte = self->faceTerre;
        self->faceTerre = self->faceOppPorte;
        self->faceOppPorte = self->faceCiel;
        self->faceCiel = tmp;
        break;
    case DROITE:
        tmp = self->faceDroitePorte;
        self->faceDroitePorte = self->faceCiel;
        self->faceCiel = self->faceGauchePorte;
        self->faceGauchePorte = self->faceTerre;
        self->faceTerre = tmp;
        break;
    case GAUCHE:
        tmp = self->faceGauchePorte;
        self->faceGauchePorte = self->faceCiel;
        self->faceCiel = self->faceDroitePorte;
        self->faceDroitePorte = self->faceTerre;
        self->faceTerre = tmp;
        break;
    default:
        break;
    }
}


bool rotationBouclierIsValid(Player* self, GameCore *core)
{
    if (self->faceTerre != BOUCLIER)
        return false;

    Vec2 pos = core->m_playerPosition;
    int x = pos.x;
    int y = pos.y;

    if (y > 0 && core->board[y - 1][x] == CRATE)
        return false;

    if (y < GAME_GRID_SIZE_Y - 1 && core->board[y + 1][x] == CRATE)
        return false;

    if (x > 0 && core->board[y][x - 1] == CRATE)
        return false;

    if (x < GAME_GRID_SIZE_X - 1 && core->board[y][x + 1] == CRATE)
        return false;

    return true;
}

void rotationBouclier(Player* self, GameCore* core)
{
    if (rotationBouclierIsValid(self, core))
    {
        int tmp;
        tmp = self->facePorte;
        self->facePorte = self->faceGauchePorte;
        self->faceGauchePorte = self->faceOppPorte;
        self->faceOppPorte = self->faceDroitePorte;
        self->faceDroitePorte = tmp;
    }
}
