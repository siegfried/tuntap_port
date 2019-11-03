#include <stdlib.h>
#include <stdio.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <err.h>
#include <netinet/ip.h>
#include "port.h"

#define BUFFER_SIZE 65535

int
tuntap_open(const char *devname) {
        char *directory = "/dev/";
        char *path;
        int fd;
        if ((path = malloc(strlen(directory) + strlen(devname) + 1)) == NULL) {
                err(1, NULL);
                exit(1);
        }
        strcpy(path, directory);
        strcat(path, devname);
        if ((fd = open(path, O_RDWR)) == -1) {
                perror("CANNOT OPEN TUN/TAP DEVICE");
                exit(1);
        }
        free(path);
        return fd;
}

void *
tuntap_read(void *fd)
{
        ssize_t nr;
        u_char buffer[BUFFER_SIZE];
        struct iovec iov[2];
        u_int32_t af;

        iov[0].iov_base = &af;
        iov[0].iov_len = sizeof(af);
        iov[1].iov_base = buffer;
        iov[1].iov_len = BUFFER_SIZE;

        while ((nr = readv(* (int *) fd, iov, 2)) != -1 && nr != 0) {
                af = ntohl(af);
                if (af != AF_INET && af != AF_INET6) {
                        err(47, "STDOUT ERROR");
                        exit(1);
                }
                if (write_packet(buffer, nr - iov[0].iov_len) == -1) {
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
        struct iovec iov[2];
        u_int32_t af;
        struct ip *iph;

        iov[0].iov_base = &af;
        iov[0].iov_len = sizeof(af);
        iov[1].iov_base = buffer;

        while ((nr = read_packet(buffer)) != -1 && nr != 0) {
                iov[1].iov_len = nr;
                iph = (struct ip *) buffer;

                if (iph -> ip_v == 6) {
                        af = htonl(AF_INET6);
                }
                else {
                        af = htonl(AF_INET);
                }

                if (writev(* (int *) fd, iov, 2) == -1) {
                        perror("TUN/TAP WRITE ERROR");
                }
        }

        if (nr == -1) {
                perror("STDIN ERROR");
        }

        exit(1);
}
