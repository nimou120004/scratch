#ifndef MYHEADER_H
#define MYHEADER_H
#include "ns3/socket.h"
#include "ns3/application.h"
#include "ns3/packet.h"
#include "ns3/header.h"

using namespace ns3;
namespace ns3
{
  class MyHeader : public Header
  {
    public:
    MyHeader();

    // new methods
    void SetData (uint16_t data);
    uint16_t GetData (void);
    // new method needed
    static TypeId GetTypeId (void);
    // overridden from Header
    virtual uint32_t GetSerializedSize (void) const;
    virtual void Serialize (Buffer::Iterator start) const;
    virtual uint32_t Deserialize (Buffer::Iterator start);
    virtual void Print (std::ostream &os) const;
  private:
    uint16_t m_data;
  };
}


#endif // MYHEADER_H
