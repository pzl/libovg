MAJOR_VERSION=0
VERSION=$(MAJOR_VERSION).0.1
NAME=ovg


TBASE=lib$(NAME).so
SONAME=$(TBASE).$(MAJOR_VERSION)
TARGET=lib$(NAME).so.$(VERSION)

SRCDIR = src
OBJDIR = build

PLATFORM=$(shell uname -m)
CC = gcc
CFLAGS += -Wall -Wextra
CFLAGS += -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-align
CFLAGS += -Wwrite-strings -ftrapv
#CFLAGS += -fsanitize=address
CFLAGS += -fPIC
#CFLAGS += -march=native
#CFLAGS += -pthread
SFLAGS = -std=gnu99 -pedantic
LDFLAGS += -shared -Wl,-soname,$(SONAME)
INCLUDES = -I.
SRCS = $(filter-out %_pi.c %_x11.c,$(wildcard $(SRCDIR)/*.c))
ifeq ($(PLATFORM), armv6l)
	CFLAGS += -DPI
	SRCS += $(wildcard $(SRCDIR)/*_pi.c)
	INCLUDES += -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
	LDFLAGS += -L/opt/vc/lib
	LIBS += -lbcm_host -lEGL -lGLESv2
else
	SRCS += $(wildcard $(SRCDIR)/*_x11.c)
	LDFLAGS += -Wl,--no-as-needed
	LIBS += -lX11 -lGL -lGLU -lOpenVG
endif
OBJS=$(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
HEAD=$(wildcard $(SRCDIR)/*.h) $(NAME).h
STARGET = lib$(NAME).a

PREFIX ?= /usr
DEST = $(DESTDIR)$(PREFIX)
LIBDIR = $(DEST)/lib
INCDIR = $(DEST)/include



all: CFLAGS += -Os
all: LDFLAGS += -s
all: static shared

debug: CFLAGS += -O0 -g -DDEBUG
debug: static shared

static: $(STARGET)
shared: $(TARGET)


$(OBJS): Makefile

dummy := $(shell test -d $(OBJDIR) || mkdir -p $(OBJDIR))


$(TARGET): $(OBJS) $(HEAD)
	$(CC) $(CFLAGS) $(SFLAGS) $(INCLUDES) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(SFLAGS) $(INCLUDES) -c -o $@ $< $(LDFLAGS) $(LIBS)

$(STARGET): $(OBJS) $(HEAD)
	ar rcs $(STARGET) $(OBJS)


install:
	install -D -m 755 $(TARGET) "$(LIBDIR)/$(TARGET)"
	install -D -m 755 $(STARGET) "$(LIBDIR)/$(STARGET)"
	install -D -m 644 $(NAME).h "$(INCDIR)/$(NAME).h"
	ln -srf "$(LIBDIR)/$(TARGET)" "$(LIBDIR)/$(TBASE)"
	ln -srf "$(LIBDIR)/$(TARGET)" "$(LIBDIR)/$(SONAME)"

install-python:
	$(MAKE) -C lib/python install

install-python2:
	$(MAKE) -C lib/python py2

uninstall:
	$(RM) "$(LIBDIR)/$(TARGET)"
	$(RM) "$(LIBDIR)/$(STARGET)"
	$(RM) "$(LIBDIR)/$(TBASE)"
	$(RM) "$(LIBDIR)/$(SONAME)"
	$(RM) "$(INCDIR)/$(NAME).h"
	$(MAKE) -C lib/python uninstall

test:
	$(CC) -o test tests/main.c -lovg
	$(CC) -o reopen tests/reopen.c -lovg

clean:
	$(RM) $(OBJS) $(TARGET) $(STARGET) test reopen

.PHONY: all debug clean install uninstall test install-python install-python2
