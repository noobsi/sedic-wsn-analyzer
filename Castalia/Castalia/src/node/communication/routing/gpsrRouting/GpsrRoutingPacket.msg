
cplusplus {{
#include "RoutingPacket_m.h"
}}

class RoutingPacket;

enum GpsrForwardingMode {
    GPSR_GREEDY_ROUTING = 0;
    GPSR_PERIMETER_ROUTING = 1;
};

enum GpsrPacketDef {
    GPSR_DATA_PACKET             =   0;       // data packet
    GPSR_HELLO_MSG_PACKET        =   1;      // hello msg
    GPSR_REP_HELLO_MSG_PACKET    =   2;     // rep hello msg 
    GPSR_SINK_ADDRESS_PACKET     =   3;    // sink address
}; 

packet GpsrPacket extends RoutingPacket {
    int GpsrPacketKind enum (GpsrPacketDef);
    int routingMode enum (GpsrForwardingMode);

    // for hello message
    double x_src;
    double y_src;


	  double x_dst;	// for the sink when it is a data message
	  double y_dst;
}



