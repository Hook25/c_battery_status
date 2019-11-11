build:
	gcc -O3 -pedantic-errors -Wall `pkg-config --cflags --libs libnotify` battery_status.c -o battery_status

clean:
	rm battery_status
