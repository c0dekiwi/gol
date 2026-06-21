// Conway's Game Of Life

#define _POSIX_C_SOURCE 199309L
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef char* cstr;
typedef uint8_t u8;
#define ALIVE '#'
#define DEAD ' '
#define GOLS 20
u8 GOL[GOLS][GOLS] = {0};

int gol_strlen(cstr str) {
	int len;
	if (!str) return 0;
	for (len = 0; len < GOLS*GOLS && str[len]; len++);
	return len;
}

u8 gol_init(int xlen, cstr state) {
	int len;
	int i;
	int x;
	int y;

	for (y = 0; y < GOLS; y++)
	for (x = 0; x < GOLS; x++)
		GOL[y][x] = DEAD;

	len = gol_strlen(state);
	if (!xlen || !len) return 0;
	if (len >= GOLS*GOLS) return 1;
	if (len % xlen) return 2;

	y = -1;
	x = 0;
	for (i = 0; i < len; i++) {
		if (i && !(i % xlen)) x = 0;
		y += !x;
		if (state[i] == '#') GOL[y][x] = ALIVE;
		else GOL[y][x] = DEAD;
		x++;
	}
	return 0;
}

u8 gol_count_neighbour(int x, int y) {
	int n;
	int dx;
	int dy;

	n = 0;
	for (dy = -1; dy <= 1; dy++) {
		for (dx = -1; dx <= 1; dx++) {
			if (!dx && !dy) continue;
			if (0 <= x + dx && x + dx < GOLS && 0 <= y + dy && y + dy < GOLS) {
				n += GOL[y + dy][x + dx] == ALIVE;
			}
		}
	}
	return n;
}

void gol_next() {
	u8 next[GOLS][GOLS];
	int x;
	int y;
	u8 nhb;

	for (y = 0; y < GOLS; y++) {
		for (x = 0; x < GOLS; x++) {
			nhb = gol_count_neighbour(x, y);
			if (nhb == 3 || (GOL[y][x] == ALIVE && nhb == 2))
				next[y][x] = ALIVE;
			else
				next[y][x] = DEAD;
		}
	}
	for (y = 0; y < GOLS; y++) {
		for (x = 0; x < GOLS; x++) {
			GOL[y][x] = next[y][x];
		}
	}
}

void gol_print(int gen) {
	int y;
	printf("\033[2J");
	printf("\033[%d,%d", 0, 0);
	printf("%d\n", gen);
	for (y = 0; y < GOLS; y++) printf("|%.*s|\n", GOLS, GOL[y]);
}

int main(int ac, char **av) {
	u8 err;

	if (ac > 2) {
		err = gol_init(atoi(av[1]), av[2]);
	}
	else {
		err = gol_init(3, "..##.#.##");
//		err = gol_init(3, "....##.##");
	}
	if (err) {
		fprintf(stderr, "Failed initialisation [%d]\n", err);
		return 1;
	}
	for (int i = 0; i <= 20; i++) {
		gol_print(i);
		gol_next();
		nanosleep(&(struct timespec){ .tv_nsec=100'000'000 }, NULL);
	}

	return 0;
}
