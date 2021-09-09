#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "simple-udp-application.h"

using namespace ns3;

/**
This example illustrates the use of SimpleUdpApplication. It should work regardless of the device you have. 
*/

int main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  NodeContainer nodes;
  nodes.Create (4);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("1Gbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds(6560)));

  NetDeviceContainer csmaDevs;
  csmaDevs = csma.Install (nodes);
  csma.EnableAsciiAll("simple-udp");

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaces;
  ifaces = address.Assign (csmaDevs);
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  Packet::EnablePrinting ();

  //Create our Two UDP applications
  Ptr <SimpleUdpApplication> udp0 = CreateObject <SimpleUdpApplication> ();
  Ptr <SimpleUdpApplication> udp1 = CreateObject <SimpleUdpApplication> ();
  
  //Set the start & stop times
  udp0->SetStartTime (Seconds(0));
  udp0->SetStopTime (Seconds (100));
  udp1->SetStartTime (Seconds(0));
  udp1->SetStopTime (Seconds (100));
  
  //install one application at node 0, and the other at node 1
  nodes.Get(0)->AddApplication (udp0);
  nodes.Get(1)->AddApplication (udp1);
  
  //This is the IP address of node 1
  Ipv4Address dest_ip ("10.1.1.2");

  //Schedule an event to send a packet of size 400 using udp0 targeting IP of node 0, and port 7777
  //loop to send par example 100 packet of size 1000b.
  Ptr <Packet> packet1 = Create <Packet> (400);
  Ptr <Packet> packet2 = Create <Packet> (800);
  // double c = 0.0;
  Simulator::Schedule (Seconds (2.0), &SimpleUdpApplication::SendPacket, udp0, packet2, dest_ip, 9999);
  for(int i = 0; i<100; i++)
    {
      // c = i/100;

      Simulator::Schedule (Seconds(10.0), &SimpleUdpApplication::SendPacket, udp0, packet1, dest_ip, 7777);

    }


  //Another packet of size 800, targeting the same IP address, but a different port.



  LogComponentEnable ("SimpleUdpApplication", LOG_LEVEL_ALL);

  Simulator::Stop (Seconds (100));
  Simulator::Run ();
  Simulator::Destroy ();

}
