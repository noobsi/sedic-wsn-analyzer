#include "StableRouting.h"
#include "assert.h"

Define_Module(StableRouting);

int StableRouting::nextId;

void StableRouting::startup(){
  nextId = 0; // static member
  setTimer(DISCOVER_HOLE_START, 2);
  secondBallRadius = par("secondBallRadius");
}

void StableRouting::timerFiredCallback(int index){
  switch(index){
    case DISCOVER_HOLE_START: {
      // check whether this node is in a hole
      double minAngle = 3 * M_PI;
      Point chosenCenter;
      for (auto &n: neighborTable) {
        for (auto center: G::centers(selfLocation, n.location, RADIO_RANGE / 2)) {
          bool ok = true;
          for (auto on: neighborTable) {
            if (on.id == n.id) continue;
            if (G::distance(center, on.location) <= RADIO_RANGE / 2) {
              ok = false;
            }
          }

          if (ok) {
            if (G::norm(atan2(center.y() - selfLocation.y(), center.x() - selfLocation.x())) < minAngle) {
              minAngle = G::norm(atan2(center.y() - selfLocation.y(), center.x() - selfLocation.x()));
              chosenCenter = center;
            }
          }
        }
      }

      if (!chosenCenter.isUnspecified()) {
        Point nextCenter;
        int nextHop = G::findNextHopRollingBall(selfLocation, chosenCenter, RADIO_RANGE / 2, neighborTable, nextCenter);
        if (nextHop != -1) {
          Point nextHopLocation = GlobalLocationService::getLocation(nextHop);
          DiscoverHolePacket *discoverHolePacket = new DiscoverHolePacket("Discover hole packet", NETWORK_LAYER_PACKET);
          discoverHolePacket->setOriginatorId(self);
          discoverHolePacket->setPreviousId(self); // unspecified previous point
          discoverHolePacket->setPath(Util::intVectorToString({self}).c_str());
          discoverHolePacket->setBallCenter(nextCenter);
          toMacLayer(discoverHolePacket, nextHop);
        }
      }

      break;
    }
    default: break;
  }
}


void StableRouting::processBufferedPacket(){
  while (!TXBuffer.empty()) {
    toMacLayer(TXBuffer.front(), BROADCAST_MAC_ADDRESS);
    TXBuffer.pop();
  }
}

void StableRouting::fromApplicationLayer(cPacket * pkt, const char *destination){

  StablePacket *dataPacket = new StablePacket("STABLE routing data packet", NETWORK_LAYER_PACKET);
  encapsulatePacket(dataPacket, pkt);
  dataPacket->setStablePacketKind(STABLE_DATA_PACKET);
  dataPacket->setSource(SELF_NETWORK_ADDRESS);
  dataPacket->setDestination(destination);


  if (string(destination).compare(BROADCAST_NETWORK_ADDRESS)==0) {
    toMacLayer(dataPacket, BROADCAST_MAC_ADDRESS);
    return;
  }

  dataPacket->setDestLocation(GlobalLocationService::getLocation(atoi(destination)));
  dataPacket->setPacketId(nextId++);
  int nextHop = getNextHop(dataPacket);
  if (nextHop != -1) {
    trace() << "WSN_EVENT SEND packetId:" << dataPacket->getPacketId() << " source:" << dataPacket->getSource()
      << " destination:" << dataPacket->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resMgrModule->getRemainingEnergy();
    toMacLayer(dataPacket, nextHop);
    return;
  }
  else {
    delete dataPacket;
  }

}



void StableRouting::fromMacLayer(cPacket * pkt, int macAddress, double rssi, double lqi){
  DiscoverHolePacket *discoverHolePacket = dynamic_cast <DiscoverHolePacket*>(pkt);
  if (discoverHolePacket) {
    processDiscoverHolePacket(discoverHolePacket);
    return;
  }


  StablePacket *netPacket = dynamic_cast <StablePacket*>(pkt);
  if (!netPacket)
    return;

  switch (netPacket->getStablePacketKind()) {
    case STABLE_DATA_PACKET:
      { 
        string dst(netPacket->getDestination());
        string src(netPacket->getSource());
        if ((dst.compare(BROADCAST_NETWORK_ADDRESS) == 0))
          trace() << "Received data from node " << src << " by broadcast";
        processDataPacketFromMacLayer(netPacket);
        break;
      }

    default: return;
  }
}

void StableRouting::finishSpecific() {
  trace() << "WSN_EVENT FINAL" << " id:" << self << " x:" << selfLocation.x() << " y:" << selfLocation.y() << " deathTime:-1";
}

