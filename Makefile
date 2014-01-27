CC := g++
LDFLAGS := -fopenmp -lm
CFLAGS  := -g -Wall -fopenmp

SRCS := $(wildcard *.cpp)
PRGS := $(patsubst %.cpp,%,$(SRCS))
RULE_SUFFIX :=.rule
BINS := $(patsubst %,%$(RULE_SUFFIX),$(PRGS))
OBJS := $(patsubst %,%.o,$(PRGS))

all: $(BINS)

.SECONDEXPANSION:
OBJ = $(patsubst %$(RULE_SUFFIX),%.o,$@)
BIN = $(patsubst %$(RULE_SUFFIX),%,$@)

%$(RULE_SUFFIX): $(OBJS)
	$(CC) $(OBJ) $(LDFLAGS) -o $(BIN)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
	

clean:
	$(RM) $(PRGS)

rebuild: clean all
