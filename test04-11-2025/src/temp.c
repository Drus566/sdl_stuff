#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to get next UTF-8 character and advance pointer
Uint32 utf8_to_codepoint(const char** text) {
    const unsigned char* p = (const unsigned char*)*text;
    Uint32 codepoint = 0;
    
    if (*p == 0) return 0;
    
    if (*p < 0x80) {
        // 1-byte UTF-8
        codepoint = *p;
        *text += 1;
    } else if ((*p & 0xE0) == 0xC0) {
        // 2-byte UTF-8
        codepoint = ((p[0] & 0x1F) << 6) | (p[1] & 0x3F);
        *text += 2;
    } else if ((*p & 0xF0) == 0xE0) {
        // 3-byte UTF-8
        codepoint = ((p[0] & 0x0F) << 12) | ((p[1] & 0x3F) << 6) | (p[2] & 0x3F);
        *text += 3;
    } else if ((*p & 0xF8) == 0xF0) {
        // 4-byte UTF-8
        codepoint = ((p[0] & 0x07) << 18) | ((p[1] & 0x3F) << 12) | ((p[2] & 0x3F) << 6) | (p[3] & 0x3F);
        *text += 4;
    } else {
        // Invalid UTF-8, skip one byte
        *text += 1;
        return 0;
    }
    
    return codepoint;
}

// Function to count UTF-8 characters
size_t utf8_strlen(const char* text) {
    size_t count = 0;
    const char* p = text;
    while (*p) {
        if ((*p & 0xC0) != 0x80) count++;
        p++;
    }
    return count;
}

int main() {
    // Unicode text in UTF-8
    const char* text = "申し訳ございませんがたくさんあります。";
    
    // Count characters for initial allocation
    size_t text_len = utf8_strlen(text);
    
    // Array for surfaces
    SDL_Surface** surfaces = malloc(text_len * sizeof(SDL_Surface*));
    if (!surfaces) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t surfaces_count = 0;
    const char* cursor = text;

    // Render each character as a surface
    while (*cursor) {
        Uint32 codepoint = utf8_to_codepoint(&cursor);
        if (codepoint == 0) continue; // Skip invalid characters
        
        SDL_Surface* textSurface = TTF_RenderGlyph_LCD(font, codepoint, 
                                                     (SDL_Color){255, 255, 255, 255}, 
                                                     (SDL_Color){0, 0, 0, 255});
        if (!textSurface) {
            fprintf(stderr, "Failed to create text surface for U+%04X: %s\n", 
                    codepoint, SDL_GetError());
            continue;
        }
        
        surfaces[surfaces_count++] = textSurface;
    }

    // Use surfaces here
    printf("Successfully rendered %zu surfaces\n", surfaces_count);
    
    // Example: Print information about each surface
    for (size_t i = 0; i < surfaces_count; i++) {
        if (surfaces[i]) {
            printf("Surface %zu: %dx%d pixels\n", 
                   i, surfaces[i]->w, surfaces[i]->h);
        }
    }
    
    // Cleanup
    for (size_t i = 0; i < surfaces_count; i++) {
        SDL_FreeSurface(surfaces[i]);
    }
    free(surfaces);
    
    return 0;
}