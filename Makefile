MAJOR_VERSION=0
VERSION=$(MAJOR_VERSION).0.1
NAME=drawlib


TBASE=lib$(NAME).so
SONAME=$(TBASE).$(MAJOR_VERSION)
TARGET=lib$(NAME).so.$(VERSION)

SRCDIR = src
OBJDIR = build

PLATFORM=$(shell uname -m)
CC = gcc
CFLAGS += -Wall -Wextra
CFLAGS += -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align
CFLAGS += -Wstrict-prototypes -Wwrite-strings -ftrapv
#CFLAGS += -fsanitize=address
CFLAGS += -fPIC
#CFLAGS += -march=native
#CFLAGS += -pthread
#SFLAGS = -std=gnu99 -pedantic
LDFLAGS += -shared -Wl,-soname,$(SONAME)
INCLUDES = -I.
LIBS = -lEGL -lGLESv2
ifeq ($(PLATFORM), armv6l)
	SRCS = $(wildcard $(SRCDIR)/*_pi.c)
	INCLUDES += -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads
	LDFLAGS += -L/opt/vc/lib
	LIBS += -lbcm_host
else
	SRCS = $(wildcard $(SRCDIR)/*_x11.c)
	LIBS += -lX11
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

uninstall:
	$(RM) "$(LIBDIR)/$(TARGET)"
	$(RM) "$(LIBDIR)/$(STARGET)"
	$(RM) "$(LIBDIR)/$(TBASE)"
	$(RM) "$(LIBDIR)/$(SONAME)"
	$(RM) "$(INCDIR)/$(NAME).h"

clean:
	$(RM) $(OBJS) $(TARGET) $(STARGET)

.PHONY: all debug clean install uninstall