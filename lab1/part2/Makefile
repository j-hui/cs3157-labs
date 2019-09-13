# Copyright (C) 2019 John Hui
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
# USA.

CC=gcc
LD=gcc
VER=6.1.2
CFLAGS=-Igmp-$(VER)
LDLIBS= -lgmp
LDFLAGS= -Lgmp-$(VER)/.libs
OBJS=convert.o

default: convert
convert: $(OBJS) lib
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)
$(OBJS): gmp-$(VER)/gmp.h

lib: gmp-$(VER)/.libs/libgmp.a
gmp-$(VER): gmp-$(VER).tar.xz
	tar xf $^
gmp-$(VER)/gmp-h.in gmp-$(VER)/configure: gmp-$(VER)
gmp-$(VER)/gmp.h: gmp-$(VER)/gmp-h.in gmp-$(VER)/configure
	cd gmp-$(VER) && ./configure
gmp-$(VER)/.libs/libgmp.a: gmp-$(VER) gmp-$(VER)/gmp.h
	cd gmp-$(VER) && make -j$$(nproc)
clean:
	rm -rf *.o main convert a.out gmp-$(VER)
.PHONY: clean default lib
