//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/stableRouting/StableRoutingPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "StableRoutingPacket_m.h"

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
    cEnum *e = cEnum::find("StablePacketDef");
    if (!e) enums.getInstance()->add(e = new cEnum("StablePacketDef"));
    e->insert(STABLE_DATA_PACKET, "STABLE_DATA_PACKET");
);

Register_Class(StablePacket);

StablePacket::StablePacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->packetId_var = 0;
    this->StablePacketKind_var = 0;
}

StablePacket::StablePacket(const StablePacket& other) : ::RoutingPacket(other)
{
    copy(other);
}

StablePacket::~StablePacket()
{
}

StablePacket& StablePacket::operator=(const StablePacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void StablePacket::copy(const StablePacket& other)
{
    this->packetId_var = other.packetId_var;
    this->StablePacketKind_var = other.StablePacketKind_var;
    this->destLocation_var = other.destLocation_var;
}

void StablePacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->packetId_var);
    doPacking(b,this->StablePacketKind_var);
    doPacking(b,this->destLocation_var);
}

void StablePacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->packetId_var);
    doUnpacking(b,this->StablePacketKind_var);
    doUnpacking(b,this->destLocation_var);
}

int StablePacket::getPacketId() const
{
    return packetId_var;
}

void StablePacket::setPacketId(int packetId)
{
    this->packetId_var = packetId;
}

int StablePacket::getStablePacketKind() const
{
    return StablePacketKind_var;
}

void StablePacket::setStablePacketKind(int StablePacketKind)
{
    this->StablePacketKind_var = StablePacketKind;
}

Point& StablePacket::getDestLocation()
{
    return destLocation_var;
}

void StablePacket::setDestLocation(const Point& destLocation)
{
    this->destLocation_var = destLocation;
}

class StablePacketDescriptor : public cClassDescriptor
{
  public:
    StablePacketDescriptor();
    virtual ~StablePacketDescriptor();

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

Register_ClassDescriptor(StablePacketDescriptor);

StablePacketDescriptor::StablePacketDescriptor() : cClassDescriptor("StablePacket", "RoutingPacket")
{
}

StablePacketDescriptor::~StablePacketDescriptor()
{
}

bool StablePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<StablePacket *>(obj)!=NULL;
}

const char *StablePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int StablePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int StablePacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *StablePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetId",
        "StablePacketKind",
        "destLocation",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int StablePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetId")==0) return base+0;
    if (fieldName[0]=='S' && strcmp(fieldName, "StablePacketKind")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destLocation")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *StablePacketDescriptor::getFieldTypeString(void *object, int field) const
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

const char *StablePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 1:
            if (!strcmp(propertyname,"enum")) return "StablePacketDef";
            return NULL;
        default: return NULL;
    }
}

int StablePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    StablePacket *pp = (StablePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string StablePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    StablePacket *pp = (StablePacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPacketId());
        case 1: return long2string(pp->getStablePacketKind());
        case 2: {std::stringstream out; out << pp->getDestLocation(); return out.str();}
        default: return "";
    }
}

bool StablePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    StablePacket *pp = (StablePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketId(string2long(value)); return true;
        case 1: pp->setStablePacketKind(string2long(value)); return true;
        default: return false;
    }
}

const char *StablePacketDescriptor::getFieldStructName(void *object, int field) const
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

void *StablePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    StablePacket *pp = (StablePacket *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getDestLocation()); break;
        default: return NULL;
    }
}

Register_Class(DiscoverHolePacket);

DiscoverHolePacket::DiscoverHolePacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->originatorId_var = 0;
    this->previousId_var = 0;
    this->path_var = 0;
}

DiscoverHolePacket::DiscoverHolePacket(const DiscoverHolePacket& other) : ::RoutingPacket(other)
{
    copy(other);
}

DiscoverHolePacket::~DiscoverHolePacket()
{
}

DiscoverHolePacket& DiscoverHolePacket::operator=(const DiscoverHolePacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void DiscoverHolePacket::copy(const DiscoverHolePacket& other)
{
    this->originatorId_var = other.originatorId_var;
    this->ballCenter_var = other.ballCenter_var;
    this->previousId_var = other.previousId_var;
    this->path_var = other.path_var;
}

void DiscoverHolePacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->originatorId_var);
    doPacking(b,this->ballCenter_var);
    doPacking(b,this->previousId_var);
    doPacking(b,this->path_var);
}

void DiscoverHolePacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->originatorId_var);
    doUnpacking(b,this->ballCenter_var);
    doUnpacking(b,this->previousId_var);
    doUnpacking(b,this->path_var);
}

int DiscoverHolePacket::getOriginatorId() const
{
    return originatorId_var;
}

void DiscoverHolePacket::setOriginatorId(int originatorId)
{
    this->originatorId_var = originatorId;
}

Point& DiscoverHolePacket::getBallCenter()
{
    return ballCenter_var;
}

void DiscoverHolePacket::setBallCenter(const Point& ballCenter)
{
    this->ballCenter_var = ballCenter;
}

int DiscoverHolePacket::getPreviousId() const
{
    return previousId_var;
}

void DiscoverHolePacket::setPreviousId(int previousId)
{
    this->previousId_var = previousId;
}

const char * DiscoverHolePacket::getPath() const
{
    return path_var.c_str();
}

void DiscoverHolePacket::setPath(const char * path)
{
    this->path_var = path;
}

class DiscoverHolePacketDescriptor : public cClassDescriptor
{
  public:
    DiscoverHolePacketDescriptor();
    virtual ~DiscoverHolePacketDescriptor();

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

Register_ClassDescriptor(DiscoverHolePacketDescriptor);

DiscoverHolePacketDescriptor::DiscoverHolePacketDescriptor() : cClassDescriptor("DiscoverHolePacket", "RoutingPacket")
{
}

DiscoverHolePacketDescriptor::~DiscoverHolePacketDescriptor()
{
}

bool DiscoverHolePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DiscoverHolePacket *>(obj)!=NULL;
}

const char *DiscoverHolePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DiscoverHolePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int DiscoverHolePacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *DiscoverHolePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "originatorId",
        "ballCenter",
        "previousId",
        "path",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int DiscoverHolePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='o' && strcmp(fieldName, "originatorId")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "ballCenter")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "previousId")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "path")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DiscoverHolePacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "Point",
        "int",
        "string",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *DiscoverHolePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int DiscoverHolePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DiscoverHolePacket *pp = (DiscoverHolePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string DiscoverHolePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DiscoverHolePacket *pp = (DiscoverHolePacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getOriginatorId());
        case 1: {std::stringstream out; out << pp->getBallCenter(); return out.str();}
        case 2: return long2string(pp->getPreviousId());
        case 3: return oppstring2string(pp->getPath());
        default: return "";
    }
}

bool DiscoverHolePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DiscoverHolePacket *pp = (DiscoverHolePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setOriginatorId(string2long(value)); return true;
        case 2: pp->setPreviousId(string2long(value)); return true;
        case 3: pp->setPath((value)); return true;
        default: return false;
    }
}

const char *DiscoverHolePacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 1: return opp_typename(typeid(Point));
        default: return NULL;
    };
}

void *DiscoverHolePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DiscoverHolePacket *pp = (DiscoverHolePacket *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getBallCenter()); break;
        default: return NULL;
    }
}


