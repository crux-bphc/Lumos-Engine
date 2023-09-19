#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

// Global variables for FreeType
FT_Library ft;
FT_Face face;

class Text {
   private:
    const char* font_path;
    int font_size;

   public:
    /// @brief Constructor
    /// @param font_path path to .ttf file
    /// @param font_size in pixels
    Text(const char* font_path, int font_size) {
        if (FT_Init_FreeType(&ft)) {
            spdlog::error("Failed to initialize FreeType");
        }

        if (FT_New_Face(ft, font_path, 0, &face)) {
            spdlog::error("Failed to load font");
        }

        FT_Set_Pixel_Sizes(face, 0, font_size);  // Set font size
    }
};
