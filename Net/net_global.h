#ifndef NET_GLOBAL_H
#define NET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NET_LIBRARY)
#  define NETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NETSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NET_GLOBAL_H
