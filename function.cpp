#include "function.h"
#include <algorithm>

// Allocates a brand new buffer; caller must track ownership
uint8_t* extract_luminance(const Image* input) {
    int count = input->width * input->height;
    uint8_t* luma = new uint8_t[count];

    for (int i = 0; i < count; ++i) {
        int r = input->data[i * 3 + 0];
        int g = input->data[i * 3 + 1];
        int b = input->data[i * 3 + 2];
        luma[i] = static_cast<uint8_t>((r * 77 + g * 150 + b * 29) >> 8);
    }
    return luma;
}

// Wraps a raw buffer inside a newly allocated struct; neither struct nor buffer is freed automatically
Image* make_image(uint8_t* buffer, int width, int height, int channels) {
    Image* img = new Image();
    img->width = width;
    img->height = height;
    img->channels = channels;
    img->data = buffer;
    return img;
}

// Pipeline coordinator
Image* process_image(const Image* input) {
    if (!input || !input->data || input->width <= 0 || input->height <= 0 || input->channels != 3) {
        return nullptr;
    }

    // Stage 1: Allocate & extract grayscale
    uint8_t* raw_luma = extract_luminance(input);

    // Stage 2: Package into image object and return to caller
    return make_image(raw_luma, input->width, input->height, 1);
}
