GPP:=g++
GCC:=gcc
RM:=rm

PROGRAM:=sunfish
SOURCES:=src/sunfish.cpp \
$(wildcard ./src/*/*.cpp)
#$(wildcard ./src/Shogi/*.cpp) \
$(wildcard ./src/Shogi/*/*.cpp) \
$(wildcard ./src/Evaluates/*.cpp) \
$(wildcard ./src/Search/*.cpp) \
$(wildcard ./src/Csa/*.cpp) \
$(wildcard ./src/Records/*.cpp) \
$(wildcard ./src/Cui/*.cpp) \
$(wildcard ./src/Network/*.cpp) \
$(wildcard ./src/Books/*.cpp) \
$(wildcard ./src/Util/*.cpp) \
$(wildcard ./src/Tools/*.cpp) \
$(wildcard ./src/Log/*.cpp) \
$(wildcard ./src/Test/*.cpp)
OBJECTS:=$(SOURCES:.cpp=.o)
DEPENDS:=$(SOURCES:.cpp=.d)

ATTACK_TABLE=attackTable
ATTACK_TABLE_SRC=src/Shogi/Attack/attackTable.cpp
OBJECTS+=$(ATTACK_TABLE_SRC:.cpp=.o)
CHECK_TABLE=checkTable
CHECK_TABLE_SRC=src/Shogi/Attack/checkTable.cpp
OBJECTS+=$(CHECK_TABLE_SRC:.cpp=.o)
OBJECTS:=$(sort $(OBJECTS))

DEV_PROGRAMS:=$(ATTACK_TABLE) $(CHECK_TABLE)

override CFLAGS+=-DPOSIX
override CFLAGS+=-Wall
override CFLAGS+=-W
override CFLAGS+=-O2
#override CFLAGS+=-O3
#override CFLAGS+=-DNDEBUG
override CFLAGS+=-DNLEARN
#override CFLAGS+=-DPRUN_EXPR
#override CFLAGS+=-g
#override CFLAGS+=-pg
override LIBS+=-lrt
#override LIBS+=-lboost_thread-mt
#override LIBS+=-lboost_regex-mt
#override LIBS+=-lboost_program_options-mt
override LIBS+=-lboost_thread
override LIBS+=-lboost_regex
override LIBS+=-lboost_program_options
override LIBS+=-lboost_system
override LIBS+=-lboost_filesystem

.PHONY: all pgo

all: $(PROGRAM) $(TEST) $(CLEAN_SH)

pgo:
	$(MAKE) clean; $(MAKE) $(PROGRAM) 'CFLAGS+=-fprofile-generate'
	./$(PROGRAM) -b -w -q
	$(MAKE) clean; $(MAKE) $(PROGRAM) 'CFLAGS+=-fprofile-use'
	$(MAKE)

$(PROGRAM): $(OBJECTS)
	$(GPP) -o $(PROGRAM) $(CFLAGS) $^ $(LIBS)

$(ATTACK_TABLE) : src/attackTable.cpp \
src/Shogi/squareDiff.o src/Shogi/piece.o \
src/Shogi/directionFlags.o src/Log/logger.cpp \
src/Develop/attackTableGenerator.o
	$(GPP) -o $@ $(CFLAGS) $^ $(LIBS)

$(ATTACK_TABLE_SRC) : $(ATTACK_TABLE)
	./$(ATTACK_TABLE) $@

$(CHECK_TABLE) : src/checkTable.cpp \
src/Shogi/squareDiff.o src/Shogi/piece.o \
src/Shogi/directionFlags.o src/Log/logger.cpp \
src/Develop/checkTableGenerator.o \
src/Shogi/Attack/attackTable.o
	$(GPP) -o $@ $(CFLAGS) $^ $(LIBS)

$(CHECK_TABLE_SRC) : $(CHECK_TABLE)
	./$(CHECK_TABLE) $@

.cpp.o:
	$(GPP) $(CFLAGS) -o $@ -c $<

%.d: %.cpp
	@$(SHELL) -c '$(CC) -MM $(CFLAGS) $< | sed "s|^.*:|$*.o $@:|g" > $@; [ -s $@ ] || rm -f $@'

clean:
	$(RM) $(PROGRAM) $(OBJECTS) $(DEPENDS) $(DEV_PROGRAMS) src/*.o src/*.d

-include $(DEPENDS)
