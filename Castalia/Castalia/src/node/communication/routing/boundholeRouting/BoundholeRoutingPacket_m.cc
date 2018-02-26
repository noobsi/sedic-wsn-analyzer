//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/boundholeRouting/BoundholeRoutingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "BoundholeRoutingPacket_m.h"

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
    cEnum *e = cEnum::find("BoundholePacketDef");
    if (!e) enums.getInstance()->add(e = new cEnum("BoundholePacketDef"));
    e->insert(BOUNDHOLE_DATA_PACKET, "BOUNDHOLE_DATA_PACKET");
    e->insert(BOUNDHOLE_HELLO_MSG_PACKET, "BOUNDHOLE_HELLO_MSG_PACKET");
    e->insert(BOUNDHOLE_REP_HELLO_MSG_PACKET, "BOUNDHOLE_REP_HELLO_MSG_PACKET");
    e->insert(BOUNDHOLE_SINK_ADDRESS_PACKET, "BOUNDHOLE_SINK_ADDRESS_PACKET");
);

Register_Class(BoundholePacket);

BoundholePacket::BoundholePacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->packetId_var = 0;
    this->BoundholePacketKind_var = 0;
}

BoundholePacket::BoundholePacket(const BoundholePacket& other) : ::RoutingPacket(other)
{
    copy(other);
}

BoundholePacket::~BoundholePacket()
{
}

BoundholePacket& BoundholePacket::operator=(const BoundholePacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void BoundholePacket::copy(const BoundholePacket& other)
{
    this->packetId_var = other.packetId_var;
    this->BoundholePacketKind_var = other.BoundholePacketKind_var;
    this->destLocation_var = other.destLocation_var;
    this->helloLocation_var = other.helloLocation_var;
}

void BoundholePacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->packetId_var);
    doPacking(b,this->BoundholePacketKind_var);
    doPacking(b,this->destLocation_var);
    doPacking(b,this->helloLocation_var);
}

void BoundholePacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->packetId_var);
    doUnpacking(b,this->BoundholePacketKind_var);
    doUnpacking(b,this->destLocation_var);
    doUnpacking(b,this->helloLocation_var);
}

int BoundholePacket::getPacketId() const
{
    return packetId_var;
}

void BoundholePacket::setPacketId(int packetId)
{
    this->packetId_var = packetId;
}

int BoundholePacket::getBoundholePacketKind() const
{
    return BoundholePacketKind_var;
}

void BoundholePacket::setBoundholePacketKind(int BoundholePacketKind)
{
    this->BoundholePacketKind_var = BoundholePacketKind;
}

Point& BoundholePacket::getDestLocation()
{
    return destLocation_var;
}

void BoundholePacket::setDestLocation(const Point& destLocation)
{
    this->destLocation_var = destLocation;
}

Point& BoundholePacket::getHelloLocation()
{
    return helloLocation_var;
}

void BoundholePacket::setHelloLocation(const Point& helloLocation)
{
    this->helloLocation_var = helloLocation;
}

class BoundholePacketDescriptor : public cClassDescriptor
{
  public:
    BoundholePacketDescriptor();
    virtual ~BoundholePacketDescriptor();

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

Register_ClassDescriptor(BoundholePacketDescriptor);

BoundholePacketDescriptor::BoundholePacketDescriptor() : cClassDescriptor("BoundholePacket", "RoutingPacket")
{
}

BoundholePacketDescriptor::~BoundholePacketDescriptor()
{
}

bool BoundholePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<BoundholePacket *>(obj)!=NULL;
}

const char *BoundholePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int BoundholePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int BoundholePacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *BoundholePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetId",
        "BoundholePacketKind",
        "destLocation",
        "helloLocation",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int BoundholePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetId")==0) return base+0;
    if (fieldName[0]=='B' && strcmp(fieldName, "BoundholePacketKind")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destLocation")==0) return base+2;
    if (fieldName[0]=='h' && strcmp(fieldName, "helloLocation")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *BoundholePacketDescriptor::getFieldTypeString(void *object, int field) const
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
        "Point",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *BoundholePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 1:
            if (!strcmp(propertyname,"enum")) return "BoundholePacketDef";
            return NULL;
        default: return NULL;
    }
}

int BoundholePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    BoundholePacket *pp = (BoundholePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string BoundholePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    BoundholePacket *pp = (BoundholePacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPacketId());
        case 1: return long2string(pp->getBoundholePacketKind());
        case 2: {std::stringstream out; out << pp->getDestLocation(); return out.str();}
        case 3: {std::stringstream out; out << pp->getHelloLocation(); return out.str();}
        default: return "";
    }
}

bool BoundholePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    BoundholePacket *pp = (BoundholePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketId(string2long(value)); return true;
        case 1: pp->setBoundholePacketKind(string2long(value)); return true;
        default: return false;
    }
}

const char *BoundholePacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 2: return opp_typename(typeid(Point));
        case 3: return opp_typename(typeid(Point));
        default: return NULL;
    };
}

void *BoundholePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    BoundholePacket *pp = (BoundholePacket *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getDestLocation()); break;
        case 3: return (void *)(&pp->getHelloLocation()); break;
        default: return NULL;
    }
}