void StableRouting::processHole(DiscoverHolePacket* pkt) {

  Point ballCenter = pkt->getBallCenter();
  string pathString(pkt->getPath());
  vector<int> path = Util::stringToIntVector(pathString);
  vector<Point> points;
  for (auto id: path) {
    points.push_back(GlobalLocationService::getLocation(id));
  }
  if (!G::pointInPolygon(ballCenter, points)) {
    return; // outside boundary
  }

  // processing the thole
  reverse(points.begin(), points.end()); // reverse because  the hole found is clockwise, while convex hull is counterclockwise
  vector<Point> convexHull = G::convexHull(points);
  debugPolygon(points, "green");
//  debugPolygon(convexHull, "blue");
  // check if there is a really significant "cave"
  int pi = 0;
  for (int i = 0; i < points.size(); i++) {
    if (points[i] == convexHull[0]) {
      pi = i;
      break;
    }
  }

  for (int i = 0; i < convexHull.size(); i++) {
    // invariant: at the beginning of the loop, points[pi] == convexHull[gate1]
    int gate1 = i;
    int gate2 = (i + 1) % convexHull.size();
    double maxDistance = 0;
    int startId = pi;
    while (points[pi] != convexHull[gate2]) {
      double distance = G::distanceToLineSegment(convexHull[gate1], convexHull[gate2], points[pi]);
      if (distance > maxDistance) {
        maxDistance = distance;
      }
      pi = (pi + 1) % points.size();
    }
    int endId = pi;

    if (maxDistance > 5 * RADIO_RANGE) {
      // cave points is points within the cave (between two gate)
      vector<Point> cavePoints;
      cavePoints.push_back(points[startId]);
      int i = startId;
      while (i != endId) {
        i = (i + 1) % points.size();
        cavePoints.push_back(points[i]);
      }

      debugPolygon(cavePoints, "red");
      const double SECOND_BALL_RADIUS = 35;
      vector<Point> candidates;
      for (auto &point: cavePoints) {
        if (G::distance(cavePoints[0], point) <= SECOND_BALL_RADIUS * 2 && cavePoints[0] != point) {
          candidates.push_back(point);
        }
      }

      Point chosenCenter;
      for (auto &n: candidates) {
        for (auto center: G::centers(cavePoints[0], n, SECOND_BALL_RADIUS)) {
          bool ok = true;
          for (auto on: candidates) {
            if (n == on) continue;
            if (G::distance(center, on) < SECOND_BALL_RADIUS) {
              ok = false;
            }
          }

          if (ok && !G::pointInPolygon(center, cavePoints)) {
            chosenCenter = center;
          }
        }
      }


      // start rolling the chosen center from points[startId] to points[endId]
      // dealing with cavePoints
      Point currentPoint = cavePoints[0];
      Point currentCenter = chosenCenter;
      while (currentPoint != cavePoints[cavePoints.size() - 1]) {
        debugCircle(currentCenter, SECOND_BALL_RADIUS, "black");
//        debugPoint(currentCenter, "black");
        vector<Point> candidates;
        for (auto &point: cavePoints) {
          if (G::distance(currentPoint, point) <= SECOND_BALL_RADIUS * 2 && currentPoint != point) {
            candidates.push_back(point);
          }
        }
        Point nextCenter;
        Point nextPoint = G::findNextHopRollingBall(currentPoint, currentCenter, SECOND_BALL_RADIUS, candidates, nextCenter);
        debugLine(currentCenter, nextCenter, "blue");
        currentPoint = nextPoint;
        currentCenter = nextCenter;
      }
    }
  }



}

void StableRouting::processDiscoverHolePacket(DiscoverHolePacket* pkt){
  int originatorId = pkt->getOriginatorId();
  int previousId = pkt->getPreviousId();
  Point ballCenter = pkt->getBallCenter();
  string pathString(pkt->getPath());
  vector<int> path = Util::stringToIntVector(pathString);

  if (smallestOriginatorId.find(previousId) != smallestOriginatorId.end()) {
    if (smallestOriginatorId[previousId] < originatorId) {
      // drop the packet
      return;
    } else {
      smallestOriginatorId[previousId] = originatorId;
    }
  } else {
    smallestOriginatorId[previousId] = originatorId;
  }


  Point nextCenter;
  int nextHop = G::findNextHopRollingBall(selfLocation, ballCenter, RADIO_RANGE / 2, neighborTable, nextCenter);

  if (nextHop != -1) {
    if (path.size() >= 2) {
      if (path[0] == self && path[1] == nextHop) {
        if (path.size() > 10) {
          // TODO
          processHole(pkt);
        }
        return;
      }
    }
    path.push_back(self);
//    debugLine(selfLocation, GlobalLocationService::getLocation(nextHop), "green");
    DiscoverHolePacket *newPkt = pkt->dup();
    newPkt->setPreviousId(self);
    newPkt->setBallCenter(nextCenter);
    newPkt->setPath(Util::intVectorToString(path).c_str());
    toMacLayer(newPkt, nextHop);
  }

}

void StableRouting::processDataPacketFromMacLayer(StablePacket* pkt){
  string dst(pkt->getDestination());
  string src(pkt->getSource());

  // if the node is the destination
  if ((dst.compare(SELF_NETWORK_ADDRESS) == 0)) {
    trace() << "WSN_EVENT RECEIVE packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resMgrModule->getRemainingEnergy();
    toApplicationLayer(pkt->decapsulate());
    return;
  } 

  // if the node is the destination by broadcast, we do not forward it
  if ((dst.compare(BROADCAST_NETWORK_ADDRESS) == 0)) {
    trace() << "Received data (routing broadcast) from MAC, send data to application layer. Source node: " << src;
    toApplicationLayer(pkt->decapsulate());
    return;
  }


  // duplicate the packet because we are going to forward it
  StablePacket *netPacket = pkt->dup();
  int nextHop = getNextHop(netPacket);
  if (nextHop != -1) {
    trace() << "WSN_EVENT FORWARD packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
    trace() << "WSN_EVENT ENERGY id:" << self << " energy:" << resMgrModule->getRemainingEnergy();
    toMacLayer(netPacket, nextHop);
    return;
  }
  else {
    trace() << "WSN_EVENT DROP packetId:" << pkt->getPacketId() << " source:" << pkt->getSource()
      << " destination:" << pkt->getDestination() << " current:" << self;
    delete netPacket;
  }
}

int StableRouting::getNextHop(StablePacket* dataPacket){
  return -1;
}

Point StableRouting::getNeighborLocation(int id) {
  for (auto &n: neighborTable) {
    if (n.id == id) {
      return n.location;
    }
  }

  return Point(); // default
}
// will handle interaction between the application layer and the GPRS module in order to pass parameters such as
// the node's position
void StableRouting::handleNetworkControlCommand(cMessage *msg) {
}
