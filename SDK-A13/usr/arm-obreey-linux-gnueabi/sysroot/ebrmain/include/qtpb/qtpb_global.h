#ifndef QTPB_GLOBAL_H
#define QTPB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTPB_LIBRARY)
#  define QTPBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTPBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTPB_GLOBAL_H
