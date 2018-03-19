//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/gpsrRouting/GpsrRoutingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "GpsrRoutingPacket_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("GpsrForwardingMode");
    if (!e) enums.getInstance()->add(e = new cEnum("GpsrForwardingMode"));
    e->insert(GPSR_GREEDY_ROUTING, "GPSR_GREEDY_ROUTING");
    e->insert(GPSR_PERIMETER_ROUTING, "GPSR_PERIMETER_ROUTING");
);

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("GpsrPacketDef");
    if (!e) enums.getInstance()->add(e = new cEnum("GpsrPacketDef"));
    e->insert(GPSR_DATA_PACKET, "GPSR_DATA_PACKET");
    e->insert(GPSR_HELLO_MSG_PACKET, "GPSR_HELLO_MSG_PACKET");
    e->insert(GPSR_REP_HELLO_MSG_PACKET, "GPSR_REP_HELLO_MSG_PACKET");
    e->insert(GPSR_SINK_ADDRESS_PACKET, "GPSR_SINK_ADDRESS_PACKET");
);

Register_Class(GpsrPacket);

GpsrPacket::GpsrPacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->packetId_var = 0;
    this->GpsrPacketKind_var = 0;
    this->routingMode_var = 0;
    this->previousId_var = 0;
    this->currentFaceFirstSender_var = 0;
    this->currentFaceFirstReceiver_var = 0;
}

GpsrPacket::GpsrPacket(const GpsrPacket& other) : ::RoutingPacket(other)
{
    copy(other);
}

GpsrPacket::~GpsrPacket()
{
}

GpsrPacket& GpsrPacket::operator=(const GpsrPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void GpsrPacket::copy(const GpsrPacket& other)
{
    this->packetId_var = other.packetId_var;
    this->GpsrPacketKind_var = other.GpsrPacketKind_var;
    this->routingMode_var = other.routingMode_var;
    this->previousId_var = other.previousId_var;
    this->previousLocation_var = other.previousLocation_var;
    this->destLocation_var = other.destLocation_var;
    this->perimeterRoutingStartLocation_var = other.perimeterRoutingStartLocation_var;
    this->perimeterRoutingFaceLocation_var = other.perimeterRoutingFaceLocation_var;
    this->currentFaceFirstSender_var = other.currentFaceFirstSender_var;
    this->currentFaceFirstReceiver_var = other.currentFaceFirstReceiver_var;
}

void GpsrPacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->packetId_var);
    doPacking(b,this->GpsrPacketKind_var);
    doPacking(b,this->routingMode_var);
    doPacking(b,this->previousId_var);
    doPacking(b,this->previousLocation_var);
    doPacking(b,this->destLocation_var);
    doPacking(b,this->perimeterRoutingStartLocation_var);
    doPacking(b,this->perimeterRoutingFaceLocation_var);
    doPacking(b,this->currentFaceFirstSender_var);
    doPacking(b,this->currentFaceFirstReceiver_var);
}

void GpsrPacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->packetId_var);
    doUnpacking(b,this->GpsrPacketKind_var);
    doUnpacking(b,this->routingMode_var);
    doUnpacking(b,this->previousId_var);
    doUnpacking(b,this->previousLocation_var);
    doUnpacking(b,this->destLocation_var);
    doUnpacking(b,this->perimeterRoutingStartLocation_var);
    doUnpacking(b,this->perimeterRoutingFaceLocation_var);
    doUnpacking(b,this->currentFaceFirstSender_var);
    doUnpacking(b,this->currentFaceFirstReceiver_var);
}

int GpsrPacket::getPacketId() const
{
    return packetId_var;
}

void GpsrPacket::setPacketId(int packetId)
{
    this->packetId_var = packetId;
}

int GpsrPacket::getGpsrPacketKind() const
{
    return GpsrPacketKind_var;
}

void GpsrPacket::setGpsrPacketKind(int GpsrPacketKind)
{
    this->GpsrPacketKind_var = GpsrPacketKind;
}

int GpsrPacket::getRoutingMode() const
{
    return routingMode_var;
}

void GpsrPacket::setRoutingMode(int routingMode)
{
    this->routingMode_var = routingMode;
}

int GpsrPacket::getPreviousId() const
{
    return previousId_var;
}

void GpsrPacket::setPreviousId(int previousId)
{
    this->previousId_var = previousId;
}

Point& GpsrPacket::getPreviousLocation()
{
    return previousLocation_var;
}

void GpsrPacket::setPreviousLocation(const Point& previousLocation)
{
    this->previousLocation_var = previousLocation;
}

Point& GpsrPacket::getDestLocation()
{
    return destLocation_var;
}

void GpsrPacket::setDestLocation(const Point& destLocation)
{
    this->destLocation_var = destLocation;
}

Point& GpsrPacket::getPerimeterRoutingStartLocation()
{
    return perimeterRoutingStartLocation_var;
}

void GpsrPacket::setPerimeterRoutingStartLocation(const Point& perimeterRoutingStartLocation)
{
    this->perimeterRoutingStartLocation_var = perimeterRoutingStartLocation;
}

Point& GpsrPacket::getPerimeterRoutingFaceLocation()
{
    return perimeterRoutingFaceLocation_var;
}

