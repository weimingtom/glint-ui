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

# NOTES:
# - Quotes around "mkdir" are required so Windows cmd.exe uses mkdir.exe
#     instead of built-in mkdir command.  (Running mkdir.exe without
#     quotes creates a directory named '.exe'!!)

OUTDIR = bin-$(MODE)
COMMON_OUTDIR = $(OUTDIR)/$(OS)-$(ARCH)
PLATFORM_OBJ_OUTDIR = $(COMMON_OUTDIR)/$(OS)_platform_obj

OBJ_OUTDIRS =  $(XPLAT_OBJ_OUTDIR) $(PLATFORM_OBJ_OUTDIR)
OBJ_OUTDIRS += $(HELLO_WORLD_OBJ_OUTDIR) $(GLINT_TEST_OBJ_OUTDIR)

EXE_FILES =  $(COMMON_OUTDIR)/$(HELLO_WORLD_EXE)
EXE_FILES += $(COMMON_OUTDIR)/$(GLINT_TEST_EXE)

ifeq ($(OS),osx)
BUNDLES = $(HELLO_WORLD_BUNDLE)
endif

# BUILD TARGETS

default:
ifeq ($(OS),osx)
    # For osx, build the non-installer targets for multiple architectures.
	$(MAKE) prebuild
	$(MAKE) tests ARCH=i386
	$(MAKE) samples ARCH=i386
#	$(MAKE) samples ARCH=ppc
else
	$(MAKE) prebuild
	$(MAKE) tests
	$(MAKE) samples
endif

prebuild: $(OBJ_OUTDIRS)
	@echo prebuild is done.

samples: $(EXE_FILES) $(BUNDLES)
	@echo samples is done.

tests: $(COMMON_OUTDIR)/$(GLINT_TEST_EXE) $(GLINT_TEST_IMAGES)
	@echo tests is done

clean:
	rm -rf $(OUTDIR)

help:
	@echo "Usage: make [MODE=dbg|opt] [CRT_DLL=1]"
	@echo
	@echo "  If you omit MODE, the default is dbg."
	@echo "  CRT_DLL=1 links with DLL CRT to see what the real exe size is."

.PHONY: default prebuild samples clean help

# MKDIR TARGETS
$(OBJ_OUTDIRS):
	"mkdir" -p $@

# C/C++ TARGETS
$(XPLAT_OBJ_OUTDIR)/%$(OBJ_SUFFIX): %.cc
	@$(MKDEP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(XPLAT_CPPFLAGS) $(XPLAT_CXXFLAGS) $<

$(XPLAT_OBJ_OUTDIR)/%$(OBJ_SUFFIX): %.c
	@$(MKDEP)
	$(CC) $(CPPFLAGS) $(XPLAT_CPPFLAGS) $(CFLAGS) $<

$(PLATFORM_OBJ_OUTDIR)/%$(OBJ_SUFFIX): %.cc
	@$(MKDEP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(PLATFORM_CPPFLAGS) $(PLATFORM_CXXFLAGS) $<

$(PLATFORM_OBJ_OUTDIR)/%$(OBJ_SUFFIX): %.mm
	@$(MKDEP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(PLATFORM_CPPFLAGS) $(PLATFORM_CXXFLAGS) $<

$(HELLO_WORLD_OBJ_OUTDIR)/%$(OBJ_SUFFIX): %.cc
	@$(MKDEP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(HELLO_WORLD_CPPFLAGS) $(HELLO_WORLD_CXXFLAGS) $<

$(HELLO_WORLD_OBJ_OUTDIR)/%$(OBJ_SUFFIX): %.mm
	@$(MKDEP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(HELLO_WORLD_CPPFLAGS) $(HELLO_WORLD_CXXFLAGS) $<

$(GLINT_TEST_OBJ_OUTDIR)/%$(OBJ_SUFFIX): %.mm
	@$(MKDEP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(GLINT_TEST_CPPFLAGS) $(GLINT_TEST_CXXFLAGS) $<

$(GLINT_TEST_OBJ_OUTDIR)/%$(OBJ_SUFFIX): %.cc
	@$(MKDEP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(GLINT_TEST_CPPFLAGS) $(GLINT_TEST_CXXFLAGS) $<

$(GLINT_TEST_IMAGES): $(GLINT_TEST_RESOURCES)
	cp -f test/$(@F) $@

# RC TARGETS
$(HELLO_WORLD_RES): $(HELLO_WORLD_RC) $(HELLO_WORLD_RESOURCES)
	$(RC) $(RCFLAGS) $<

# LINK TARGETS

HELLO_WORLD_EXE_DEPS = $(XPLAT_OBJS) $(PLATFORM_OBJS) $(HELLO_WORLD_OBJS) $(HELLO_WORLD_RES)
$(COMMON_OUTDIR)/$(HELLO_WORLD_EXE): $(HELLO_WORLD_EXE_DEPS)
	$(MKEXE) $(LINKFLAGS_HELLO_WORLD) $(HELLO_WORLD_EXE_DEPS)

GLINT_TEST_EXE_DEPS = $(XPLAT_OBJS) $(PLATFORM_OBJS) $(GLINT_TEST_OBJS)
$(COMMON_OUTDIR)/$(GLINT_TEST_EXE): $(GLINT_TEST_EXE_DEPS)
	$(MKEXE) $(LINKFLAGS_GLINT_TEST) $(GLINT_TEST_EXE_DEPS)

ifeq ($(OS),osx)
$(HELLO_WORLD_BUNDLE): $(HELLO_WORLD_RESOURCES) $(COMMON_OUTDIR)/$(HELLO_WORLD_EXE)
	"mkdir" -p $(COMMON_OUTDIR)/hello_world.app/Contents/MacOS
	"mkdir" -p $(COMMON_OUTDIR)/hello_world.app/Contents/Resources
	cp -f $(COMMON_OUTDIR)/$(HELLO_WORLD_EXE) $(COMMON_OUTDIR)/hello_world.app/Contents/MacOS/
	cp -f sample_apps/hello_world/*.png $(COMMON_OUTDIR)/hello_world.app/Contents/Resources/
	cp -f sample_apps/hello_world/*.xml $(COMMON_OUTDIR)/hello_world.app/Contents/Resources/
	cp -f sample_apps/hello_world/Info.plist $(COMMON_OUTDIR)/hello_world.app/Contents/
	touch -f $(COMMON_OUTDIR)/hello_world.app/Contents/Info.plist
endif

# We generate dependency information for each source file as it is compiled.
# Here, we include the generated dependency information, which silently fails
# if the files do not exist.
-include $(DEPS)

