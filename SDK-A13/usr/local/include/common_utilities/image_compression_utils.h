#ifndef IMAGE_COMPRESSION_UTILS_H
#define IMAGE_COMPRESSION_UTILS_H

#include "inkview.h"
#include <iostream>
#include <string>
namespace pocketbook {
namespace utilities {

struct CompressedBitmapImpl;
class CompressedBitmap {
    CompressedBitmap();
    CompressedBitmap(const CompressedBitmap&) = delete;
    CompressedBitmap operator=(const CompressedBitmap&) = delete;
public:
    ~CompressedBitmap();
    static CompressedBitmap* fromRGB565(const void* src, int width, int height, int scanline);
    static CompressedBitmap* fromStream(std::istream& is);
    static CompressedBitmap* fromCompressedStream(std::istream& is);

    ibitmap* toGrayscale8();
    int toStream(std::ostream& os);
    int toCompressedStream(std::ostream& os);

private:
    CompressedBitmapImpl* impl_ = nullptr;
};

}
}


#endif // IMAGE_COMPRESSION_UTILS_H
