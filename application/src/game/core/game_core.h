/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"

#define GAME_GRID_SIZE_X 5
#define GAME_GRID_SIZE_Y 4

typedef struct GameCore
{
    int** board;
} GameCore;

enum GridElement
{
    VOID = 0,
    CRATE = 1,
    PLAYER = 2,
    PILLAR = 3,
    CRYSTAL = 4,
    KEY = 5,
    AXE = 6,
    MONSTER = 7,
};

enum PlayerFace
{
    VISAGE = 1,
    BOUCLIER = 2,
    CREUX_HACHE = 3,
    CREUX_CLE = 4,
    DOS = 5,
    TETE = 6,
};


typedef struct Player
{
    int face1;
    int face2;
    int face3;
    int face4;
    int face5;
    int face6;
};

GameCore* GameCore_init();

int** Board_create();

void Grid_Render(int* grid);
