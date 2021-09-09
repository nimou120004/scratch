/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "simple-udp-application.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");

int main (int argc, char *argv[])
{
  double d = 30;
  int n = 2;
  int r = 0;
 // bool tracing = false;
  int channelWidth = 80;
  std::string Vht = "7";

  CommandLine cmd;
  cmd.AddValue("r","random seed",r);
  cmd.AddValue("d","distance",d);
  cmd.AddValue("n","the number of nodes",n);
  //cmd.AddValue("verbose","Output transmission and reception timestamps",verbose);
  cmd.AddValue("Vht","Very High Throughput of wifi standard",Vht);
  cmd.AddValue("channelWidth","Channel width",channelWidth);

  cmd.Parse (argc, argv);

  std::fstream f;
  f.open ("2.txt", std::fstream::out | std::fstream::app);
  f << r << ";"<< d << ";" << n << ";";

  f.close();
  std::cout << "r = " << r << std::endl;
  std::cout << "d = " << d << std::endl;
  std::cout << "n = " << n << std::endl;
  std::cout << "Vht = " << Vht << std::endl;
  std::cout << "channelWidth = " << channelWidth << std::endl;

  RngSeedManager::SetRun (r);

  NodeContainer nodes;
  nodes.Create (n);

  WifiHelper wifiHelper;
  wifiHelper.SetStandard (WIFI_PHY_STANDARD_80211ax_5GHZ); // WIFI_PHY_STANDARD_80211g WIFI_STANDARD_80211n_2_4GHZ  WIFI_PHY_STANDARD_80211n_5GHZ  WIFI_PHY_STANDARD_80211ax_5GHZ or WIFI_PHY_STANDARD_80211ac
  // WIFI_PHY_STANDARD_80211ax_5GHZ, serverChannelWidth, 5, 55, 0, 60, 120 * channelRateFactor));
  //WIFI_PHY_STANDARD_80211ax_5GHZ, clientChannelWidth, 5, 45, 0, 50, 160 * channelRateFactor));

  YansWifiPhyHelper wifiPhyHelper =  YansWifiPhyHelper::Default ();
  wifiPhyHelper.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

  wifiPhyHelper.Set ("TxPowerStart", DoubleValue(13)); // 2,5 W or 33,98 dBm
  wifiPhyHelper.Set ("TxPowerEnd", DoubleValue(13)); // 2,5 W or 33,98 dBm
  //wifiPhyHelper.Set ("RxSensitivity", DoubleValue(-72));
  wifiPhyHelper.Set ("RxNoiseFigure", DoubleValue (7));
  wifiPhyHelper.Set ("ChannelWidth", UintegerValue (channelWidth));
  wifiPhyHelper.Set ("Frequency", UintegerValue(5180));
  wifiPhyHelper.Set ("Antennas", UintegerValue(1));
  wifiPhyHelper.Set ("ChannelNumber", UintegerValue(42));
  //wifiPhyHelper.Set ("TxGain", DoubleValue(0));
  //wifiPhyHelper.Set ("RxGain", DoubleValue(0));


  YansWifiChannelHelper wifiChannelHelper;
  wifiChannelHelper.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannelHelper.AddPropagationLoss ("ns3::FriisPropagationLossModel",
                                        "SystemLoss", DoubleValue(1),
                                        "Frequency", DoubleValue(5.18e9));
  wifiPhyHelper.SetChannel (wifiChannelHelper.Create ());

  /* 802.11ax
wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode", StringValue (oss.str ()),
                              "ControlMode", StringValue (oss.str ()));
*/

  ///*1 version 802.11 n
  wifiHelper.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ( "VhtMcs" + Vht), "ControlMode", StringValue ("VhtMcs0"));

  HtWifiMacHelper macHelper = HtWifiMacHelper::Default ();
  macHelper.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer devices = wifiHelper.Install (wifiPhyHelper, macHelper, nodes);

  //wifiPhyHelper.EnablePcap ("mytest", devices);

  InternetStackHelper internet;
  internet.Install (nodes);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer i;
  i = ipv4.Assign (devices);

  MobilityHelper serverMobility;
  serverMobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  serverMobility.Install (nodes.Get(0));
  nodes.Get(0)->GetObject<MobilityModel> ()->SetPosition (Vector(0,0,0));

  MobilityHelper clientMobility;
  for (int j = 1; j <= n - 1; j++)
    {

      clientMobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
      clientMobility.Install (nodes.Get(j));
      nodes.Get(j)->GetObject<MobilityModel> ()->SetPosition (Vector(0,0,d));
    }
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  Packet::EnablePrinting ();

  //Install the simple socket application

  Ptr <SimpleUdpApplication> udp0 = CreateObject <SimpleUdpApplication> ();
  Ptr <SimpleUdpApplication> udp1 = CreateObject <SimpleUdpApplication> ();

  //Set the start & stop times
  udp0->SetStartTime (Seconds(0));
  udp0->SetStopTime (Seconds (10));
  udp1->SetStartTime (Seconds(0));
  udp1->SetStopTime (Seconds (10));

  //install one application at node 0, and the other at node 1
  nodes.Get(0)->AddApplication (udp0);
  nodes.Get(1)->AddApplication (udp1);

  //This is the IP address of node 1
  Ipv4Address dest_ip ("10.1.1.2");

  //Schedule an event to send a packet of size 400 using udp0 targeting IP of node 0, and port 7777
  //Ptr <Packet> packet1 = Create <Packet> (400);
  //Simulator::Schedule (Seconds (1), &SimpleUdpApplication::SendPacket, udp0, packet1, dest_ip, 7777);
  //Another packet of size 800, targeting the same IP address, but a different port.
  Ptr <Packet> packet2 = Create <Packet> (800);

  for(int n = 2; n < 10; n++)
    {
      Simulator::Schedule (Seconds (n), &SimpleUdpApplication::SendPacket, udp0, packet2, dest_ip, 9999);
    }


  LogComponentEnable ("SimpleUdpApplication", LOG_LEVEL_INFO);

  Simulator::Stop (Seconds (10));
  Simulator::Run ();
  Simulator::Destroy ();
}
