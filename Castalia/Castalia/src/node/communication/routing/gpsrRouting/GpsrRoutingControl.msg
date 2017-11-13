
enum GpsrRoutingCommandDef {
	SET_GPSR_NODE_POS = 1;
	SET_GPSR_SINK_POS = 2;
	SET_GPSR_BOOL = 3; // for future usage
}

message GpsrRoutingControlCommand {
	int gpsrRoutingCommandKind enum (GpsrRoutingCommandDef);
	double double1 = 0;
	double double2 = 0;
	int int1 = 0;
	int int2 = 0;
	bool bool1 = false; // for future usage
}

