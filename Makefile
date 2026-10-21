CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -O2
X11_CFLAGS ?= $(shell pkg-config --cflags x11)
X11_LIBS ?= $(shell pkg-config --libs x11)

NAVAL_SOURCE = import/extracted/C\ Games/naval_battle_graduation_work-main/batalha_naval.c
TRUNFO_SOURCE = import/extracted/C\ Games/super_trunfo_graduation_work-main/super_trunfo.c
CHESS_SOURCE = import/extracted/C\ Games/xadrez_graduation_work-main/xadrez.c

.PHONY: all run clean

all: build/c_games_menu build/batalha_naval build/super_trunfo build/xadrez

build:
	mkdir -p build

build/c_games_menu: src/launcher.c | build
	$(CC) $(CFLAGS) $(X11_CFLAGS) -o $@ $< $(X11_LIBS)

build/batalha_naval: $(NAVAL_SOURCE) | build
	$(CC) $(CFLAGS) -o $@ 'import/extracted/C Games/naval_battle_graduation_work-main/batalha_naval.c'

build/super_trunfo: $(TRUNFO_SOURCE) | build
	$(CC) $(CFLAGS) -o $@ 'import/extracted/C Games/super_trunfo_graduation_work-main/super_trunfo.c' -lm

build/xadrez: $(CHESS_SOURCE) | build
	$(CC) $(CFLAGS) -o $@ 'import/extracted/C Games/xadrez_graduation_work-main/xadrez.c'

run: all
	./build/c_games_menu

clean:
	rm -rf build