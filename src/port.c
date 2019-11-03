#include <unistd.h>
#include <arpa/inet.h>

int
read_packet(u_char *buffer)
{
        ssize_t nr;
        uint16_t packet_size;

        if ((nr = read(STDIN_FILENO, &packet_size, 2)) != 2) {
                return nr;
        }

        packet_size = ntohs(packet_size);

        return read(STDIN_FILENO, buffer, packet_size);
}

int
write_packet(u_char *buffer, uint16_t packet_size)
{
        ssize_t nr;
        uint16_t _packet_size;

        _packet_size = htons(packet_size);

        if ((nr = write(STDOUT_FILENO, &_packet_size, 2)) != 2) {
                return nr;
        }

        return write(STDOUT_FILENO, buffer, packet_size);
}
