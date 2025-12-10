/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"

#define GAME_GRID_SIZE_X 5
#define GAME_GRID_SIZE_Y 4

typedef struct Player
{
    int facePorte;
    int faceCiel;
    int faceOppPorte;
    int faceTerre;
    int faceGauchePorte;
    int faceDroitePorte;
}Player;

typedef struct GameCore
{
    int** board;
    Vec2 m_playerPosition;
    Player* player;
    bool CleCollected;
    bool AxeCollected;
    bool crystalUnder;
} GameCore;

typedef struct GameHashmapEntry
{
    GameCore prevState;
    GameCore currState;
} GameHashmapEntry;

typedef struct GameHashmap
{
    GameHashmapEntry* m_entries;
    size_t* m_idMap;
    size_t m_capacity;
    size_t m_size;
} GameHashmap;


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

enum Direction
{
    HAUT = 0,
    BAS = 1,
    GAUCHE = 2,
    DROITE = 3,
};


GameCore* GameCore_init();

Player* playerInit();

int** Board_create();

void Grid_Render(int** grid);

void MovePlayer(int direction, GameCore* self);

void rotationDeplacement(Player* self, int direction);

void rotationBouclier(Player* self, GameCore* core);

bool rotationBouclierIsValid(Player* self, GameCore* core);

bool tryMove(int direction, GameCore* self);

bool soltion(Player* self, GameCore* core);

bool gameCore_equals(GameCore* plateau1, GameCore* plateau2);

bool gameCore_hashContains(GameHashmap* map, GameCore* state);

void gameCore_hashInsert(GameHashmap* map, GameCore* state);

void gameCore_resolution();
