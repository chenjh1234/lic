 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string>  getNetCards()
{
    vector<string> names;
    string name;
    struct ifaddrs *ifc, *ifc1;
    char ip[64] = {};
    char nm[64] = {};

    int i;
    i = 0;
 
    if(0 != getifaddrs(&ifc)) return names;
    ifc1 = ifc;
 
    //printf("iface\tIP address\tNetmask\n");

    for(; NULL != ifc; ifc = (*ifc).ifa_next){
 
        if ( (ifc->ifa_flags & IFF_LOOPBACK) )  continue; //printf ("loopback ");
        if (! (ifc->ifa_flags & IFF_RUNNING) )  continue;//printf ("running  ");
        if ( !(ifc->ifa_flags & IFF_UP) ) continue;// printf ("up   ");
        if (! (ifc->ifa_flags & IFF_MULTICAST) )  continue;//printf ("IFF_MULTICAST   ");
        if (!(ifc->ifa_addr->sa_family == AF_INET)) continue ;//printf ("AF_INET   ");
        //name;
        name = (*ifc).ifa_name;
        //ip
        if(NULL != (*ifc).ifa_addr) 
        {
            inet_ntop(AF_INET, &(((struct sockaddr_in*)((*ifc).ifa_addr))->sin_addr), ip, 64);
        }
        //mask:
        if(NULL != (*ifc).ifa_netmask)
        {
            inet_ntop(AF_INET, &(((struct sockaddr_in*)((*ifc).ifa_netmask))->sin_addr), nm, 64);
        }
        names.push_back(name);
        i++;      
    }
 
    //freeifaddrs(ifc);
    freeifaddrs(ifc1);
    return names;
}
string getNetCard()
{
    string card;
    card="";
    vector<string> cards;
    cards = getNetCards();
    if (cards.size()>0)  card = cards[0];
    return card;
}
 
int main(int argc, char* argv[])
{
    int i;
    vector<string> vnm;
    vnm = getNetCards();
    cout <<"cards =" <<vnm.size() << endl;
    for (i  =0; i < vnm.size();i++)  
        cout << i << "=" <<vnm[i] << endl; 
    cout << "getNetCard=" << getNetCard() <<endl;

//exit(1);
    struct ifaddrs *ifc, *ifc1;
    char ip[64] = {};
    char nm[64] = {};
    char loop[]="loop ";
      char run[]="run ";
      char  up[]="up  ";
  char  multi[]="mult ";
  char       af[]="inet ";
   char       nl[]="     ";
 
    if(0 != getifaddrs(&ifc)) return -1;
    ifc1 = ifc;
 
    printf("iface\t\t\t\tIP address\tNetmask\n");

    for(; NULL != ifc; ifc = (*ifc).ifa_next){
        printf("%s", (*ifc).ifa_name);
        printf("\t");
        if ( (ifc->ifa_flags & IFF_LOOPBACK) )  printf (loop);
        else printf(nl);
        if ( (ifc->ifa_flags & IFF_RUNNING) )  printf (run);
        else printf(nl);
        if ( (ifc->ifa_flags & IFF_UP) )  printf (up);
        else printf(nl);
        if ( (ifc->ifa_flags & IFF_MULTICAST) )  printf (multi);
        else printf(nl);
        if (ifc->ifa_addr->sa_family == AF_INET) printf (af);
        else printf(nl);
        //printf("\t");
 
        if(NULL != (*ifc).ifa_addr) 
        {
            inet_ntop(AF_INET, &(((struct sockaddr_in*)((*ifc).ifa_addr))->sin_addr), ip, 64);
            printf("\t%s", ip);
        }else
        {
            printf("\t\t");
        }
        if(NULL != (*ifc).ifa_netmask)
        {
            inet_ntop(AF_INET, &(((struct sockaddr_in*)((*ifc).ifa_netmask))->sin_addr), nm, 64);
            printf("\t%s", nm);
        }else
        {
            printf("\t\t");
        }
        printf("\n");
    }
 
    //freeifaddrs(ifc);
    freeifaddrs(ifc1);
    return 0;
}
 #if 0
	network_conf nc;
	for (struct ifaddrs* ifa=addrs ; ifa ; ifa=ifa->ifa_next) {
		/* ignore looback interface, and those not up and running */
		if ( (ifa->ifa_flags & IFF_LOOPBACK) ||
			!(ifa->ifa_flags & IFF_RUNNING) ||
			!(ifa->ifa_flags & IFF_UP))
			continue;
		/* ignore interfaces without a valid address */
		if (ifa->ifa_addr == NULL)
			continue;
		/* ignore interfaces with address families other than AF_INET */
		if (ifa->ifa_addr->sa_family != AF_INET)
			continue;

		/* save the network */
		struct sockaddr_in* sin;
		sin = (struct sockaddr_in*) ifa->ifa_addr;
		nc.addr = sin->sin_addr.s_addr;
		sin = (struct sockaddr_in*) ifa->ifa_netmask;
		nc.netmask = sin->sin_addr.s_addr;
		if (BUS_DEBUG_ENABLED) {
			struct in_addr _in = { nc.addr };
			/* inet_ntoa() uses a static buffer, so we cannot combine
			 * two calls to inet_ntoa() in one bus_debug() */
			bus_debug ("network: %s/", inet_ntoa (_in));
			_in.s_addr = nc.netmask;
			bus_debug ("%s\n", inet_ntoa (_in));
		}
		networks.push_back (nc);
	}
        #endif
