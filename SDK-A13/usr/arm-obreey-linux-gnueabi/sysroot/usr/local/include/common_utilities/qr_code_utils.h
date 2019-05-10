#ifndef QR_CODE_UTILS_H
#define QR_CODE_UTILS_H

#include "smartbitmap.h"

namespace pocketbook {
namespace utilities {

SmartBitmap MakeQrCode(const std::string& text, int preffered_size);

}
}


#endif // QR_CODE_UTILS_H
