from scapy.all import *
import sys
import random
import numpy as np


IPtoHC = {}
def_ttl = 64
server_index = 0
serverIP = ""
accept_num = 0
reject_num = 0
false_positive = 0
false_negative = 0



def init(index):
    global server_index
    server_index = index
    global serverIP
    serverIP = "10.0.0.{:d}".format(server_index)
    global accept_num
    accept_num = 0
    global reject_num
    reject_num = 0




def show_statistic():
    global server_index
    f = open("statistic{:d}.txt".format(server_index), "w")
    f.write("accept:{:d}\n".format(accept_num))
    f.write("reject:{:d}\n".format(reject_num))
    f.write("false positive:{:d}\n".format(false_positive))
    f.write("false negative:{:d}\n".format(false_negative))
    f.close()

    print("accept:{:d}".format(accept_num))
    print("reject:{:d}".format(reject_num))
    print("false positive:{:d}".format(false_positive))
    print("false negative:{:d}".format(false_negative))





def main(InPacket):
    try:
        global accept_num
        global reject_num
        global false_positive
        global false_negative
        pktParsed = Ether(str(InPacket))
        IPsrc = pktParsed[IP].src
        IPdst = pktParsed[IP].dst
        ttl = pktParsed[IP].ttl
        HC = def_ttl - ttl
        # ignore packet sending out
        if IPsrc == serverIP:
            return

        payload = str(pktParsed[TCP].payload)

        print("IPsrc:{:s}, IPdst:{:s}".format(IPsrc, IPdst))
        IPtoHC.setdefault(IPsrc, 0)

        # Learning valid HC
        if "learning" in payload:
            IPtoHC[IPsrc] = HC
            print("Learn packet from {:s} with hop count = {:d}".format(IPsrc, HC))
            return

        # Filtering packet
        if HC == IPtoHC[IPsrc]:
            print("Accept packet from {:s} with hop count = {:d}".format(IPsrc, HC))
            accept_num += 1
            if "spoof" in payload:
                false_negative += 1
        else:
            print("Reject packet from {:s} with hop count = {:d}".format(IPsrc, HC))
            reject_num += 1
            if "valid" in payload:
                false_positive += 1
        if accept_num + reject_num >= 10000:
            show_statistic()
            sys.exit("finish")
    except Exception as e:
        return




if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit("usage python server.py [server_index]")
    init(int(sys.argv[1]))
    sniff(prn=lambda x: main(x))
    show_statistic()

