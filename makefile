# SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
# Copyright (c) 2009-2026 John R. Smythe Jr.
#
# sys — sources are the linux-x86_64 variant (see VARIANT).
# SYS_VARIANT defaults to linux-x86_64 in rules.mak. Not auto-detected.
#
include		../rules.mak
ifneq ($(SYS_VARIANT),linux-x86_64)
$(error sys: unsupported SYS_VARIANT=$(SYS_VARIANT); this repo ships linux-x86_64 only)
endif
CHDRS =	$(HDRS:%.h=$(HDR)/%.h)
LIBRARIES =
include		../targets.mak
include	.depend
