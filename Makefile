build:
	gcc -O3 -pedantic-errors -Wall `pkg-config --cflags --libs libnotify` battery_status.c -o battery_status

debug:
	gcc -DDEBUG -O0 -g -pedantic-errors -Wall `pkg-config --cflags --libs libnotify` battery_status.c -o battery_status

clean:
	rm battery_status

.PHONY:build debug clean
