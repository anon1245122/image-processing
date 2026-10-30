#pragma once
#include <cstdint>

struct Image {
    int width;
    int height;
    int channels;
    uint8_t* data;
};

// Pipeline stage helpers
uint8_t* extract_luminance(const Image* input);
Image* make_image(uint8_t* buffer, int width, int height, int channels);

// Primary pipeline entrypoint
Image* process_image(const Image* input);
