//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/gpsrRouting/GpsrRoutingControl.msg.
//

#ifndef _GPSRROUTINGCONTROL_M_H_
#define _GPSRROUTINGCONTROL_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Enum generated from <tt>src/node/communication/routing/gpsrRouting/GpsrRoutingControl.msg:16</tt> by nedtool.
 * <pre>
 * enum GpsrRoutingCommandDef
 * {
 * 
 *     SET_GPSR_NODE_POS = 1;
 *     SET_GPSR_SINK_POS = 2;
 *     SET_GPSR_BOOL = 3; // for future usage
 * }
 * </pre>
 */
enum GpsrRoutingCommandDef {
    SET_GPSR_NODE_POS = 1,
    SET_GPSR_SINK_POS = 2,
    SET_GPSR_BOOL = 3
};

/**
 * Class generated from <tt>src/node/communication/routing/gpsrRouting/GpsrRoutingControl.msg:22</tt> by nedtool.
 * <pre>
 * message GpsrRoutingControlCommand
 * {
 *     int gpsrRoutingCommandKind @enum(GpsrRoutingCommandDef);
 *     double double1 = 0;
 *     double double2 = 0;
 *     int int1 = 0;
 *     int int2 = 0;
 *     bool bool1 = false; // for future usage
 * }
 * </pre>
 */
class GpsrRoutingControlCommand : public ::cMessage
{
  protected:
    int gpsrRoutingCommandKind_var;
    double double1_var;
    double double2_var;
    int int1_var;
    int int2_var;
    bool bool1_var;

  private:
    void copy(const GpsrRoutingControlCommand& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const GpsrRoutingControlCommand&);

  public:
    GpsrRoutingControlCommand(const char *name=NULL, int kind=0);
    GpsrRoutingControlCommand(const GpsrRoutingControlCommand& other);
    virtual ~GpsrRoutingControlCommand();
    GpsrRoutingControlCommand& operator=(const GpsrRoutingControlCommand& other);
    virtual GpsrRoutingControlCommand *dup() const {return new GpsrRoutingControlCommand(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getGpsrRoutingCommandKind() const;
    virtual void setGpsrRoutingCommandKind(int gpsrRoutingCommandKind);
    virtual double getDouble1() const;
    virtual void setDouble1(double double1);
    virtual double getDouble2() const;
    virtual void setDouble2(double double2);
    virtual int getInt1() const;
    virtual void setInt1(int int1);
    virtual int getInt2() const;
    virtual void setInt2(int int2);
    virtual bool getBool1() const;
    virtual void setBool1(bool bool1);
};

inline void doPacking(cCommBuffer *b, GpsrRoutingControlCommand& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, GpsrRoutingControlCommand& obj) {obj.parsimUnpack(b);}


#endif // ifndef _GPSRROUTINGCONTROL_M_H_

