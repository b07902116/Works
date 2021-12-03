from scapy.all import *
import sys
import random
import time

def_ttl = 64


def randomIP(host_num):
    x = random.randint(1, host_num)
    return "10.0.0.{:d}".format(x)

def randomttl(host_num):
    HC = random.randint(1, 30)
    return def_ttl - HC

def send_learning_packet(hostIP, serverIP):
    IPsrc = hostIP
    IPdst = serverIP
    IPttl = def_ttl
    TCPsrc = 5566
    TCPdst = 1234
    payload = "learning"
    packet = IP(src = IPsrc, dst = IPdst, ttl = IPttl) / TCP(sport = TCPsrc, dport = TCPdst)
    packet.add_payload(payload)
    send(packet, verbose = 0)


def send_valid_packet(hostIP, serverIP):
    IPsrc = hostIP
    IPdst = serverIP
    IPttl = def_ttl
    TCPsrc = 5566
    TCPdst = 1234
    payload = "valid"
    packet = IP(src = IPsrc, dst = IPdst, ttl = IPttl) / TCP(sport = TCPsrc, dport = TCPdst)
    packet.add_payload(payload)
    send(packet, verbose = 0)


def send_spoofing_packet(hostIP, serverIP, host_num):
    IPsrc = randomIP(host_num)
    while IPsrc == hostIP or IPsrc == serverIP:
        IPsrc = randomIP(host_num)
    IPdst = serverIP
    # Use random ttl in range [1, 30], spoofing IP and HC
    # IPttl = randomttl(host_num)
    # Use host's ttl, only spoofing IP
    IPttl = def_ttl
    TCPsrc = 5566
    TCPdst = 1234
    payload = "spoofing"
    packet = IP(src = IPsrc, dst = IPdst, ttl = IPttl) / TCP(sport = TCPsrc, dport = TCPdst)
    packet.add_payload(payload)
    send(packet, verbose = 0)



def main(host_index, server_index, host_num):
    hostIP = "10.0.0.{:d}".format(host_index)
    serverIP = "10.0.0.{:d}".format(server_index)
    print("start sending learning packet")
    for _ in range(10):
        send_learning_packet(hostIP, serverIP)
        time.sleep(2)
    count = 0
    print("start sending spoofing packet")
    while True:
        # randomly send valid/spoofing packet
        if random.randint(1, 100) <= 50:
            send_valid_packet(hostIP, serverIP)
        else:
            send_spoofing_packet(hostIP, serverIP, host_num)
        count += 1
        time.sleep(0.1)



if __name__ == "__main__":
    if len(sys.argv) != 4:
        sys.exit("usage: python spoofing.py [host_index] [server_index] [host_num]")
    main(int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]))
