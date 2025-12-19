 /*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/game_graphics.h"
#include "common/assets.h"
#include "common/common.h"
#include "game/input.h"
#include "game/scene.h"
#include "game/game_config.h"

GameGraphics* GameGraphics_create(Scene* scene)
{
    GameGraphics* self = (GameGraphics*)calloc(1, sizeof(GameGraphics));
    AssertNew(self);

    self->m_scene = scene;
    self->m_padding = Vec2_set(0.0f, 0.0f);
    self->m_spacing = Vec2_set(0.1f, 0.1f);
    //
    self->m_rotationGrid.lower = Vec2_add(Vec2_set(-6.f, -4.f), Vec2_set(16.0f, 4.5f));
    self->m_rotationGrid.upper = Vec2_add(Vec2_set(6.f, 4.f), Vec2_set(16.0f, 4.5f));
    self->m_gridAABB.lower = Vec2_add(Vec2_set(-5.f, -4.f), Vec2_set(6.0f, 4.5f));
    self->m_gridAABB.upper = Vec2_add(Vec2_set(+5.f, +4.f), Vec2_set(6.0f, 4.5f));
    //
    self->m_enabled = false;

    // Initialiser la position sélectionnée avec la position du joueur
    Vec2 playerPos = scene->m_gameCore->m_playerPosition;
    self->m_selectedRowIndex = (int)playerPos.x;
    self->m_selectedColIndex = (int)playerPos.y;
    //

    AssetManager* assets = Scene_getAssetManager(scene);
    SpriteSheet* spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_PLAYER);
    SpriteSheet* crateSpriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_CRATE);
    SpriteSheet* pillarSpriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_PILLAR);
    SpriteSheet* crystalSpriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_CRYSTAL);
    SpriteSheet* demonSpriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_DEMON);
    SpriteSheet* keySpriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_KEY);
    SpriteSheet* axeSpriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_AXE);
    AssertNew(spriteSheet);
    AssertNew(crateSpriteSheet);
    AssertNew(pillarSpriteSheet);
    AssertNew(crystalSpriteSheet);
    AssertNew(demonSpriteSheet);
    AssertNew(keySpriteSheet);
    AssertNew(axeSpriteSheet);
    self->m_spritePlayerTop = SpriteSheet_getGroupByName(spriteSheet, "top");
    self->m_spritePlayerBotKey = SpriteSheet_getGroupByName(spriteSheet, "bot_key");
    self->m_spritePlayerBotNoKey = SpriteSheet_getGroupByName(spriteSheet, "bot_no_key");
    self->m_spritePlayerBack = SpriteSheet_getGroupByName(spriteSheet, "back");
    self->m_spritePlayerFront = SpriteSheet_getGroupByName(spriteSheet, "front");
    self->m_spritePlayerShield = SpriteSheet_getGroupByName(spriteSheet, "Shield");
    self->m_spritePlayerSideAxe = SpriteSheet_getGroupByName(spriteSheet, "side_axe");
    self->m_spritePlayerSideNoAxe = SpriteSheet_getGroupByName(spriteSheet, "side_no_axe");

    self->m_spriteCrate = SpriteSheet_getGroupByName(crateSpriteSheet, "crate");
    self->m_spritePillar = SpriteSheet_getGroupByName(pillarSpriteSheet, "pillar");
    self->m_spriteCrystal = SpriteSheet_getGroupByName(crystalSpriteSheet, "crystal");
    self->m_spriteDemon = SpriteSheet_getGroupByName(demonSpriteSheet, "demon");
    self->m_spriteKey = SpriteSheet_getGroupByName(keySpriteSheet, "key");
    self->m_spriteAxe = SpriteSheet_getGroupByName(axeSpriteSheet, "axe");
    AssertNew(self->m_spritePlayerTop);
    AssertNew(self->m_spritePlayerBotKey);
    AssertNew(self->m_spritePlayerBotNoKey);
    AssertNew(self->m_spritePlayerBack);
    AssertNew(self->m_spritePlayerFront);
    AssertNew(self->m_spritePlayerShield);
    AssertNew(self->m_spritePlayerSideAxe);
    AssertNew(self->m_spritePlayerSideNoAxe);

    AssertNew(self->m_spriteCrate);
    AssertNew(self->m_spritePillar);
    AssertNew(self->m_spriteCrystal);
    AssertNew(self->m_spriteDemon);
    AssertNew(self->m_spriteKey);
    AssertNew(self->m_spriteAxe);
    return self;
}

void GameGraphics_destroy(GameGraphics* self)
{
    if (!self) return;
    free(self);
}

//
static void GameGraphics_updateCells(GameGraphics* self)
{
    float totalPaddingX = self->m_padding.x * 2.f;
    float totalPaddingY = self->m_padding.y * 2.f;
    float totalSpacingX = self->m_spacing.x * (GAME_GRID_SIZE_X - 1);
    float totalSpacingY = self->m_spacing.y * (GAME_GRID_SIZE_Y - 1);
    Vec2 gridSize = AABB_getSize(&(self->m_gridAABB));
    Vec2 rotationGridSize = AABB_getSize(&(self->m_rotationGrid));
    float cellW = (gridSize.x - totalPaddingX - totalSpacingX) / (GAME_GRID_SIZE_X + 1);
    float cellH = (gridSize.y - totalPaddingY - totalSpacingY) / (GAME_GRID_SIZE_Y + 1);
    Vec2 playerPos = self->m_scene->m_gameCore->m_playerPosition;
    self->m_selectedRowIndex = (int)playerPos.x;
    self->m_selectedColIndex = (int)playerPos.y;


    for (int i = 0; i < GAME_GRID_SIZE_Y; i++)
    {
        for (int j = 0; j < GAME_GRID_SIZE_X; j++)
        {
            float cellX = self->m_gridAABB.lower.x + self->m_padding.x + j * (cellW + self->m_spacing.x);
            float cellY = self->m_gridAABB.lower.y + self->m_padding.y + i * (cellH + self->m_spacing.y);
            self->m_cells[GAME_GRID_SIZE_Y - 1 - i][j].lower = Vec2_set(cellX, cellY);
            self->m_cells[GAME_GRID_SIZE_Y - 1 - i][j].upper = Vec2_set(cellX + cellW, cellY + cellH);
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float cellX = self->m_rotationGrid.lower.x + self->m_padding.x + j * (cellW + self->m_spacing.x);
            float cellY = self->m_rotationGrid.lower.y + self->m_padding.y + i * (cellH + self->m_spacing.y);
            self->m_rotationCells[3 - 1 - i][j].lower = Vec2_set(cellX, cellY);
            self->m_rotationCells[3 - 1 - i][j].upper = Vec2_set(cellX + cellW, cellY + cellH);
        }
    }
}
//


void GameGraphics_update(GameGraphics* self)
{
    assert(self && "self must not be NULL");
    Scene* scene = self->m_scene;
    Input* input = Scene_getInput(scene);
    Camera* camera = Scene_getCamera(scene);

    GameGraphics_updateCells(self);

    Vec2 mouseWorldPos = { 0 };
    Camera_viewToWorld(camera, input->mouse.position.x, input->mouse.position.y, &mouseWorldPos);

    if (self->m_enabled == false)
    {
        return;
    }

    if (input->mouse.rightPressed)
    {
        gameCore_rotationBouclier(&self->m_scene->m_gameCore->player, self->m_scene->m_gameCore);
    }

    if (input->mouse.leftPressed)
    {
        for (int i = 0; i < GAME_GRID_SIZE_Y; i++)
        {
            for (int j = 0; j < GAME_GRID_SIZE_X; j++)
            {
                AABB* cellAABB = &(self->m_cells[i][j]);
                if (AABB_containsPoint(cellAABB, mouseWorldPos))
                {
                    int direction = 0;
                    if (self->m_selectedRowIndex == i && self->m_selectedColIndex != j)
                    {
                        if (self->m_selectedColIndex < j)
                        {
                            direction = DROITE;
                        }
                        else
                        {
                            direction = GAUCHE;
                        }
                    }
                    if (self->m_selectedRowIndex != i && self->m_selectedColIndex == j)
                    {
                        if (self->m_selectedRowIndex < i)
                        {
                            direction = BAS;
                        }
                        else
                        {
                            direction = HAUT;
                        }
                    }
                    if (gameCore_tryMove(direction, self->m_scene->m_gameCore))
                    {
                        self->m_selectedRowIndex = i;
                        self->m_selectedColIndex = j;
                        gameCore_movePlayer(direction, self->m_scene->m_gameCore);
                    }
                }
            }
        }
    }
}

void GameGraphics_render(GameGraphics* self)
{
    assert(self && "self must not be NULL");

    Scene* scene = self->m_scene;
    Camera* camera = Scene_getCamera(scene);
    float scale = Camera_getWorldToViewScale(camera);
    self->m_selectedColIndex = self->m_scene->m_gameCore->m_playerPosition.y;
    self->m_selectedRowIndex= self->m_scene->m_gameCore->m_playerPosition.x;

    SDL_FRect rect = { 0 };
    for (int i = 0; i < GAME_GRID_SIZE_Y; i++)
    {
        for (int j = 0; j < GAME_GRID_SIZE_X; j++)
        {
            AABB* cellAABB = &(self->m_cells[i][j]);
            rect.x = Camera_worldToViewX(camera, cellAABB->lower.x);
            rect.y = Camera_worldToViewY(camera, cellAABB->upper.y);
            rect.w = (cellAABB->upper.x - cellAABB->lower.x) * scale;
            rect.h = (cellAABB->upper.y - cellAABB->lower.y) * scale;
            bool isSelected = (i == self->m_selectedRowIndex && j == self->m_selectedColIndex);

            SDL_Color color = isSelected ? g_colors.orange9 : g_colors.gray8;
            SDL_SetRenderDrawColor(g_renderer, color.r, color.g, color.b, 255);
            SDL_RenderFillRect(g_renderer, &rect);

            switch (self->m_scene->m_gameCore->board[i][j])
            {
            case CRATE:
                SpriteGroup_render(self->m_spriteCrate, 0, &rect, Vec2_anchor_north_west, 1.0f);
                break;
            case CRYSTAL:
                SpriteGroup_render(self->m_spriteCrystal, 0, &rect, Vec2_anchor_north_west, 1.0f);
                break;
            case MONSTER:
                SpriteGroup_render(self->m_spriteDemon, 0, &rect, Vec2_anchor_north_west, 1.0f);
                break;
            case PILLAR:
                SpriteGroup_render(self->m_spritePillar, 0, &rect, Vec2_anchor_north_west, 1.0f);
                break;
            case KEY:
                SpriteGroup_render(self->m_spriteKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                break;
            case AXE:
                SpriteGroup_render(self->m_spriteAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                break;
            case PLAYER:
                switch (self->m_scene->m_gameCore->player.faceCiel)
                {
                case TETE:
                    SpriteGroup_render(self->m_spritePlayerTop, 0, &rect, Vec2_anchor_north_west, 1.0f);
                    break;
                case DOS:
                    SpriteGroup_render(self->m_spritePlayerBack, 0, &rect, Vec2_anchor_north_west, 1.0f);
                    break;
                case BOUCLIER:
                    SpriteGroup_render(self->m_spritePlayerShield, 0, &rect, Vec2_anchor_north_west, 1.0f);
                    break;
                case VISAGE:
                    SpriteGroup_render(self->m_spritePlayerFront, 0, &rect, Vec2_anchor_north_west, 1.0f);
                    break;
                case CREUX_HACHE:
                    if (self->m_scene->m_gameCore->AxeCollected)
                    {
                        SpriteGroup_render(self->m_spritePlayerSideAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    }
                    else
                    {
                        SpriteGroup_render(self->m_spritePlayerSideNoAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    }
                case CREUX_CLE:
                    if (self->m_scene->m_gameCore->CleCollected)
                    {
                        SpriteGroup_render(self->m_spritePlayerBotKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    }
                    else
                    {
                        SpriteGroup_render(self->m_spritePlayerBotNoKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    }
                }
            }
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            AABB* RotationGrid = &(self->m_rotationCells[i][j]);
            rect.x = Camera_worldToViewX(camera, RotationGrid->lower.x);
            rect.y = Camera_worldToViewY(camera, RotationGrid->upper.y);
            rect.w = (RotationGrid->upper.x - RotationGrid->lower.x) * scale;
            rect.h = (RotationGrid->upper.y - RotationGrid->lower.y) * scale;
            switch (i)
            {
            case 0:
                switch (j)
                {
                case 0:
                    break;
                case 1:
                    switch (self->m_scene->m_gameCore->player.facePorte)
                    {
                    case TETE:
                        SpriteGroup_render(self->m_spritePlayerTop, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case DOS:
                        SpriteGroup_render(self->m_spritePlayerBack, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case BOUCLIER:
                        SpriteGroup_render(self->m_spritePlayerShield, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case VISAGE:
                        SpriteGroup_render(self->m_spritePlayerFront, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case CREUX_HACHE:
                        if (self->m_scene->m_gameCore->AxeCollected)
                        {
                            SpriteGroup_render(self->m_spritePlayerSideAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        else
                        {
                            SpriteGroup_render(self->m_spritePlayerSideNoAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                    case CREUX_CLE:
                        if (self->m_scene->m_gameCore->CleCollected)
                        {
                            SpriteGroup_render(self->m_spritePlayerBotKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        else
                        {
                            SpriteGroup_render(self->m_spritePlayerBotNoKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                    }
                case 2:
                    break;
                }
                break;
            case 1:
                switch (j)
                {
                case 0:
                    switch (self->m_scene->m_gameCore->player.faceGauchePorte)
                    {
                    case VISAGE:
                        SpriteGroup_render(self->m_spritePlayerFront, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case CREUX_HACHE:
                        if (self->m_scene->m_gameCore->AxeCollected)
                        {
                            SpriteGroup_render(self->m_spritePlayerSideAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        else
                        {
                            SpriteGroup_render(self->m_spritePlayerSideNoAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                    case TETE:
                        SpriteGroup_render(self->m_spritePlayerTop, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case DOS:
                        SpriteGroup_render(self->m_spritePlayerBack, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case BOUCLIER:
                        SpriteGroup_render(self->m_spritePlayerShield, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case CREUX_CLE:
                        if (self->m_scene->m_gameCore->CleCollected)
                        {
                            SpriteGroup_render(self->m_spritePlayerBotKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        else
                        {
                            SpriteGroup_render(self->m_spritePlayerBotNoKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        break;
                    }
                    break;
                case 1:
                    switch (self->m_scene->m_gameCore->player.faceTerre)
                    {
                    case VISAGE:
                        SpriteGroup_render(self->m_spritePlayerFront, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case CREUX_HACHE:
                        if (self->m_scene->m_gameCore->AxeCollected)
                        {
                            SpriteGroup_render(self->m_spritePlayerSideAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        else
                        {
                            SpriteGroup_render(self->m_spritePlayerSideNoAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        break;
                    case CREUX_CLE:
                        if (self->m_scene->m_gameCore->CleCollected)
                        {
                            SpriteGroup_render(self->m_spritePlayerBotKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        else
                        {
                            SpriteGroup_render(self->m_spritePlayerBotNoKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                    case TETE:
                        SpriteGroup_render(self->m_spritePlayerTop, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case DOS:
                        SpriteGroup_render(self->m_spritePlayerBack, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case BOUCLIER:
                        SpriteGroup_render(self->m_spritePlayerShield, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    }
                    break;
                case 2:
                    switch (self->m_scene->m_gameCore->player.faceDroitePorte)
                    {
                    case VISAGE:
                        SpriteGroup_render(self->m_spritePlayerFront, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case CREUX_HACHE:
                        if (self->m_scene->m_gameCore->AxeCollected)
                        {
                            SpriteGroup_render(self->m_spritePlayerSideAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        else
                        {
                            SpriteGroup_render(self->m_spritePlayerSideNoAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        break;
                    case CREUX_CLE:
                        if (self->m_scene->m_gameCore->CleCollected)
                        {
                            SpriteGroup_render(self->m_spritePlayerBotKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        else
                        {
                            SpriteGroup_render(self->m_spritePlayerBotNoKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                    case TETE:
                        SpriteGroup_render(self->m_spritePlayerTop, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case DOS:
                        SpriteGroup_render(self->m_spritePlayerBack, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    case BOUCLIER:
                        SpriteGroup_render(self->m_spritePlayerShield, 0, &rect, Vec2_anchor_north_west, 1.0f);
                        break;
                    }
                    break;
                }
                break;
                case 2:
                    switch (j)
                    {
                    case 0:
                        break;
                    case 1:
                        switch (self->m_scene->m_gameCore->player.faceOppPorte)
                        {
                        case VISAGE:
                            SpriteGroup_render(self->m_spritePlayerFront, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        case CREUX_HACHE:
                            if (self->m_scene->m_gameCore->AxeCollected)
                            {
                                SpriteGroup_render(self->m_spritePlayerSideAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                                break;
                            }
                            else
                            {
                                SpriteGroup_render(self->m_spritePlayerSideNoAxe, 0, &rect, Vec2_anchor_north_west, 1.0f);
                                break;
                            }
                            break;
                        case CREUX_CLE:
                            if (self->m_scene->m_gameCore->CleCollected)
                            {
                                SpriteGroup_render(self->m_spritePlayerBotKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                                break;
                            }
                            else
                            {
                                SpriteGroup_render(self->m_spritePlayerBotNoKey, 0, &rect, Vec2_anchor_north_west, 1.0f);
                                break;
                            }
                        case TETE:
                            SpriteGroup_render(self->m_spritePlayerTop, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        case DOS:
                            SpriteGroup_render(self->m_spritePlayerBack, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        case BOUCLIER:
                            SpriteGroup_render(self->m_spritePlayerShield, 0, &rect, Vec2_anchor_north_west, 1.0f);
                            break;
                        }
                        break;
                    case 2:
                        break;
                    }
            }
        }
    }
}


