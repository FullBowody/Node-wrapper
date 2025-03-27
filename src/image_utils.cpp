#include "image_utils.hpp"

char* base64_encodeImage(const Image& image, int quality)
{
    int size;
    unsigned char* jpg = image.encodeJPG(quality, &size);
    char* encoded = b64_encode(jpg, size);
    delete[] jpg;
    return encoded;
}
