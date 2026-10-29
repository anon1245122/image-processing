#include "function.h"
#include <algorithm>
#include <memory>

// Pipeline stage helpers refactored to return managed std::unique_ptr buffers
std::unique_ptr<uint8_t[]> extract_luminance(const Image* input) {
    int count = input->width * input->height;
    auto luma = std::make_unique<uint8_t[]>(count);

    for (int i = 0; i < count; ++i) {
        int r = input->data[i * 3 + 0];
        int g = input->data[i * 3 + 1];
        int b = input->data[i * 3 + 2];
        luma[i] = static_cast<uint8_t>((r * 77 + g * 150 + b * 29) >> 8);
    }
    return luma;
}

Image* make_image(std::unique_ptr<uint8_t[]> buffer, int width, int height, int channels) {
    auto img = std::make_unique<Image>();
    img->width = width;
    img->height = height;
    img->channels = channels;
    img->data = buffer.get();
    return img.release();
}

// Pipeline coordinator refactored to RAII
Image* process_image(const Image* input) {
    if (!input || !input->data || input->width <= 0 || input->height <= 0 || input->channels != 3) {
        return nullptr;
    }

    // Stage 1: Allocate & extract grayscale via unique_ptr
    auto raw_luma = extract_luminance(input);

    // Stage 2: Package into image object and transfer buffer ownership to Image container
    return make_image(std::move(raw_luma), input->width, input->height, 1);
}
