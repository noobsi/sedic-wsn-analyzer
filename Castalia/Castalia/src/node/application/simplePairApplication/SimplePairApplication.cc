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
// added for GPSR
#include "GpsrRoutingControl_m.h"

Define_Module(SimplePairApplication);

void SimplePairApplication::startup()
{
	packet_rate = par("packet_rate");
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

	// set the node's position for the routing layer 
	GpsrRoutingControlCommand *cmd1 = new GpsrRoutingControlCommand("GPSR set node pos", NETWORK_CONTROL_COMMAND);
	cmd1->setGpsrRoutingCommandKind(SET_GPSR_NODE_POS);
	cmd1->setDouble1(px);
	cmd1->setDouble2(py);
	toNetworkLayer(cmd1);
  
	if (isSource) {
		// added for GPSR
		// then set the sink's position if the node is sending something
		VirtualMobilityManager *sinkMobilityModule;
		sinkMobilityModule = check_and_cast <VirtualMobilityManager*>(getParentModule()->getParentModule()->getSubmodule("node",atoi(sinkAddress.c_str()))->getSubmodule("MobilityManager"));
	
		nodeLocation=sinkMobilityModule->getLocation();
		px = nodeLocation.x;
		py = nodeLocation.y;
	
		trace() << "Sink for Node " << self << " is " << sinkAddress << " and has position (" << px << "," << py << ")" << endl;
		
		GpsrRoutingControlCommand *cmd2 = new GpsrRoutingControlCommand("GPSR set sink pos", NETWORK_CONTROL_COMMAND);
		cmd2->setGpsrRoutingCommandKind(SET_GPSR_SINK_POS);
		cmd2->setDouble1(px);
		cmd2->setDouble2(py);
		cmd2->setInt1(atoi(sinkAddress.c_str()));
		toNetworkLayer(cmd2);
		///
		
		setTimer(SEND_PACKET, 10); // 10s of set up
	}
	else
		trace() << "Not sending packets";

	declareOutput("Packets received per node");
}

void SimplePairApplication::fromNetworkLayer(ApplicationPacket * rcvPacket,
		const char *source, double rssi, double lqi)
{
	int sequenceNumber = rcvPacket->getSequenceNumber();

	trace() << "Received packet #" << sequenceNumber << " from node " << source;
	collectOutput("Packets received per node", atoi(source));
}

void SimplePairApplication::timerFiredCallback(int index)
{
	switch (index) {
		case SEND_PACKET:{
      // just send once
			trace() << "Sending packet #" << dataSN;
			toNetworkLayer(createGenericDataPacket(0, dataSN), par("sink"));
			dataSN++;
			/* setTimer(SEND_PACKET, packet_spacing); */
			break;
		}
	}
}

// This method processes a received carrier sense interupt. Used only for demo purposes
// in some simulations. Feel free to comment out the trace command.
void SimplePairApplication::handleRadioControlMessage(RadioControlMessage *radioMsg)
{
	switch (radioMsg->getRadioControlMessageKind()) {
		case CARRIER_SENSE_INTERRUPT:
			trace() << "CS Interrupt received! current RSSI value is: " << radioModule->readRSSI();
                        break;
	}
}

