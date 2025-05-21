##
## EPITECH PROJECT, 2025
## plazza
## File description:
## base-config
##

/ ?= ./
BUILD := $/.build

CC := gcc
CXX := g++
LD := $(CC)
AR ?= ar
RM ?= rm --force

CXXFLAGS := -std=c++20
CXXFLAGS += -iquote $/libs -iquote $/include
CXXFLAGS += $(shell cat $/warning_flags.txt)

CXXFLAGS_release := -O2 -fomit-frame-pointer

# Fix false positive of `-Wfree-nonheap-object`
# caused by `[[nodiscard]] auto get() const -> ErrorOr<T>` in JSONValue.hpp
# See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=108088
CXXFLAGS_release += -fno-inline-small-functions

CXXFLAGS_bonus := $(CXXFLAGS_release) -DBONUS=1 -fanalyzer -DDEBUG_MODE
CXXFLAGS_bonus += -g3 -fsanitize=address,leak,undefined
CXXFLAGS_debug := -g3 -fsanitize=address,leak,undefined -DDEBUG_MODE=1
CXXFLAGS_tests := --coverage -g3

LDLIBS :=
LDFLAGS :=

DEPS_FLAGS ?= -MMD -MP

#MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-print-directory
