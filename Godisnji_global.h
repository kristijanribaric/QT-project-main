  #ifndef GODISNJI_GLOBAL_H
#define GODISNJI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GODISNJI_LIBRARY)
#  define GODISNJI_EXPORT Q_DECL_EXPORT
#else
#  define GODISNJI_EXPORT Q_DECL_IMPORT
#endif

#endif // GODISNJI_GLOBAL_H
