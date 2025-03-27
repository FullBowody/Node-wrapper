#pragma once
#include "b64.h"
#include "Camera/Image.hpp"

/**
 * Encode an image to base64.
 * @param image The image to encode.
 * @param quality The quality of the encoding.
 * @return The base64 encoded image.
 * NOTE: The caller is responsible for freeing the returned string.
 */
char* base64_encodeImage(const Image& image, int quality = 80);