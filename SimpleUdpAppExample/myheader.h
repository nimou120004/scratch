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
    static TypeId GetTypeId (void);
    virtual TypeId GetInstanceTypeId (void) const;
    // overridden from Header
    virtual uint32_t GetSerializedSize (void) const;
    virtual void Serialize (Buffer::Iterator start) const;
    virtual uint32_t Deserialize (Buffer::Iterator start);
    virtual void Print (std::ostream &os) const;

    // new methods
    void SetData (uint32_t data);
    uint32_t GetData (void) const;


  private:
    uint32_t m_data;
  };
}


#endif // MYHEADER_H
