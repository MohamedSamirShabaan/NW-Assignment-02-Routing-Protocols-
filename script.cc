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

#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/output-stream-wrapper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-routing-helper.h"
//#include "ns3/netanim-module.h"



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Part_One_ScriptExample");

int
main (int argc, char *argv[])
{
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (6);
  NodeContainer n0n1 = NodeContainer (nodes.Get (0), nodes.Get (1));
  NodeContainer n0n2 = NodeContainer (nodes.Get (0), nodes.Get (2));
  NodeContainer n1n2 = NodeContainer (nodes.Get (1), nodes.Get (2));
  NodeContainer n1n4 = NodeContainer (nodes.Get (1), nodes.Get (4));
  NodeContainer n1n3 = NodeContainer (nodes.Get (1), nodes.Get (3));
  NodeContainer n2n4 = NodeContainer (nodes.Get (2), nodes.Get (4));
  NodeContainer n4n5 = NodeContainer (nodes.Get (4), nodes.Get (5));
  NodeContainer n3n4 = NodeContainer (nodes.Get (3), nodes.Get (4));

  InternetStackHelper stack;
  stack.Install (nodes);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer d0d1 = p2p.Install (n0n1);
  NetDeviceContainer d0d2 = p2p.Install (n0n2);
  NetDeviceContainer d1d2 = p2p.Install (n1n2);
  NetDeviceContainer d1d4 = p2p.Install (n1n4);
  NetDeviceContainer d1d3 = p2p.Install (n1n3);
  NetDeviceContainer d2d4 = p2p.Install (n2n4);
  NetDeviceContainer d4d5 = p2p.Install (n4n5);
  NetDeviceContainer d3d4 = p2p.Install (n3n4);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i2 = ipv4.Assign (d0d2);
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Assign (d1d2);
  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i4 = ipv4.Assign (d1d4);
  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i3 = ipv4.Assign (d1d3);
  ipv4.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer i2i4 = ipv4.Assign (d2d4);
  ipv4.SetBase ("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer i4i5 = ipv4.Assign (d4d5);
  ipv4.SetBase ("10.1.8.0", "255.255.255.0");
  Ipv4InterfaceContainer i3i4 = ipv4.Assign (d3d4);

  Ipv4GlobalRoutingHelper g;
  g.PopulateRoutingTables ();

  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (5));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (i4i5.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps0 = echoClient.Install (nodes.Get (0));
  ApplicationContainer clientApps1 = echoClient.Install (nodes.Get (1));
  ApplicationContainer clientApps2 = echoClient.Install (nodes.Get (2));
  ApplicationContainer clientApps3 = echoClient.Install (nodes.Get (3));
  ApplicationContainer clientApps4 = echoClient.Install (nodes.Get (4));

  clientApps0.Start (Seconds (2.0));
  clientApps1.Start (Seconds (3.0));
  clientApps2.Start (Seconds (4.0));
  clientApps3.Start (Seconds (5.0));
  clientApps4.Start (Seconds (6.0));

  clientApps0.Stop (Seconds (10.0));
  clientApps1.Stop (Seconds (10.0));
  clientApps2.Stop (Seconds (10.0));
  clientApps3.Stop (Seconds (10.0));
  clientApps4.Stop (Seconds (10.0));

  Ptr<OutputStreamWrapper> routingStreamOut = Create<OutputStreamWrapper>("printed-routing-tables.routes" , std::ios::out);
  g.PrintRoutingTableAllAt(Seconds(0) , routingStreamOut);

/*  AnimationInterface anim("animation.xml");
  anim.SetConstantPosition(nodes.Get(0),0.0 , 3.0);
  anim.SetConstantPosition(nodes.Get(1),1.5 , 5.0);
  anim.SetConstantPosition(nodes.Get(2),1.5 , 1.0);
  anim.SetConstantPosition(nodes.Get(3),5.0 , 5.0);
  anim.SetConstantPosition(nodes.Get(4),5.0 , 1.0);
  anim.SetConstantPosition(nodes.Get(5),7.0 , 1.0);*/


  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
