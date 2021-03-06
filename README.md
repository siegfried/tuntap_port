# Tun/Tap port for Erlang/Elixir

This is a port written in C to work with Tun/Tap devices in Erlang/Elixir applications, as Erlang/Elixir cannot open special files such as devices. Other languages should be able to use it as well, because it is just a simple program printing outgoing packets to standard output and receiving incoming packets from standard input concurrently, with 2 bytes as the packet length prepended.

It should be able to work on any OS supports Tun/Tap devices, but I have only tested it on OpenBSD 6.5, OpenBSD 6.6 and CentOS 15 hitherto.

## Setup a Tun/Tap device

*This project does not have the feature to create Tun/Tap device*, as there are already some system commands(require root) to do that.

### OpenBSD

It's quite easy to create a Tun/Tap device on OpenBSD with [ifconfig(8)](https://man.openbsd.org/ifconfig.8):

```shell
ifconfig tun0 create
ifconfig tun0 inet 10.0.0.1 10.0.0.2 netmask 255.255.255.255
```

or [hostname.if(5)](https://man.openbsd.org/hostname.if.5):

Create file `/etc/hostname.tun0`:

```
inet 10.0.0.1 255.255.255.255 NONE
dest 10.0.0.2
```

and run:

```shell
sh /etc/netstart
```

### CentOS

```shell
ip tuntap add tun0 mode tun
ifconfig tun0 10.0.0.1 dstaddr 10.0.0.2 up
```

## Build

This project tries to build on any system without a dependency, even `gmake` which needs to be installed on OpenBSD.

### OpenBSD

```shell
make openbsd
```

### Linux

```shell
make linux
```

### Others

They are not supported yet, but should not be hard.

## Usage

To open a Tun/Tap port, run:

```elixir
port = Port.open({:spawn, "tuntap tun0"}, [:binary, {:packet, 2}, :exit_status])
```

then current process will receive messages contain outgoing packets:

```elixir
{#Port<0.1444>, {:data, packet}}
```

to notify the Tun/Tap device a incoming packet:

```elixir
send(port, {self(), {:command, packet}})
```

Please check the [Port document](https://hexdocs.pm/elixir/Port.html) for more information.
