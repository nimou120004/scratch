#include "myheader.h"
#include "ns3/socket.h"
#include "ns3/application.h"
#include "ns3/packet.h"
#include "ns3/header.h"
#include "ns3/log.h"

namespace ns3 {
  NS_LOG_COMPONENT_DEFINE("MyHeader");
  NS_OBJECT_ENSURE_REGISTERED(MyHeader);

  MyHeader::MyHeader()
  {

  }


  void MyHeader::SetData(uint16_t data)
  {
    m_data = data;
  }

  uint16_t MyHeader::GetData (void)
  {
    return m_data;
  }

  uint32_t MyHeader::GetSerializedSize (void) const
  {
    return 2;
  }

  void MyHeader::Serialize (Buffer::Iterator start) const
  {
    start.WriteHtonU16 (m_data);
  }

  uint32_t MyHeader::Deserialize (Buffer::Iterator start)
  {
    m_data = start.ReadNtohU16 ();
    return 2;
  }

  void MyHeader::Print (std::ostream &os) const
  {
    os << m_data;
  }

  TypeId MyHeader::GetTypeId (void)
  {
    static TypeId tid = TypeId("ns3::MyHeader").SetParent<Header> ();
    return tid;
  }
}


