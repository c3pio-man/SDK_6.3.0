#ifndef TTSPM_GLOBAL_H
#define TTSPM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TTSPM_LIBRARY)
#  define TTSPMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TTSPMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TTSPM_GLOBAL_H
