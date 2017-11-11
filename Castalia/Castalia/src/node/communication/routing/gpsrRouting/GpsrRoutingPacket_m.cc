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
    this->GpsrPacketKind_var = 0;
    this->routingMode_var = 0;
    this->destX_var = 0;
    this->destY_var = 0;
    this->helloX_var = 0;
    this->helloY_var = 0;
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
    this->GpsrPacketKind_var = other.GpsrPacketKind_var;
    this->routingMode_var = other.routingMode_var;
    this->destX_var = other.destX_var;
    this->destY_var = other.destY_var;
    this->helloX_var = other.helloX_var;
    this->helloY_var = other.helloY_var;
}

void GpsrPacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->GpsrPacketKind_var);
    doPacking(b,this->routingMode_var);
    doPacking(b,this->destX_var);
    doPacking(b,this->destY_var);
    doPacking(b,this->helloX_var);
    doPacking(b,this->helloY_var);
}

void GpsrPacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->GpsrPacketKind_var);
    doUnpacking(b,this->routingMode_var);
    doUnpacking(b,this->destX_var);
    doUnpacking(b,this->destY_var);
    doUnpacking(b,this->helloX_var);
    doUnpacking(b,this->helloY_var);
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

double GpsrPacket::getDestX() const
{
    return destX_var;
}

void GpsrPacket::setDestX(double destX)
{
    this->destX_var = destX;
}

double GpsrPacket::getDestY() const
{
    return destY_var;
}

void GpsrPacket::setDestY(double destY)
{
    this->destY_var = destY;
}

double GpsrPacket::getHelloX() const
{
    return helloX_var;
}

void GpsrPacket::setHelloX(double helloX)
{
    this->helloX_var = helloX;
}

double GpsrPacket::getHelloY() const
{
    return helloY_var;
}

void GpsrPacket::setHelloY(double helloY)
{
    this->helloY_var = helloY;
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
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
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
        "GpsrPacketKind",
        "routingMode",
        "destX",
        "destY",
        "helloX",
        "helloY",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int GpsrPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='G' && strcmp(fieldName, "GpsrPacketKind")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "routingMode")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destX")==0) return base+2;
    if (fieldName[0]=='d' && strcmp(fieldName, "destY")==0) return base+3;
    if (fieldName[0]=='h' && strcmp(fieldName, "helloX")==0) return base+4;
    if (fieldName[0]=='h' && strcmp(fieldName, "helloY")==0) return base+5;
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
        "double",
        "double",
        "double",
        "double",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
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
        case 0:
            if (!strcmp(propertyname,"enum")) return "GpsrPacketDef";
            return NULL;
        case 1:
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
        case 0: return long2string(pp->getGpsrPacketKind());
        case 1: return long2string(pp->getRoutingMode());
        case 2: return double2string(pp->getDestX());
        case 3: return double2string(pp->getDestY());
        case 4: return double2string(pp->getHelloX());
        case 5: return double2string(pp->getHelloY());
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
        case 0: pp->setGpsrPacketKind(string2long(value)); return true;
        case 1: pp->setRoutingMode(string2long(value)); return true;
        case 2: pp->setDestX(string2double(value)); return true;
        case 3: pp->setDestY(string2double(value)); return true;
        case 4: pp->setHelloX(string2double(value)); return true;
        case 5: pp->setHelloY(string2double(value)); return true;
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
        default: return NULL;
    }
}


