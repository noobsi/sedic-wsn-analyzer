
cplusplus {{
#include "RoutingPacket_m.h"
#include "GeoMathHelper.h"
}}

class noncobject Point;

class RoutingPacket;

enum StablePacketDef {
    STABLE_DATA_PACKET             =   0;       // data packet
};

packet StablePacket extends RoutingPacket {
    int packetId;
    int StablePacketKind enum (StablePacketDef);
    Point destLocation;
}

packet DiscoverHolePacket extends RoutingPacket {
  int originatorId;
  Point ballCenter;
  int previousId;
  string path;
}


