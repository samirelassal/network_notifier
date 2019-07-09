all: notify

notify: notify.c
	gcc -g -Wall notify.c -o notify