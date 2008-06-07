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

include tools/config.mk

#-------------------------------------------------------------------------------
# Crossplatform files

XPLAT_OBJ_OUTDIR = $(COMMON_OUTDIR)/xplat_obj

XPLAT_VPATH = \
	crossplatform

XPLAT_CPP_SOURCES = \
	allocator.cc \
	animation_timeline.cc \
	base_object.cc \
	bitmap.cc \
	button.cc \
	color.cc \
	column.cc \
	core.cc \
	current_time.cc \
	draw_stack.cc \
	image_node.cc \
	lexer.cc \
	message.cc \
	nine_grid.cc \
	node.cc \
	rectangle.cc \
	root_ui.cc \
	row.cc \
	simple_text.cc \
	size.cc \
	snapshot_stack.cc \
	tile_plane.cc \
	transform.cc \
	type_system.cc \
	xml_lite.cc \
	xml_parser.cc

#	TODO(dimich): figure out the unittest story.
#	color_unittest.cc \
#	pixel_tricks_unittest.cc \

XPLAT_OBJS = $(patsubst %.cc,$(XPLAT_OBJ_OUTDIR)/%$(OBJ_SUFFIX),$(XPLAT_CPP_SOURCES))

DEPS += $(XPLAT_OBJS:$(OBJ_SUFFIX)=.pp)
VPATH += $(XPLAT_VPATH)

#-------------------------------------------------------------------------------
# Win32 files

ifeq ($(OS),win32)

PLATFORM_VPATH = win

PLATFORM_CPP_SOURCES = \
	formatted_text.cc \
	win32.cc

endif

#-------------------------------------------------------------------------------
# Mac files

ifeq ($(OS),osx)

PLATFORM_VPATH = mac posix

PLATFORM_OBJC_SOURCES = \
	darwin_platform_bitmaps.mm \
	darwin_platform_fonttext.mm \
	darwin_platform_misc.mm \
	darwin_platform_windows.mm \
	glint_bitmap_view.mm \
	glint_window.mm \
	glint_work_item_dispatcher.mm \

PLATFORM_CPP_SOURCES = \
	posix.cc \
	formatted_text.cc

PLATFORM_OBJS += $(patsubst %.mm,$(PLATFORM_OBJ_OUTDIR)/%$(OBJ_SUFFIX),$(PLATFORM_OBJC_SOURCES))

endif

#-------------------------------------------------------------------------------
# Linux files

ifeq ($(OS),linux)

PLATFORM_VPATH = linux posix

PLATFORM_CPP_SOURCES = \
	linux.cc \
	posix.cc \
	formatted_text.cc

PLATFORM_OBJS += $(patsubst %.mm,$(PLATFORM_OBJ_OUTDIR)/%$(OBJ_SUFFIX),$(PLATFORM_OBJC_SOURCES))

endif

#-------------------------------------------------------------------------------
# Common platform variables

PLATFORM_OBJS += $(patsubst %.cc,$(PLATFORM_OBJ_OUTDIR)/%$(OBJ_SUFFIX),$(PLATFORM_CPP_SOURCES))
DEPS += $(PLATFORM_OBJS:$(OBJ_SUFFIX)=.pp)
VPATH += $(PLATFORM_VPATH)
PLATFORM_OBJ_OUTDIR = $(COMMON_OUTDIR)/$(OS)_platform_obj

#-------------------------------------------------------------------------------
# XML library (expat)
# Link them in, put obj files into crossplatform obj dir.

ifdef ENABLE_XML
EXPAT_VPATH = third_party/expat/v2_0_1/source/lib
EXPAT_C_FILES = \
	xmlparse.c \
	xmltok.c \
	xmlrole.c

XPLAT_OBJS += $(patsubst %.c,$(XPLAT_OBJ_OUTDIR)/%$(OBJ_SUFFIX),$(EXPAT_C_FILES))
VPATH += $(EXPAT_VPATH)

endif

#-------------------------------------------------------------------------------
# Unittests

GLINT_TEST_EXE = glint_test$(EXE_SUFFIX)
GLINT_TEST_OBJ_OUTDIR = $(COMMON_OUTDIR)/glint_test_obj
GLINT_TEST_VPATH = test crossplatform

GLINT_TEST_CPP_SOURCES = \
	glint_unittest.cc \
	rectangle_unittest.cc \
	tile_plane_unittest.cc \
	transform_unittest.cc

GLINT_TEST_RESOURCES = \
	red_opaque.png \
	red_transparent.png \
	green_opaque.png \
	green_transparent.png \
	blue_opaque.png \
	blue_transparent.png

GLINT_TEST_IMAGES = $(addprefix $(COMMON_OUTDIR)/,$(GLINT_TEST_RESOURCES))

ifeq ($(OS),osx)
GLINT_TEST_VPATH += mac
GLINT_TEST_OBJC_SOURCES = darwin_platform_unittest.mm
GLINT_TEST_OBJS += $(patsubst %.mm,$(GLINT_TEST_OBJ_OUTDIR)/%$(OBJ_SUFFIX),$(GLINT_TEST_OBJC_SOURCES))
endif

GLINT_TEST_OBJS += $(patsubst %.cc,$(GLINT_TEST_OBJ_OUTDIR)/%$(OBJ_SUFFIX),$(GLINT_TEST_CPP_SOURCES))
DEPS += $(GLINT_TEST_OBJS:$(OBJ_SUFFIX)=.pp)
VPATH += $(GLINT_TEST_VPATH)


#-------------------------------------------------------------------------------
# Sample app base class

#-------------------------------------------------------------------------------
# HelloWorld sample app

HELLO_WORLD_EXE = hello_world$(EXE_SUFFIX)
HELLO_WORLD_OBJ_OUTDIR = $(COMMON_OUTDIR)/hello_world_obj
HELLO_WORLD_VPATH = \
	sample_apps/hello_world \
	sample_apps/main

HELLO_WORLD_CPP_SOURCES = \
	hello_world.cc

HELLO_WORLD_OBJS = $(patsubst %.cc,$(HELLO_WORLD_OBJ_OUTDIR)/%$(OBJ_SUFFIX),$(HELLO_WORLD_CPP_SOURCES))

ifeq ($(OS),win32)
HELLO_WORLD_CPP_SOURCES += \
	main_win32.cc

HELLO_WORLD_RC = hello_world.rc
HELLO_WORLD_RES = $(HELLO_WORLD_OBJ_OUTDIR)/hellow_world.res
endif

ifeq ($(OS),osx)
HELLO_WORLD_OBJC_SOURCES += \
	main_darwin.mm \
	sample_app_controller.mm

HELLO_WORLD_OBJS += $(patsubst %.mm,$(HELLO_WORLD_OBJ_OUTDIR)/%$(OBJ_SUFFIX),$(HELLO_WORLD_OBJC_SOURCES))

# This file is timestamped and used as the whole bundle target.
HELLO_WORLD_BUNDLE = $(COMMON_OUTDIR)/hello_world.app/Contents/Info.plist
endif

ifeq ($(OS),linux)
HELLO_WORLD_CPP_SOURCES += \
	main_linux.cc

endif

HELLO_WORLD_RESOURCES = test.xml dialog_bg.png button_strip.png
DEPS += $(HELLO_WORLD_OBJS:$(OBJ_SUFFIX)=.pp)
VPATH += $(HELLO_WORLD_VPATH)

include tools/rules.mk

