#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "port.h"
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#define BUFFER_SIZE 65535

int
tuntap_open(const char *devname)
{
        struct ifreq ifr;
        int fd;

        if ((fd = open("/dev/net/tun", O_RDWR)) == -1) {
                perror("OPEN /dev/net/tun");
                exit(1);
        }
        memset(&ifr, 0, sizeof(ifr));
        ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
        strncpy(ifr.ifr_name, devname, IFNAMSIZ);

        if (ioctl(fd, TUNSETIFF, (void *) &ifr) == -1) {
                perror("ioctl TUNSETIFF");
                close(fd);
                exit(1);
        }

        return fd;
}

void *
tuntap_read(void *fd)
{
        ssize_t nr;
        unsigned char buffer[BUFFER_SIZE];

        while ((nr = read(* (int *) fd, buffer, sizeof(buffer))) != -1 && nr != 0) {
                if (write_packet(buffer, nr) != nr) {
                        perror("STDOUT ERROR");
                        exit(1);
                }
        }

        if (nr == -1) {
                perror("TUN/TAP READ ERROR");
        }

        exit(1);
}

void *
tuntap_write(void *fd)
{
        ssize_t nr;
        u_char buffer[BUFFER_SIZE];

        while ((nr = read_packet(buffer)) != -1 && nr != 0) {
                if (write(* (int *) fd, buffer, nr) != nr) {
                        perror("TUN/TAP WRITE ERROR");
                        exit(1);
                }
        }

        if (nr == -1) {
                perror("STDIN ERROR");
        }

        exit(1);
}
