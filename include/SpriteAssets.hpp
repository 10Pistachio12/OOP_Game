#ifndef SPRITE_ASSETS_HPP
#define SPRITE_ASSETS_HPP

#include "pch.hpp"

#include "config.hpp"

namespace SpriteAssets {
inline std::string Path(const std::string &filename) {
    return std::string(RESOURCE_DIR) + "/Sprites/" + filename;
}
} // namespace SpriteAssets

#endif