void GpsrPacket::setPerimeterRoutingFaceLocation(const Point& perimeterRoutingFaceLocation)
{
    this->perimeterRoutingFaceLocation_var = perimeterRoutingFaceLocation;
}

int GpsrPacket::getCurrentFaceFirstSender() const
{
    return currentFaceFirstSender_var;
}

void GpsrPacket::setCurrentFaceFirstSender(int currentFaceFirstSender)
{
    this->currentFaceFirstSender_var = currentFaceFirstSender;
}

int GpsrPacket::getCurrentFaceFirstReceiver() const
{
    return currentFaceFirstReceiver_var;
}

void GpsrPacket::setCurrentFaceFirstReceiver(int currentFaceFirstReceiver)
{
    this->currentFaceFirstReceiver_var = currentFaceFirstReceiver;
}

class GpsrPacketDescriptor : public cClassDescriptor
{
  public:
    GpsrPacketDescriptor();
    virtual ~GpsrPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(GpsrPacketDescriptor);

GpsrPacketDescriptor::GpsrPacketDescriptor() : cClassDescriptor("GpsrPacket", "RoutingPacket")
{
}

GpsrPacketDescriptor::~GpsrPacketDescriptor()
{
}

bool GpsrPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<GpsrPacket *>(obj)!=NULL;
}

const char *GpsrPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int GpsrPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 10+basedesc->getFieldCount(object) : 10;
}

unsigned int GpsrPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<10) ? fieldTypeFlags[field] : 0;
}

const char *GpsrPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetId",
        "GpsrPacketKind",
        "routingMode",
        "previousId",
        "previousLocation",
        "destLocation",
        "perimeterRoutingStartLocation",
        "perimeterRoutingFaceLocation",
        "currentFaceFirstSender",
        "currentFaceFirstReceiver",
    };
    return (field>=0 && field<10) ? fieldNames[field] : NULL;
}

int GpsrPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetId")==0) return base+0;
    if (fieldName[0]=='G' && strcmp(fieldName, "GpsrPacketKind")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "routingMode")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "previousId")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "previousLocation")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "destLocation")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "perimeterRoutingStartLocation")==0) return base+6;
    if (fieldName[0]=='p' && strcmp(fieldName, "perimeterRoutingFaceLocation")==0) return base+7;
    if (fieldName[0]=='c' && strcmp(fieldName, "currentFaceFirstSender")==0) return base+8;
    if (fieldName[0]=='c' && strcmp(fieldName, "currentFaceFirstReceiver")==0) return base+9;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *GpsrPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "Point",
        "Point",
        "Point",
        "Point",
        "int",
        "int",
    };
    return (field>=0 && field<10) ? fieldTypeStrings[field] : NULL;
}

const char *GpsrPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 1:
            if (!strcmp(propertyname,"enum")) return "GpsrPacketDef";
            return NULL;
        case 2:
            if (!strcmp(propertyname,"enum")) return "GpsrForwardingMode";
            return NULL;
        default: return NULL;
    }
}

int GpsrPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    GpsrPacket *pp = (GpsrPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string GpsrPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    GpsrPacket *pp = (GpsrPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPacketId());
        case 1: return long2string(pp->getGpsrPacketKind());
        case 2: return long2string(pp->getRoutingMode());
        case 3: return long2string(pp->getPreviousId());
        case 4: {std::stringstream out; out << pp->getPreviousLocation(); return out.str();}
        case 5: {std::stringstream out; out << pp->getDestLocation(); return out.str();}
        case 6: {std::stringstream out; out << pp->getPerimeterRoutingStartLocation(); return out.str();}
        case 7: {std::stringstream out; out << pp->getPerimeterRoutingFaceLocation(); return out.str();}
        case 8: return long2string(pp->getCurrentFaceFirstSender());
        case 9: return long2string(pp->getCurrentFaceFirstReceiver());
        default: return "";
    }
}

bool GpsrPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    GpsrPacket *pp = (GpsrPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketId(string2long(value)); return true;
        case 1: pp->setGpsrPacketKind(string2long(value)); return true;
        case 2: pp->setRoutingMode(string2long(value)); return true;
        case 3: pp->setPreviousId(string2long(value)); return true;
        case 8: pp->setCurrentFaceFirstSender(string2long(value)); return true;
        case 9: pp->setCurrentFaceFirstReceiver(string2long(value)); return true;
        default: return false;
    }
}

const char *GpsrPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 4: return opp_typename(typeid(Point));
        case 5: return opp_typename(typeid(Point));
        case 6: return opp_typename(typeid(Point));
        case 7: return opp_typename(typeid(Point));
        default: return NULL;
    };
}

void *GpsrPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    GpsrPacket *pp = (GpsrPacket *)object; (void)pp;
    switch (field) {
        case 4: return (void *)(&pp->getPreviousLocation()); break;
        case 5: return (void *)(&pp->getDestLocation()); break;
        case 6: return (void *)(&pp->getPerimeterRoutingStartLocation()); break;
        case 7: return (void *)(&pp->getPerimeterRoutingFaceLocation()); break;
        default: return NULL;
    }
}


