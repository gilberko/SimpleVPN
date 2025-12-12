#include <stdio.h>





#define IP_HEADER_ITER(F) \
    F(header_len,4) \
    F(version,4) \
    F(ECN,2) \
    F(DSCP,6) \
    F(packet_len,16) \
    F(identification,16) \
    F(fragment_offset,13) \
    F(flags,3) \
    F(ttl, 8) \
    F(protocol, 8) \
    F(header_checksump,16) \
    F(source_addr,32) \
    F(dest_addr,32)


struct simple_ip_header
{
    #define F(x,y) \
    unsigned int x:y;

    IP_HEADER_ITER(F)
#undef F

    void debugPrint()
    {
        printf("IP Packet.\n");

    #define F(x,y) \
        printf(#x " - 0x%x (%d)\n", x, x);


    IP_HEADER_ITER(F)
#undef F
    
    }
};
