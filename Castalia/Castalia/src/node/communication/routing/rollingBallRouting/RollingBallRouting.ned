package node.communication.routing.rollingBallRouting;

simple RollingBallRouting like node.communication.routing.iRouting {
 parameters:
	bool collectTraceInfo = default (false);
	int maxNetFrameSize = default (0);					// bytes, 0 means no limit
  int netDataFrameOverhead = default (14);			// bytes
	int netBufferSize = default (32);					// number of messages
	int GreedySetupFrameOverhead = default (13);	        // bytes


 gates:
	output toCommunicationModule;
	output toMacModule;
	input fromCommunicationModule;
	input fromMacModule;
	input fromCommModuleResourceMgr;
}

