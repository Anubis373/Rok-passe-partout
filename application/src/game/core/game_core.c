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
    self->AxeCollected = 0;
    self->crystalUnder = 0;
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
    board[0][0] = CRATE;
    board[2][1] = PLAYER;
    board[0][3] = PILLAR;
    board[0][2] = KEY;
    board[2][2] = CRYSTAL;
    board[3][3] = MONSTER;
    board[2][0] = AXE;
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
    if (self->board[(int)PlayerPos.x][(int)PlayerPos.y] != CRYSTAL)
    {
        self->board[(int)(PlayerPos.x)][(int)(PlayerPos.y)] = VOID;
    }
    rotationDeplacement(self->player, direction);
    if (direction == HAUT)
    {
        self->board[(int)(PlayerPos.x)-1][(int)(PlayerPos.y)] = PLAYER;
        self->m_playerPosition = Vec2_add(PlayerPos, Vec2_left);
    }
    if (direction == BAS)
    {
        self->board[(int)(PlayerPos.x) + 1][(int)(PlayerPos.y)] = PLAYER;
        self->m_playerPosition = Vec2_add(PlayerPos, Vec2_right);
    }
    if (direction == GAUCHE)
    {
        self->board[(int)(PlayerPos.x)][(int)(PlayerPos.y)-1] = PLAYER;
        self->m_playerPosition = Vec2_add(PlayerPos, Vec2_down);
    }
    if (direction == DROITE)
    {
        self->board[(int)(PlayerPos.x)][(int)(PlayerPos.y)+1] = PLAYER;
        self->m_playerPosition = Vec2_add(PlayerPos, Vec2_up);
    }
    Grid_Render(self->board);
    printf("%d\n", self->player->facePorte);
    printf("%d %d %d\n", self->player->faceGauchePorte, self->player->faceCiel, self->player->faceDroitePorte);
    printf("%d\n", self->player->faceOppPorte);
    soltion(self->player, self);
}


