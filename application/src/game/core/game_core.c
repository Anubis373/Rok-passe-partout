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
    self->player = playerInit();
    self->CleCollected = 0;
    return(self);
}

Player* playerInit()
{
    Player* cube = (Player*)calloc(1, sizeof(Player));
    AssertNew(cube)
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
    board[0][3] = PILLAR;
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
    soltion(self->player, self);
}

bool tryMove(int direction, GameCore* self)
{
    switch (direction)
    {
    case HAUT:
        if (self->m_playerPosition.x == 0) return false;
        if (self->board[(int)self->m_playerPosition.x - 1][(int)self->m_playerPosition.y] == CRATE) return false;
        if (self->board[(int)self->m_playerPosition.x - 1][(int)self->m_playerPosition.y] == PILLAR) return false;
        if (self->board[(int)self->m_playerPosition.x - 1][(int)self->m_playerPosition.y] == MONSTER) return false;
        if (self->board[(int)self->m_playerPosition.x - 1][(int)self->m_playerPosition.y] == CRYSTAL)
        {
            if (self->player->facePorte != CREUX_CLE) return false;
            if (self->CleCollected == true) return false;
        }
        return true;
    case BAS:
        if (self->m_playerPosition.x == GAME_GRID_SIZE_Y-1) return false;
        if (self->board[(int)self->m_playerPosition.x + 1][(int)self->m_playerPosition.y] == CRATE) return false;
        if (self->board[(int)self->m_playerPosition.x + 1][(int)self->m_playerPosition.y] == PILLAR) return false;
        if (self->board[(int)self->m_playerPosition.x + 1][(int)self->m_playerPosition.y] == MONSTER) return false;
        return true;
    case DROITE:
        if (self->m_playerPosition.y == 0) return false;
        if (self->board[(int)self->m_playerPosition.x][(int)self->m_playerPosition.y - 1] == CRATE) return false;
        if (self->board[(int)self->m_playerPosition.x][(int)self->m_playerPosition.y - 1] == PILLAR) return false;
        if (self->board[(int)self->m_playerPosition.x][(int)self->m_playerPosition.y - 1] == MONSTER) return false;
        return true;
    case GAUCHE:
        if (self->m_playerPosition.y == GAME_GRID_SIZE_X - 1) return false;
        if (self->board[(int)self->m_playerPosition.x][(int)self->m_playerPosition.y + 1] == CRATE) return false;
        if (self->board[(int)self->m_playerPosition.x][(int)self->m_playerPosition.y + 1] == PILLAR) return false;
        if (self->board[(int)self->m_playerPosition.x][(int)self->m_playerPosition.y + 1] == MONSTER) return false;

        return true;
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

bool soltion(Player* self, GameCore* core)
{
    if (core->m_playerPosition.x != 0 || core->m_playerPosition.y != 2)
        return false;
    if (self->facePorte != VISAGE || self->faceTerre != CREUX_CLE)
        return false;
    if (!core->CleCollected)
        return false;
    printf("GAGNE, WOHOOOO");
    return true;
    
}
