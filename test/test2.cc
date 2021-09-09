#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/olsr-module.h"
#include "ns3/aodv-module.h"
#include "ns3/mesh-module.h"
#include "ns3/mesh-helper.h"
#include "ns3/animation-interface.h"
#include "ns3/mobility-helper.h"
#include "ns3/mobility-model.h"

using namespace ns3;
bool verbose = true;

template <typename T> std::string to_string(T const& value)
{
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}

void TxTrace(std::string context, Ptr<const Packet> packet)
{
  if(verbose)
    {
      NS_LOG_UNCOND("Packet transmitted by "
                    << context << " Time: "
                    << Simulator::Now().GetSeconds());
    }
}

void ReceivePacket (Ptr<Socket> socket)
{
  while (socket->Recv ())
    {
      if(verbose)
        {
          NS_LOG_UNCOND("Received one packet at "
                        << Simulator::Now().GetSeconds());
        }
    }
}


void PrintStats (UdpServerHelper *server)
{
  uint32_t cur = server->GetServer ()->GetReceived() ;
  std::fstream f;
  f.open ("2.txt", std::fstream::out | std::fstream::app);
  f << cur << std::endl;
  f.close();
  std::cout << "total packets = " << cur << std::endl;
}

void SetNodePosition (Ptr<Node> node, Vector pos)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition(pos);
}

int main (int argc, char *argv[])
{

  double d = 30;
  int n = 5;
  int r = 0;
  bool tracing = false;
  int channelWidth = 80;
  std::string Vht = "7";

  CommandLine cmd;
  cmd.AddValue("r","random seed",r);
  cmd.AddValue("d","distance",d);
  cmd.AddValue("n","the number of nodes",n);
  cmd.AddValue("verbose","Output transmission and reception timestamps",verbose);
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
  // "DataMode", StringValue ("OfdmRate6Mbps"),
  // "ControlMode", StringValue ("OfdmRate6Mbps"));
  //*/
  //SetRemoteStationManager.SetMaxSlrc (0);
  // for n
  // ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("HtMcs7"), "ControlMode", StringValue ("HtMcs0"));

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


  /*
MobilityHelper clientMobility2;
clientMobility2.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
clientMobility2.Install (nodes.Get(2));
nodes.Get(2)->GetObject<MobilityModel> ()->SetPosition (Vector(0,0,d));
*/

  //  uint32_t socketPort = 9;
  /*
PPBPHelper ppbp = PPBPHelper ("ns3::UdpSocketFactory",
                     InetSocketAddress (i.GetAddress (0),socketPort));

ppbp.SetAttribute ("H", DoubleValue(0.6));
ppbp.SetAttribute ("MeanBurstArrivals", DoubleValue (1.0));
ppbp.SetAttribute ("MeanBurstTimeLength", DoubleValue(0.500));
ppbp.SetAttribute ("BurstIntensity", DataRateValue (DataRate ("4Mb/s")));
ppbp.SetAttribute("PacketSize", UintegerValue (1370));

ApplicationContainer apps=ppbp.Install (nodes.Get (1));


Ptr<Socket> recvSink = Socket::CreateSocket (nodes.Get (0), UdpSocketFactory::GetTypeId ());
InetSocketAddress local = InetSocketAddress (i.GetAddress (0), socketPort);
recvSink->Bind (local);
recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));
*/
  ApplicationContainer apps;

  UdpServerHelper server (5005);
  apps = server.Install (nodes.Get(0));


  for (int j = 1; j <= n - 1; j++)
    {
      UdpClientHelper client1 (i.GetAddress (0), 5005);
      client1.SetAttribute ("MaxPackets", UintegerValue (1000));
      client1.SetAttribute ("PacketSize", UintegerValue (1250));
      client1.SetAttribute ("Interval", TimeValue (Seconds(0.003)));
      apps = client1.Install (nodes.Get(j));
    }


  // apps = client1.Install (nodes.Get(2));
  /*
  UdpClientHelper client2 (i.GetAddress (0), 5005);
  client2.SetAttribute ("MaxPackets", UintegerValue (10000));
  client2.SetAttribute ("PacketSize", UintegerValue (1250));
  client2.SetAttribute ("Interval", TimeValue (Seconds(0.005)));
  apps = client2.Install (nodes.Get(2));
*/

  Simulator::Schedule (Seconds (60.0), &PrintStats, &server);
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (60.0));

  //Config::Connect("/NodeList/*/ApplicationList/*/Tx", MakeCallback (&TxTrace));

  Simulator::Stop (Seconds(70.0));

  if (tracing == true)
    {
      wifiPhyHelper.EnablePcap ("Sink", devices.Get (0));
      wifiPhyHelper.EnablePcap ("Node1", devices.Get (1));
      wifiPhyHelper.EnablePcap ("Node2", devices.Get (2));
    }

  AnimationInterface animationInterface (std::string ("home.anim"));
  Simulator::Run ();
  Simulator::Destroy ();

}
