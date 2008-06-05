# Copyright 2008, Google Inc.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#  3. Neither the name of Google Inc. nor the names of its contributors may be
#     used to endorse or promote products derived from this software without
#     specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Discover the OS
ifeq ($(shell uname),Linux)
OS = linux
else
ifeq ($(shell uname),Darwin)
OS = osx
else
OS = win32
endif
endif

# Set default build mode
#   dbg = debug build
#   opt = release build
MODE = dbg

# Uncomment the following line to build with XML support
ENABLE_XML = -DGLINT_ENABLE_XML

# Set default OS architecture
#   OSX builds will override this value (see rules.mk).
#   For other platforms we set just one value.
ARCH = i386

# $(shell ...) statements need to be different on Windows (%% vs %).
ifeq ($(OS),win32)
PCT=%%
else
PCT=%
endif

MAKEFLAGS = --no-print-directory

CPPFLAGS = -I.. -I$(OUTDIR)/$(OS)-$(ARCH)

######################################################################
# OS == linux
######################################################################
ifeq ($(OS),linux)
CC = gcc
CXX = g++
OBJ_SUFFIX = .o
MKDEP = gcc -M -MF $(@D)/$*.pp -MT $@ $(CPPFLAGS) $<

CPPFLAGS += -DLINUX `pkg-config --cflags gtk+-2.0`

COMPILE_FLAGS_dbg = -g -O0
COMPILE_FLAGS_opt = -Os
COMPILE_FLAGS = -c -o $@ -fPIC -fmessage-length=0 -Wall -Werror $(COMPILE_FLAGS_$(MODE))

CFLAGS = $(COMPILE_FLAGS)
CXXFLAGS = $(COMPILE_FLAGS) -fno-exceptions -fno-rtti -Wno-non-virtual-dtor -Wno-ctor-dtor-privacy -funsigned-char

DLL_SUFFIX = .so
MKSHLIB = g++
SHLIBFLAGS = -o $@ -shared -fPIC -Bsymbolic

LINKFLAGS = -o $@ `pkg-config --libs gtk+-2.0`
LINKFLAGS_GLINT_TEST = $(LINKFLAGS)

LINKFLAGS_HELLO_WORLD = $(LINKFLAGS)
MKEXE = g++
endif

######################################################################
# OS == osx
######################################################################
ifeq ($(OS),osx)
OSX_SDK_ROOT = /Developer/SDKs/MacOSX10.4u.sdk
CC = gcc -arch $(ARCH)
CXX = g++ -arch $(ARCH)
OBJ_SUFFIX = .o
MKDEP = gcc -M -MF $(@D)/$*.pp -MT $@ $(CPPFLAGS) $<

CPPFLAGS += -DLINUX -DOS_MACOSX

COMPILE_FLAGS_dbg = -g -O0
COMPILE_FLAGS_opt = -O2
COMPILE_FLAGS = -c -o $@ -fPIC -fmessage-length=0 -Wall -Werror
COMPILE_FLAGS += $(COMPILE_FLAGS_$(MODE)) -isysroot $(OSX_SDK_ROOT)
COMPILE_FLAGS += -DMAC_OS_X_VERSION_MIN_REQUIRED=1040 -DOSX_USE_COCOA -DOSX
COMPILE_FLAGS += -mmacosx-version-min=10.4

CFLAGS = $(COMPILE_FLAGS)
CXXFLAGS = $(COMPILE_FLAGS) -fno-exceptions -fno-rtti -Wno-non-virtual-dtor
CXXFLAGS += -Wno-ctor-dtor-privacy -funsigned-char

DLL_SUFFIX = .dylib
MKEXE = g++
LINKFLAGS = -o $@  -arch $(ARCH)
LINKFLAGS += -framework AppKit -framework CoreServices -framework Carbon
LINKFLAGS += -isysroot $(OSX_SDK_ROOT) -ObjC
LINKFLAGS += -Xlinker -undefined -Xlinker error -mmacosx-version-min=10.4

LINKFLAGS_GLINT_TEST = $(LINKFLAGS)

