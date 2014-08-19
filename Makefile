CFLAGS  = -nostdlib -Os -Wall -Wextra -Werror
LDFLAGS = -nostdlib
SLEEPY  = sleepy.o
TOOLS   = test echo cat

all: $(TOOLS)

test: $(SLEEPY) test.o

echo: $(SLEEPY) echo.o

cat: $(SLEEPY) cat.o

.PHONY: clean all

clean:
	$(RM) *.o $(TOOLS)

