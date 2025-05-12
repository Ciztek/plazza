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

CFLAGS := -std=c2x -pedantic
CFLAGS += -iquote $/lib/parsing -iquote $/include
CFLAGS += $(shell cat $/warning_flags.txt)

CXXFLAGS := -std=c++20
CXXFLAGS += -iquote $/lib/parsing -iquote $/include
CXXFLAGS += $(shell cat $/warning_flags.txt)

CFLAGS_release := -O2 -DNEBUG -fomit-frame-pointer
CFLAGS_bonus := $(CFLAGS_release) -DBONUS=1 -fanalyzer -DDEBUG_MODE
CFLAGS_bonus += -g3 -fsanitize=address,leak,undefined
CFLAGS_debug := -g3 -fsanitize=address,leak,undefined -DDEBUG_MODE=1
CFLAGS_tests := --coverage -g3

CXXFLAGS_release := -O2 -DNEBUG -fomit-frame-pointer
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
