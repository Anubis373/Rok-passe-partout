/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "common/assets.h"

void Game_addAssets(AssetManager* assets)
{
    AssetManager_addSpriteSheet(
        assets, SPRITE_UI_BASE, "atlas/ui_base", "atlas/ui_base_desc"
    );
    AssetManager_addSpriteSheet(
        assets, SPRITE_GAME, "atlas/jump_in", "atlas/jump_in_desc"
    );

    AssetManager_addSpriteSheet(
        assets, SPRITE_CRATE, "atlas/crate", "atlas/crate_desc"
    );

    AssetManager_addSpriteSheet(
        assets, SPRITE_PILLAR, "atlas/pillar", "atlas/pillar_desc"
    );

    AssetManager_addSpriteSheet(
        assets, SPRITE_CRYSTAL, "atlas/crystal", "atlas/crystal_desc"
    );

    // -------------------------------------------------------------------------
    // Fonts

    AssetManager_addFont(
        assets, FONT_NORMAL, "font/noto_sans_regular", 32
    );
    AssetManager_addFont(
        assets, FONT_BOLD, "font/noto_sans_bold", 32
    );
    AssetManager_addFont(
        assets, FONT_BIG, "font/noto_sans_regular", 64
    );
    AssetManager_addFont(
        assets, FONT_BIG_BOLD, "font/noto_sans_bold", 64
    );
}
