# Copyright (c) 2013 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

#
# GNU Make based build file.  For details on GNU Make see:
# http://www.gnu.org/software/make/manual/make.html
#

#
# Get pepper directory for toolchain and includes.
#
# If NACL_SDK_ROOT is not set, then assume it can be found three directories up.
#
THIS_MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
NACL_SDK_ROOT ?= $(abspath $(dir $(THIS_MAKEFILE))../..)

# Project Build flags
WARNINGS := -Wno-long-long -Wall -Wswitch-enum -pedantic -Werror
CXXFLAGS := -pthread -std=gnu++98 $(WARNINGS)

#
# Compute tool paths
#
GETOS := python $(NACL_SDK_ROOT)/tools/getos.py
OSHELPERS = python $(NACL_SDK_ROOT)/tools/oshelpers.py
OSNAME := $(shell $(GETOS))
RM := $(OSHELPERS) rm

PNACL_TC_PATH := $(abspath $(NACL_SDK_ROOT)/toolchain/$(OSNAME)_pnacl)
PNACL_CXX := $(PNACL_TC_PATH)/bin/pnacl-clang++
PNACL_FINALIZE := $(PNACL_TC_PATH)/bin/pnacl-finalize
CXXFLAGS := -I$(NACL_SDK_ROOT)/include
LDFLAGS := -L$(NACL_SDK_ROOT)/lib/pnacl/Release -lppapi_cpp -lppapi

#
# Disable DOS PATH warning when using Cygwin based tools Windows
#
CYGWIN ?= nodosfilewarning
export CYGWIN

HDRS := stitching.h
SRCS := stitching.cc nacl_glue.cc

## Note that OPENCV should have been compiled and installed in the appropriate
## NaCl (pnacl, hopefully here) toolching pseudo root. So no need to paste any
LDFLAGS  += -lopencv_core

# Declare the ALL target first, to make the 'all' target the default build
all: guard-OPENCV_ROOT nacl_glue.pexe

clean:
	$(RM) *.pexe *.bc *.o

nacl_glue.bc: $(SRCS) $(HDRS)
	$(PNACL_CXX) -O2 $(CXXFLAGS) $(SRCS) $(LDFLAGS)   -o $@

nacl_glue.pexe: nacl_glue.bc
	$(PNACL_FINALIZE) -o $@ $<


guard-%:
	@if [ "${${*}}" == "" ]; then \
	  echo "***Environment variable $* not set***"; \
	  exit 1; \
	fi

#
# Makefile target to run the SDK's simple HTTP server and serve this example.
#
HTTPD_PY := python ./tools/httpd.py

.PHONY: serve
serve: all
	$(HTTPD_PY) -C $(CURDIR)
