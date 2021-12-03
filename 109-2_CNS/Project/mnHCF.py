from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import RemoteController
from mininet.cli import CLI
import sys
import math

class Mytopo(Topo):
    def __init__(self, height):
        super(Mytopo, self).__init__()

        h_num = 2 ** height
        # Add hosts and switches
        hosts = [self.addHost("h{:d}".format(i + 1)) for i in range(h_num)]
        switches = [self.addSwitch("s{:d}".format(i + 1)) for i in range(h_num - 1)]
        # Link between switches
        for i in range(0, 2 ** (height - 1) - 1):
            self.addLink(switches[i], switches[i * 2 + 1])
            self.addLink(switches[i], switches[i * 2 + 2])
        # Link between switch and host
        index = 0
        for i in range(2 ** (height - 1) - 1, 2 ** height - 1):
            self.addLink(switches[i], hosts[index])
            self.addLink(switches[i], hosts[index + 1])
            index += 2


def tmp_sh(server_index):
    cmd = "xterm h{:d}".format(server_index)
    f = open("tmp.sh", "w")
    f.write(cmd)
    f.close()
    return "tmp.sh"


def main(host_num, server_index):
    height = math.log(host_num, 2)
    if not height.is_integer():
        sys.exit("host_num should be power of 2")
    height = int(height)
    c = RemoteController('c', '0.0.0.0', 6633)
    net = Mininet(topo = Mytopo(height), controller = None)
    net.addController(c)
    net.start()

    # In the first 10 seconds, hosts only send learning packet
    # Server should start run in the duration and learn IPtoHC
    # Hosts then start to send spoofing / valid packets
    # All hosts run in background
    host_num = 2 ** height
    for i in range(host_num):
        if (i + 1) == server_index:
            continue
        hnode = net.get("h{:d}".format(i + 1))
        hnode.cmd("python host.py {:d} {:d} {:d} &".format(i + 1, server_index, host_num))
        print("h{:d} starts sending packet".format(i + 1))
    
    # Server's xterm will pop up
    # run "python server.py [server_index]" in server's xterm
    CLI(net, script = tmp_sh(server_index))
    CLI(net)
    net.stop()

if __name__ == '__main__':
    if len(sys.argv) != 3:
        sys.exit("usage: sudo python mnHCF.py [host_num] [server_index]")
    main(int(sys.argv[1]), int(sys.argv[2]))
