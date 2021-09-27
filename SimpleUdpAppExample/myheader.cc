#include "myheader.h"
#include "ns3/socket.h"
#include "ns3/application.h"
#include "ns3/packet.h"
#include "ns3/header.h"
#include "ns3/log.h"

namespace ns3 {
  NS_LOG_COMPONENT_DEFINE("MyHeader");
  NS_OBJECT_ENSURE_REGISTERED(MyHeader);

/*
  MyHeader::MyHeader()
  {

  }
*/

/*
  MyHeader::~MyHeader()
  {

  }
*/


  void MyHeader::SetData(uint32_t data)
  {
    m_data = data;
  }

  uint32_t MyHeader::GetData (void) const
  {
    return m_data;
  }

  uint32_t
  MyHeader::GetSerializedSize (void) const
  {
    return 6;
  }

  void
  MyHeader::Serialize (Buffer::Iterator start) const
  {
    start.WriteU8 (0xfe);
    start.WriteU8 (0xef);
    //the data
    start.WriteHtonU32 (m_data);
  }

  uint32_t
  MyHeader::Deserialize (Buffer::Iterator start)
  {
    uint8_t tmp;
    tmp = start.ReadU8 ();
    NS_ASSERT (tmp == 0xfe);
    tmp = start.ReadU8 ();
    NS_ASSERT (tmp == 0xef);
    m_data = start.ReadNtohU32 ();
    return 6; // the number of bytes consumed.
  }

  void MyHeader::Print (std::ostream &os) const
  {
    os << "data= " << m_data;
  }

  TypeId
  MyHeader::GetTypeId (void)
  {
    static TypeId tid = TypeId("MyHeader").SetParent<Header> ().AddConstructor<MyHeader>();
    return tid;
  }

  TypeId
  MyHeader::GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }
}