LINKFLAGS_HELLO_WORLD = $(LINKFLAGS)
LINKFLAGS_HELLO_WORLD += -Xlinker -sectcreate
LINKFLAGS_HELLO_WORLD += -Xlinker __TEXT
LINKFLAGS_HELLO_WORLD += -Xlinker __info_plist
LINKFLAGS_HELLO_WORLD += -Xlinker sample_apps/hello_world/Info.plist

COPY = cp

endif

######################################################################
# OS == win32
######################################################################
ifeq ($(OS),win32)
CC = cl
CXX = cl
OBJ_SUFFIX = .obj
MKDEP = python.exe tools/mkdepend.py $< $@ > $(@D)/$*.pp

# Most Windows headers use the cross-platform NDEBUG and DEBUG #defines
# (handled later).  But a few Windows files look at _DEBUG instead.
CPPFLAGS_dbg = -D_DEBUG=1
CPPFLAGS_opt =
CPPFLAGS += /nologo \
            -DSTRICT \
            -D_UNICODE \
            -DUNICODE \
            -DWIN32 \
            -D_CRT_SECURE_NO_DEPRECATE

CPPFLAGS += -D_WINDOWS \
            -DWINVER=0x0500 \
            -D_WIN32_WINNT=0x0500 \
            $(CPPFLAGS_$(MODE))

COMPILE_FLAGS_dbg = /MTd
ifdef CRT_DLL
COMPILE_FLAGS_opt = /O1 /Os /Oy /Oi /GL /MD /wd4275
else
COMPILE_FLAGS_opt = /O1 /Os /Oy /Oi /GL /MT
endif
COMPILE_FLAGS = /c /Fo"$@" /Fd"$(@D)/$*.pdb" /W3 /WX /GR- /Gy /Gd /Zi
COMPILE_FLAGS += $(COMPILE_FLAGS_$(MODE))
# In VC8, the way to disable exceptions is to remove all /EH* flags, and to
# define _HAS_EXCEPTIONS=0 (for C++ headers) and _ATL_NO_EXCEPTIONS (for ATL).
COMPILE_FLAGS += -D_HAS_EXCEPTIONS=0

CFLAGS = $(COMPILE_FLAGS)
CXXFLAGS = $(COMPILE_FLAGS) /TP /J

DLL_SUFFIX = .dll
EXE_SUFFIX = .exe
MKEXE = link
LIBS = ole32.lib gdi32.lib gdiplus.lib user32.lib
# /RELEASE adds a checksum to the PE header to aid symbol loading.
# /DEBUG causes PDB files to be produced.
# We want both these flags in all build modes, despite their names.
LINKFLAGS_dbg =
LINKFLAGS_opt = /INCREMENTAL:NO /OPT:REF /OPT:ICF /OPT:NOWIN98 /LTCG /MAP
LINKFLAGS = /NOLOGO /OUT:$@ /DEBUG /RELEASE /MACHINE:X86 $(LINKFLAGS_$(MODE)) $(LIBS)

LINKFLAGS_GLINT_TEST = $(LINKFLAGS)
LINKFLAGS_HELLO_WORLD = $(LINKFLAGS) /SUBSYSTEM:WINDOWS

RC = rc
RCFLAGS_dbg = /DDEBUG=1
RCFLAGS_opt = /DNDEBUG=1
RCFLAGS = $(RCFLAGS_$(MODE)) /d "_UNICODE" /d "UNICODE" /l 0x409 /fo $@

endif

######################################################################
# set the following for all OS values
######################################################################

# For Expat XML library.
ifdef ENABLE_XML
CPPFLAGS += -DXML_STATIC -Ithird_party/expat/v2_0_1/source/lib
ifeq ($(OS),win32)
CPPFLAGS += -DCOMPILED_FROM_DSP
else
CPPFLAGS += -DHAVE_EXPAT_CONFIG_H
endif  # win32
endif  # ENABLE_XML

CPPFLAGS += $(INCLUDES) $(ENABLE_XML)

ifeq ($(MODE),dbg)
CPPFLAGS += -DDEBUG=1
else
CPPFLAGS += -DNDEBUG=1
endif

