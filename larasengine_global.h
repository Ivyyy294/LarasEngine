#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(LARASENGINE_LIB)
#  define LARASENGINE_EXPORT Q_DECL_EXPORT
# else
#  define LARASENGINE_EXPORT Q_DECL_IMPORT
# endif
#else
# define LARASENGINE_EXPORT
#endif
