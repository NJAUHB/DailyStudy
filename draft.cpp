#include <iostream>
#include <ifaddrs.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <iomanip>



void get_mac_address(uint8_t* mac_address) {
    ifaddrs* if_list = nullptr;
    ifaddrs* ifa = nullptr;

    getifaddrs(&if_list);
    for (ifa = if_list; ifa != nullptr; ifa = ifa->ifa_next){
        if (ifa->ifa_addr->sa_family == AF_PACKET && !(ifa->ifa_flags & IFF_LOOPBACK)){
            auto sa = (struct sockaddr_ll*)ifa->ifa_addr;
            mac_address[0] = sa->sll_addr[0];
            mac_address[1] = sa->sll_addr[1];
            mac_address[2] = sa->sll_addr[2];
			break;
        }
    }
   if (if_list != nullptr)
        freeifaddrs(if_list);
}

int main() {
   uint8_t mac[3];
   uint8_t a{90};
   std::cout << static_cast<int>(a) << std::endl;
   get_mac_address(mac);
   std::cout << "MAC address: " 
             << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(mac[0]) << ":"
             << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(mac[1]) << ":"
             << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(mac[2]) 
             << std::endl;
   return 0;
}