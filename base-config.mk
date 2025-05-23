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

CXXFLAGS += $(shell grep -vP '^(#|$$)' ./warning_flags.conf)
# Do not listen to bad hightlighter  ^

CXXFLAGS_release := -O2 -fomit-frame-pointer

CXXFLAGS_bonus := $(CXXFLAGS_release) -DBONUS=1 -fanalyzer -DDEBUG_MODE
CXXFLAGS_bonus += -g3 -fsanitize=address,leak,undefined
CXXFLAGS_debug := -g3 -fsanitize=address,leak,undefined -DDEBUG_MODE=1
CXXFLAGS_tests := --coverage -g3

LDLIBS :=
LDFLAGS :=

DEPS_FLAGS ?= -MMD -MP

MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-print-directory
