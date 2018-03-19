//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/rollingBallRouting/RollingBallRoutingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "RollingBallRoutingPacket_m.h"

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
    cEnum *e = cEnum::find("RollingBallPacketDef");
    if (!e) enums.getInstance()->add(e = new cEnum("RollingBallPacketDef"));
    e->insert(ROLLINGBALL_DATA_PACKET, "ROLLINGBALL_DATA_PACKET");
);

Register_Class(RollingBallPacket);

RollingBallPacket::RollingBallPacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->packetId_var = 0;
    this->RollingBallPacketKind_var = 0;
}

RollingBallPacket::RollingBallPacket(const RollingBallPacket& other) : ::RoutingPacket(other)
{
    copy(other);
}

RollingBallPacket::~RollingBallPacket()
{
}

RollingBallPacket& RollingBallPacket::operator=(const RollingBallPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void RollingBallPacket::copy(const RollingBallPacket& other)
{
    this->packetId_var = other.packetId_var;
    this->RollingBallPacketKind_var = other.RollingBallPacketKind_var;
    this->destLocation_var = other.destLocation_var;
}

void RollingBallPacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->packetId_var);
    doPacking(b,this->RollingBallPacketKind_var);
    doPacking(b,this->destLocation_var);
}

void RollingBallPacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->packetId_var);
    doUnpacking(b,this->RollingBallPacketKind_var);
    doUnpacking(b,this->destLocation_var);
}

int RollingBallPacket::getPacketId() const
{
    return packetId_var;
}

void RollingBallPacket::setPacketId(int packetId)
{
    this->packetId_var = packetId;
}

int RollingBallPacket::getRollingBallPacketKind() const
{
    return RollingBallPacketKind_var;
}

void RollingBallPacket::setRollingBallPacketKind(int RollingBallPacketKind)
{
    this->RollingBallPacketKind_var = RollingBallPacketKind;
}

Point& RollingBallPacket::getDestLocation()
{
    return destLocation_var;
}

void RollingBallPacket::setDestLocation(const Point& destLocation)
{
    this->destLocation_var = destLocation;
}

class RollingBallPacketDescriptor : public cClassDescriptor
{
  public:
    RollingBallPacketDescriptor();
    virtual ~RollingBallPacketDescriptor();

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

Register_ClassDescriptor(RollingBallPacketDescriptor);

RollingBallPacketDescriptor::RollingBallPacketDescriptor() : cClassDescriptor("RollingBallPacket", "RoutingPacket")
{
}

RollingBallPacketDescriptor::~RollingBallPacketDescriptor()
{
}

bool RollingBallPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<RollingBallPacket *>(obj)!=NULL;
}

const char *RollingBallPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int RollingBallPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int RollingBallPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *RollingBallPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetId",
        "RollingBallPacketKind",
        "destLocation",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int RollingBallPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetId")==0) return base+0;
    if (fieldName[0]=='R' && strcmp(fieldName, "RollingBallPacketKind")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destLocation")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *RollingBallPacketDescriptor::getFieldTypeString(void *object, int field) const
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
        "Point",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *RollingBallPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 1:
            if (!strcmp(propertyname,"enum")) return "RollingBallPacketDef";
            return NULL;
        default: return NULL;
    }
}

int RollingBallPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    RollingBallPacket *pp = (RollingBallPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string RollingBallPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    RollingBallPacket *pp = (RollingBallPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPacketId());
        case 1: return long2string(pp->getRollingBallPacketKind());
        case 2: {std::stringstream out; out << pp->getDestLocation(); return out.str();}
        default: return "";
    }
}

bool RollingBallPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    RollingBallPacket *pp = (RollingBallPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketId(string2long(value)); return true;
        case 1: pp->setRollingBallPacketKind(string2long(value)); return true;
        default: return false;
    }
}

const char *RollingBallPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 2: return opp_typename(typeid(Point));
        default: return NULL;
    };
}

void *RollingBallPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    RollingBallPacket *pp = (RollingBallPacket *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getDestLocation()); break;
        default: return NULL;
    }
}


