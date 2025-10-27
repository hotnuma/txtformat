TEMPLATE = app
TARGET = txtformat
CONFIG = c99 link_pkgconfig
DEFINES = _GNU_SOURCE bool=BOOL true=TRUE false=FALSE
INCLUDEPATH =
PKGCONFIG =

PKGCONFIG += tinyc

SOURCES = \
    0temp.c \
    main.c \
    txtparser.c

DISTFILES = \
    install.sh \
    License.txt \
    meson.build \
    Readme.md \

HEADERS += \
    txtparser.h


