CFLAGS = -O3 -Wall -Wextra -Wno-unused-parameter -pthread

.PHONY: clean prepare

clean:
	rm -rf _build/tuntap

prepare:
	mkdir -p _build

utuntap: prepare src/port.c src/utuntap.c src/main.c
	cc $(CFLAGS) -o _build/tuntap src/port.c src/utuntap.c src/main.c

tuntap: prepare src/port.c src/tuntap.c src/main.c
	cc $(CFLAGS) -o _build/tuntap src/port.c src/tuntap.c src/main.c
