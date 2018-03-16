/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#include "SimplePairApplication.h"

Define_Module(SimplePairApplication);

void SimplePairApplication::startup()
{
	startupDelay = par("startupDelay");
  isSource = par("isSource");
  sinkAddress = par("sink").stringValue();
	dataSN = 0;

	// added for GPSR
	// set the node's coordinate, this will also trigger HELLO msg from the GPSR routing layer
	NodeLocation_type nodeLocation;
	nodeLocation=mobilityModule->getLocation();
	double px = nodeLocation.x;
	double py = nodeLocation.y;

	trace() << "Node " << self << " has position (" << px << "," << py << ")" << endl;

	if (isSource) {
		// added for GPSR
		// then set the sink's position if the node is sending something
		VirtualMobilityManager *sinkMobilityModule;
		sinkMobilityModule = check_and_cast <VirtualMobilityManager*>(getParentModule()->getParentModule()->getSubmodule("node",atoi(sinkAddress.c_str()))->getSubmodule("MobilityManager"));
		nodeLocation=sinkMobilityModule->getLocation();
		px = nodeLocation.x;
		py = nodeLocation.y;
		trace() << "Sink for Node " << self << " is " << sinkAddress << " and has position (" << px << "," << py << ")" << endl;

		setTimer(SEND_PACKET, 10); // 10s of set up
	}
	else
		trace() << "Not sending packets";
}

void SimplePairApplication::fromNetworkLayer(ApplicationPacket * rcvPacket,
		const char *source, double rssi, double lqi) {
	int sequenceNumber = rcvPacket->getSequenceNumber();
	trace() << "Received packet #" << sequenceNumber << " from node " << source;
}

void SimplePairApplication::timerFiredCallback(int index) {
	switch (index) {
		case SEND_PACKET:{
      // just send once
			trace() << "Sending packet #" << dataSN;
			toNetworkLayer(createGenericDataPacket(0, dataSN, 100), par("sink"));
			dataSN++;
			/* setTimer(SEND_PACKET, packet_spacing); */
			break;
		}
	}
}

