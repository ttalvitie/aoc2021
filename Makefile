CXX ?= g++
CFLAGS_COMMON := -std=c++17 -Wall -Wextra
CFLAGS_debug := $(CFLAGS_COMMON) -g -O0
CFLAGS_release := $(CFLAGS_COMMON) -O3 -DNDEBUG
LDFLAGS_COMMON :=
LDFLAGS_debug := $(LDFLAGS_COMMON)
LDFLAGS_release := $(LDFLAGS_COMMON)
SRCS := $(shell find src -name "*.cpp")

.PHONY: debug release clean default

default: debug

define OUTDEFS
OBJS_$(1) := $(SRCS:%.cpp=obj/$(1)/%.o)
DEPS_$(1) := $(SRCS:%.cpp=obj/$(1)/%.d)
endef
$(foreach b,debug release,$(eval $(call OUTDEFS,$(b))))

define BUILDRULES

$(1): aoc2021.$(1)

aoc2021.$(1): $(OBJS_$(1))
	$(CXX) $(CFLAGS_$(1)) $(OBJS_$(1)) -o aoc2021.$(1) $(LDFLAGS_$(1))

endef
$(eval $(call BUILDRULES,debug))
$(eval $(call BUILDRULES,release))

define OBJRULE
$(2:%.cpp=obj/$(1)/%.o): $(2)
	@mkdir -p `dirname $(2:%.cpp=obj/$(1)/%.o)`
	$(CXX) $(CFLAGS_$(1)) -Isrc -MMD -c $(2) -o $(2:%.cpp=obj/$(1)/%.o)
endef
$(foreach s,$(SRCS),$(eval $(call OBJRULE,debug,$(s))))
$(foreach s,$(SRCS),$(eval $(call OBJRULE,release,$(s))))

clean:
	rm -rf $(OBJS_debug) $(OBJS_release) $(DEPS_debug) $(DEPS_release) aoc2021.debug aoc2021.release

-include $(DEPS_debug) $(DEPS_release)
