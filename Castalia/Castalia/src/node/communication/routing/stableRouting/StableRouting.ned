package node.communication.routing.stableRouting;

simple StableRouting like node.communication.routing.iRouting {
 parameters:
	bool collectTraceInfo = default (false);
	int maxNetFrameSize = default (0);					// bytes, 0 means no limit
  int netDataFrameOverhead = default (14);			// bytes
	int netBufferSize = default (32);					// number of messages
	int GreedySetupFrameOverhead = default (13);	        // bytes
	double secondBallRadius = default (60);


 gates:
	output toCommunicationModule;
	output toMacModule;
	input fromCommunicationModule;
	input fromMacModule;
	input fromCommModuleResourceMgr;
}

