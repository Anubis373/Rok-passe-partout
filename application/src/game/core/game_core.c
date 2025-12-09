/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game_core.h"
#include "game/scene.h"

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

int Get_Move(Scene* scene)
{
    Vec2 RelativeMousePosition = Vec2_sub(scene->m_gameCore->m_playerPosition, scene->m_input->mouse.position);
    if (max(RelativeMousePosition.x, RelativeMousePosition.y) == abs(RelativeMousePosition.x))
    {
        if (RelativeMousePosition.x > 0)
        {
            return(DROITE);
        }
        else
        {
            return(GAUCHE);
        }
    }
    else
    {
        if (RelativeMousePosition.y > 0)
        {
            return(HAUT);
        }
        else
        {
            return(BAS);
        }
    }
}
