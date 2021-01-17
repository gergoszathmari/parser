.PHONY = all clean
target := parser

INCLDIR = include
SRCDIR = src
OBJDIR = obj

CC = gcc
CFLAGS = -Wall -I$(INCLDIR) 
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -I$(INCLDIR)
LDFLAGS = -g -lwt -lwthttp

LEXFLS := cparser.l
YACCFLS := cparser.y
SRCSC := $(SRCDIR)/y.tab.c $(SRCDIR)/lex.yy.c
SRCSCXX := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(addprefix $(OBJDIR)/, $(notdir $(SRCSC:.c=.o))) $(addprefix $(OBJDIR)/, $(notdir $(SRCSCXX:.cpp=.o)))
DEPS := $(patsubst %.o, %.d, $(OBJS))

DEPFLAGS = -MMD -MF $(@:.o=.d)

yellow:=$(shell tput setaf 3)
green:=$(shell tput setaf 2)
reset:=$(shell tput sgr0)


all: post-build

post-build: $(target)
	$(info $(green)Build completed!$(reset))

parser: $(OBJS) 
	$(info $(yellow)Linking program...$(reset))
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(info $(yellow)Compiling C++ sources...$(reset))
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(DEPFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(info $(yellow)Compiling C sources...$(reset))
	$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

$(SRCDIR)/lex.yy.c: $(LEXFLS)
	$(info $(yellow)Creating lex definitions...$(reset))
	lex cparser.l
	mv lex.yy.c $(SRCDIR)/lex.yy.c

$(SRCDIR)/y.tab.c: $(YACCFLS)
	$(info $(yellow)Creating yacc definitions...$(reset))
	bison -d -v -y cparser.y
	mv y.tab.c $(SRCDIR)/y.tab.c
	mv y.tab.h $(INCLDIR)/y.tab.h

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	$(info $(yellow)Cleaning up...$(reset))
	rm -rvf $(target) $(OBJS) $(DEPS) $(SRCSC) $(INCLDIR)/*.tab.*

-include $(DEPS)