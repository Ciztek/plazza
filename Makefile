##
## EPITECH PROJECT, 2025
## plazza
## File description:
## Makefile
##

# ↓ Compiler configuration
include base-config.mk

#? (default): Build the arcade
.PHONY: _start
_start: all

every_lib =
every_lib_obj =

# call generic-o-builder, profile
define generic-o-builder

$(BUILD)/$(strip $1)/%.o: %.cpp
	@ mkdir -p $$(dir $$@)
	$$Q $$(COMPILE.cpp) -fPIC \
        -o $$@ -c $$< $$(CXXFLAGS) $$(CXXFLAGS_$(strip $1))
	@ $$(LOG_TIME) "CC $$(C_YELLOW)$(strip $1)$$(C_RESET) \
		$$(C_PURPLE)$$(notdir $$@) $$(C_RESET)"

endef

$(foreach build-mode, debug release bonus cov tests, \
    $(eval $(call generic-o-builder, $(build-mode) )))

every_out :=
every_obj :=

# call mk-bin, bin-name, profile, a-src
define mk-bin

out_$(strip $2) := $(strip $1)

objs__$(strip $2) := $(base-src:%.cpp=$(BUILD)/$(strip $2)/%.o)
lib_objs__$(strip $2) += $(foreach m,$(modules),$(BUILD)/$(strip $2)/lib$m.a)

$$(out_$(strip $2)): LDFLAGS += -L $$(BUILD)/$(strip $2)
$$(out_$(strip $2)): LDLIBS += $(foreach m,$(modules),-l$m)
$$(out_$(strip $2)): $$(objs__$(strip $2)) $$(lib_objs__$(strip $2))
	$$Q $$(CXX) $$(CXXFLAGS) $$(CXXFLAGS_$(strip $2)) \
		-o $$@ $$^ $$(LDLIBS) $$(LDFLAGS)
	@ $$(LOG_TIME) "LD $$(C_GREEN)$$@ $$(C_RESET)"

every_out += $$(out_$(strip $2))
every_obj += $$(objs__$(strip $2))

endef

base-src := $(shell find src -type f -name "*.cpp")
base-src += $(shell find lib -type f -name "*.cpp")

$(eval $(call mk-bin, plazza, release))
$(eval $(call mk-bin, debug, debug))
$(eval $(call mk-bin, bonus, bonus))

CC_JSON := compile_commands.json

ifeq ($(auto-complete-fix),1)
$(error "This should not be set")
bonus debug:
#? bonus: Build the project bonuses
#? debug: Build a developer version
endif

.PHONY: all #? all: Build the project
all: $(every_out)

.PHONY: clean #? clean: Remove generated compiled files
clean:
	$Q $(RM) $(every_lib_obj)
	$Q $(RM) $(every_obj)

.PHONY: fclean #? fclean: Force total recompilation on next build
fclean: clean
	$Q $(RM) $(every_out)
	$Q $(RM) $(every_lib)

.PHONY: mrproper #? mrproper: (almost) Full repository cleanup
mrproper: fclean
	$(RM) -r $(BUILD)
	$(RM) -r .cache result $(CC_JSON)

$(CC_JSON):
	$Q compiledb -- make all


.NOTPARALLEL: re
.PHONY: re
re: fclean all #? re: rebuild the default target

.NOTPARALLEL: compiledb
.PHONY: compiledb
compiledb: #? compiledb: Generathe the compile_commands.json file
compiledb: mrproper $(CC_JSON)

.PHONY: help
help: #? help: Show this help message
	@ grep -P "#[?] " $(MAKEFILE_LIST)          \
      | sed -E 's/.*#\? ([^:]+): (.*)/\1 "\2"/' \
	  | xargs printf "%-12s: %s\n"

V ?= 0
ifneq ($(V),0)
  Q :=
else
  Q := @
endif

ifneq ($(shell command -v tput),)
  ifneq ($(shell tput colors),0)

mk-color = \e[$(strip $1)m

C_BEGIN := \033[A
C_RESET := $(call mk-color, 00)

C_RED := $(call mk-color, 31)
C_GREEN := $(call mk-color, 32)
C_YELLOW := $(call mk-color, 33)
C_BLUE := $(call mk-color, 34)
C_PURPLE := $(call mk-color, 35)
C_CYAN := $(call mk-color, 36)

  endif
endif

NOW = $(shell date +%s%3N)

ifndef STIME
STIME := $(call NOW)
endif

TIME_NS = $(shell expr $(call NOW) - $(STIME))
TIME_MS = $(shell expr $(call TIME_NS))

BOXIFY = "[$(C_BLUE)$(1)$(C_RESET)] $(2)"

ifneq ($(shell command -v printf),)
  LOG_TIME = printf $(call BOXIFY, %6s , %b\n) "$(call TIME_MS)"
else
  LOG_TIME = echo -e $(call BOXIFY, $(call TIME_MS) ,)
endif

-include ./hook.mk