bool tryMove(int direction, GameCore* self)
{
    if (self->crystalUnder == true)
    {
        self->crystalUnder = false;
        self->board[(int)self->m_playerPosition.x][(int)self->m_playerPosition.y] = CRYSTAL;
    }
    switch (direction)
    {
    case HAUT:
        if (self->m_playerPosition.x == 0) return false;
        switch (self->board[(int)self->m_playerPosition.x - 1][(int)self->m_playerPosition.y])
        {
        case CRATE:
            return false;
        case PILLAR:
            return false;
        case MONSTER:
            return false;
        case CRYSTAL:
            if (self->player->facePorte != CREUX_CLE) return false;
            if (self->CleCollected == true) return false;
            self->crystalUnder = true;
            return true;
        case KEY:
            if (self->player->facePorte != CREUX_CLE) return false;
            self->CleCollected = true;
            return(true);
        case AXE:
            if (self->player->facePorte != CREUX_HACHE) return false;
            self->AxeCollected = true;
            for (int i = 0; i < GAME_GRID_SIZE_Y; i++)
            {
                for (int j = 0; j < GAME_GRID_SIZE_X; j++)
                {
                    if (self->board[i][j] == MONSTER) self->board[i][j] = VOID;
                }
            }
            return true;
        default:
            return true;
        }

        return true;
    case BAS:
        if (self->m_playerPosition.x == GAME_GRID_SIZE_Y - 1) return false;
        switch (self->board[(int)self->m_playerPosition.x + 1][(int)self->m_playerPosition.y])
        {
        case CRATE:
            return false;
        case PILLAR:
            return false;
        case MONSTER:
            return false;
        case CRYSTAL:
            if (self->player->faceOppPorte != CREUX_CLE) return false;
            if (self->CleCollected == true) return false;
            self->crystalUnder = true;
            return true;
        case KEY:
            if (self->player->faceOppPorte != CREUX_CLE) return false;
            self->CleCollected = true;
            return(true);
        case AXE:
            if (self->player->faceOppPorte != CREUX_HACHE) return false;
            self->AxeCollected = true;
            for (int i = 0; i < GAME_GRID_SIZE_Y; i++)
            {
                for (int j = 0; j < GAME_GRID_SIZE_X; j++)
                {
                    if (self->board[i][j] == MONSTER) self->board[i][j] = VOID;
                }
            }
            return true;
        default:
            return true;
        }
    case DROITE:
        if (self->m_playerPosition.y == GAME_GRID_SIZE_X - 1) return false;
        switch (self->board[(int)self->m_playerPosition.x][(int)self->m_playerPosition.y + 1])
        {
        case CRATE:
            return false;
        case PILLAR:
            return false;
        case MONSTER:
            return false;
        case CRYSTAL:
            if (self->player->faceDroitePorte != CREUX_CLE) return false;
            if (self->CleCollected == true) return false;
            self->crystalUnder = true;
            return true;
        case KEY:
            if (self->player->faceDroitePorte != CREUX_CLE) return false;
            self->CleCollected = true;
            return(true);
        case AXE:
            if (self->player->faceDroitePorte != CREUX_HACHE) return false;
            self->AxeCollected = true;
            for (int i = 0; i < GAME_GRID_SIZE_Y; i++)
            {
                for (int j = 0; j < GAME_GRID_SIZE_X; j++)
                {
                    if (self->board[i][j] == MONSTER) self->board[i][j] = VOID;
                }
            }
            return true;
        default:
            return true;
        }
    case GAUCHE:
        if (self->m_playerPosition.y == 0) return false;
        switch (self->board[(int)self->m_playerPosition.x][(int)self->m_playerPosition.y - 1])
        {
        case CRATE:
            return false;
        case PILLAR:
            return false;
        case MONSTER:
            return false;
        case CRYSTAL:
            if (self->player->faceGauchePorte != CREUX_CLE) return false;
            if (self->CleCollected == true) return false;
            self->crystalUnder = true;
            return true;
        case KEY:
            if (self->player->faceGauchePorte != CREUX_CLE) return false;
            self->CleCollected = true;
            return(true);
        case AXE:
            if (self->player->faceGauchePorte != CREUX_HACHE) return false;
            self->AxeCollected = true;
            for (int i = 0; i < GAME_GRID_SIZE_Y; i++)
            {
                for (int j = 0; j < GAME_GRID_SIZE_X; j++)
                {
                    if (self->board[i][j] == MONSTER) self->board[i][j] = VOID;
                }
            }
            return true;
        default:
            return true;
        }
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


bool rotationBouclierIsValid(Player* self, GameCore* core)
{
    if (self->faceTerre != BOUCLIER)
        return false;

    Vec2 pos = core->m_playerPosition;
    int x = (int)pos.y;
    int y = (int)pos.x;

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
    printf("%d\n", self->facePorte);
    printf("%d %d %d\n", self->faceGauchePorte, self->faceCiel, self->faceDroitePorte);
    printf("%d\n", self->faceOppPorte);
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


GameHashmap* gamehashmap_Create(int capacity)
{
    GameHashmap* hashMap = calloc(1, sizeof(GameHashmap));
    assert(capacity > 0);
    assert(hashMap);
    hashMap->m_capacity = capacity;
    hashMap->m_entries = calloc(capacity, sizeof(GameHashmapEntry));
    assert(hashMap->m_entries);
    hashMap->m_size = 0;
    hashMap->m_idMap = calloc(capacity, sizeof(size_t));
    for (size_t i = 0; i < capacity; i++)
    {
        hashMap->m_idMap[i] = (size_t)-1;
    }

    return hashMap;
}

GameHashmapEntry *GameHashmapEntry_Create(GameCore currState, GameCore prevState)
{
    GameHashmapEntry *entrie = calloc(1, sizeof(GameHashmapEntry));

    entrie->currState = currState;
    entrie->prevState = prevState;

    return entrie;
}

uint64_t gameCore_hash(GameCore *self)
{
    uint64_t hash = 0;

    hash ^= (uint64_t)(self->m_playerPosition.x);
    hash = hash * 0xbf58476d1ce4e5b9ULL + 0x9e3779b97f4a7c15ULL;

    hash ^= (uint64_t)(self->m_playerPosition.y);
    hash = hash * 0xbf58476d1ce4e5b9ULL + 0x9e3779b97f4a7c15ULL;

    hash ^= (uint64_t)(self->CleCollected);
    hash = hash * 0xbf58476d1ce4e5b9ULL + 0x9e3779b97f4a7c15ULL;

    hash ^= (uint64_t)(self->AxeCollected);
    hash = hash * 0xbf58476d1ce4e5b9ULL + 0x9e3779b97f4a7c15ULL;

    hash ^= (uint64_t)(self->player->faceTerre);
    hash = hash * 0xbf58476d1ce4e5b9ULL + 0x9e3779b97f4a7c15ULL;

    hash ^= (uint64_t)(self->player->facePorte);
    hash = hash * 0xbf58476d1ce4e5b9ULL + 0x9e3779b97f4a7c15ULL;

    return hash;
}


bool gameCore_equals(GameCore* plateau1, GameCore* plateau2)
{
    if (plateau1->AxeCollected != plateau2->AxeCollected) return false;
    if (plateau1->CleCollected != plateau2->CleCollected) return false;
    if (plateau1->player->facePorte != plateau2->player->facePorte) return false;
    if (plateau1->player->faceTerre != plateau2->player->faceTerre) return false;
    if (plateau1->m_playerPosition.x != plateau2->m_playerPosition.x) return false;
    if (plateau1->m_playerPosition.y != plateau2->m_playerPosition.y) return false;
    return true;
}

bool gameCore_hashContains(GameHashmap* map, GameCore* state)
{
    uint64_t hash = gameCore_hash(state);
    size_t idx = hash % map->m_capacity;

    while (map->m_idMap[idx] != (size_t)-1)
    {
        size_t id = map->m_idMap[idx];
        GameCore* stored = &map->m_entries[id].currState;

        if (gameCore_equals(stored, state))
            return true;

        idx = (idx + 1) % map->m_capacity;
    }
    return false;
}


void gameCore_hashInsert(GameHashmap* map, GameCore curr, GameCore prev)
{
    uint64_t hash = gameCore_hash(&curr);
    size_t idx = hash % map->m_capacity;

    while (map->m_idMap[idx] != (size_t)-1)
    {
        size_t id = map->m_idMap[idx];
        if (gameCore_equals(&map->m_entries[id].currState, &curr))
            return; // déjà présent
        idx = (idx + 1) % map->m_capacity;
    }

    size_t id = map->m_size++;
    map->m_entries[id].currState = curr;
    map->m_entries[id].prevState = prev;
    map->m_idMap[idx] = id;
}


void gameCore_resolution()
{

}
