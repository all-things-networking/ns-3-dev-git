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
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-layer.h"
#include "../model/QUIC_TEL.h"
#include "ns3/QUIC-Dispatcher.h"
#include "ns3/QUIC-Scheduler.h"
#include "ns3/QUIC-Context.h"
#include "ns3/TCP-header.h"
#include "ns3/QUIC-ReceiveLogic.h"
#include "ns3/QUIC-Event.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("QUIC_TELTest");

int
main (int argc, char *argv[])
{
  NS_LOG_UNCOND ("Testing QUIC FROM TEL");

  NodeContainer nodes;
  nodes.Create(2);

  PointToPointHelper p2p;
  NetDeviceContainer devs = p2p.Install(nodes);

  // Install the stack
  for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); ++i){
    Ptr<Node> node = *i;

    NS_LOG_UNCOND ("Installing ARP on Node " << node->GetId());

    Ptr<ArpL3Protocol> arp = CreateObject<ArpL3Protocol>();
    node->AggregateObject(arp);

    NS_LOG_UNCOND ("Installing IP on Node " << node->GetId());

    Ptr<Ipv4L3Protocol> ip = CreateObject<Ipv4L3Protocol>();
    node->AggregateObject(ip);


    NS_LOG_UNCOND ("Installing ICMP on Node " << node->GetId());

    Ptr<Icmpv4L4Protocol> icmp = CreateObject<Icmpv4L4Protocol>();
    node->AggregateObject(icmp);

    NS_LOG_UNCOND("Configuring Routing");

    Ipv4StaticRoutingHelper staticRouting;
    Ipv4GlobalRoutingHelper globalRouting;
    Ipv4ListRoutingHelper listRouting;
    listRouting.Add (staticRouting, 0);
    listRouting.Add (globalRouting, -10);

    Ptr<Ipv4> ipv4 = node->GetObject<Ipv4>();
    Ptr<Ipv4RoutingProtocol> ipv4Routing = listRouting.Create (node);
    ipv4->SetRoutingProtocol (ipv4Routing);

    Ptr<TrafficControlLayer> tc = CreateObject<TrafficControlLayer>();
    node->AggregateObject(tc);
    arp->SetTrafficControl(tc);

    NS_LOG_UNCOND ("Installing Modular Transport on Node " << node->GetId());
    // As Scheduler and Dispatcher are initialized already during the QUIC_TEL construction, only creating a new QUIC_TEL should be enough
    Ptr<QUIC_TEL> transport = CreateObjectWithAttributes<QUIC_TEL>();
    node->AggregateObject(transport);
  }

  NS_LOG_UNCOND("");

  // Set up IP addresses
  NS_LOG_UNCOND ("Setting up IP Addresses");

  Ipv4AddressHelper ipv4_add;
  ipv4_add.SetBase("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer ipInterfs = ipv4_add.Assign(devs);

  for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); ++i){
    Ptr<Node> node = *i;
    NS_LOG_UNCOND("Node " << node->GetId() << " Interfaces: ");
    Ptr<Ipv4L3Protocol> ipv4 = node->GetObject<Ipv4L3Protocol>();
    for (unsigned int f = 0; f < ipv4->GetNInterfaces(); f++){
      Ptr<Ipv4Interface> intf = ipv4->GetInterface(f);
      for (unsigned int a = 0; a < intf->GetNAddresses(); a++){
        NS_LOG_UNCOND(intf->GetAddress(a));
      }
    }

    NS_LOG_UNCOND("");
  }


  // Send a packet
  Ptr<Node> src = *nodes.Begin();
  Ptr<Ipv4Interface> src_intf = src->GetObject<Ipv4L3Protocol>()->GetInterface(1);
  Ipv4Address saddr = src_intf->GetAddress(0).GetAddress();
  NS_LOG_UNCOND("Source address: " << saddr);
  Ptr<Node> dst = *(nodes.End() - 1);
  Ipv4Address daddr = dst->GetObject<Ipv4L3Protocol>()->GetInterface(1)->GetAddress(0).GetAddress();
  NS_LOG_UNCOND("Destination address: " << daddr);

  Ptr<Packet> packet = Create<Packet> (100);
  MTHeader mth = MTTCPHeader();
  mth.SetF1(2);
  Ptr<QUIC_TEL> transport = src->GetObject<QUIC_TEL>();
  //Simulator::Schedule(Seconds(1), &ModularTransport::SendPacket, transport, packet, mth, saddr, daddr);
  std::cout<<"########## STARTING NOW ##########"<<std::endl;
  int flow_id=1; //flow_id here should be same
  auto context =new QUICContext(flow_id);
  context->saddr = saddr;
  context->daddr = daddr;
  uint8_t data [128];
  for(int i=0;i<128;i++){
      data[i]=i;
  }
  context->data = data;

  // initiate a flow by adding its state/context to the state table.
  // pick a constant for flow id. Context can include anything you need
  // for processing TCP packets, e.g., initial sequence number,
  // window size, beginning of the window, total number of bytes to send, etc.

  transport->WriteToTable(flow_id, context);
  long time = 1;
  // Then, create a "send" event to send the first window of packets for this
  // flow. This event will be processed by "Send if Possible" event processor

  SenderEventCreator senderEventCreator;


  // // Create and send first packet (hellogooodworldbye)
  // MTEvent* event1 = senderEventCreator.CreateAddDataEvent(flow_id, time, "helloworld", 1);
  // transport->AddEventToScheduler(event1);

  // MTEvent* event2 = senderEventCreator.CreateAddDataEvent(flow_id, time, "gooodbye", 2);
  // transport->AddEventToScheduler(event2);
  
  // MTEvent* event3 = senderEventCreator.CreateSendPacketEvent(flow_id, time);
  // transport->AddEventToScheduler(event3);

  // // Create and send second packet (applepizzabreaddance)
  // MTEvent* event4 = senderEventCreator.CreateAddDataEvent(flow_id, time, "applebread", 1);
  // transport->AddEventToScheduler(event4);

  // MTEvent* event5 = senderEventCreator.CreateAddDataEvent(flow_id, time, "pizzadance", 2);
  // transport->AddEventToScheduler(event5);
  
  // MTEvent* event6 = senderEventCreator.CreateSendPacketEvent(flow_id, time);
  // transport->AddEventToScheduler(event6);

  // Receiver side testing -------------------
  /*
  // TODO: comment back the sender back and make both sender test and receiver test working at the same time
  ReceiverEventCreator ReceiverEventCreator;
  
  // normal case:
  std::vector<std::string> fakeData = { "helloworld", "gooodbye" };
  Packet* fakePacket = ReceiverEventCreator.CreateFakePacket(fakeData, 1, true, 0, 0);

  MTEvent* event7 = ReceiverEventCreator.CreateReceiveEvent(flow_id, time, fakePacket);
  transport->AddEventToScheduler(event7);


  // normal case with two packets
  fakeData = { "anotherOneRandom", "anotherTwoRamdom" };
  Packet* fakePacket1 = ReceiverEventCreator.CreateFakePacket(fakeData, 2, false, 2, 0);
  MTEvent* event8 = ReceiverEventCreator.CreateReceiveEvent(flow_id, time, fakePacket1);
  transport->AddEventToScheduler(event8);
  
  fakeData = { "text", "text" };
  Packet* fakePacket2 = ReceiverEventCreator.CreateFakePacket(fakeData, 3, true, 2, 16);
  MTEvent* event9 = ReceiverEventCreator.CreateReceiveEvent(flow_id, time, fakePacket2);
  transport->AddEventToScheduler(event9);


  // packet loss case
  fakeData = { "world", "bye" };
  Packet* fakePacket3 = ReceiverEventCreator.CreateFakePacket(fakeData, 6, true, 4, 5);

  MTEvent* event10 = ReceiverEventCreator.CreateReceiveEvent(flow_id, time, fakePacket3);
  transport->AddEventToScheduler(event10);
  

  fakeData = { "12345", "12345" };
  Packet* fakePacket4 = ReceiverEventCreator.CreateFakePacket(fakeData, 5, false, 4, 0);

  MTEvent* event11 = ReceiverEventCreator.CreateReceiveEvent(flow_id, time, fakePacket4);
  transport->AddEventToScheduler(event11);
  
  fakeData = { "ReciverTesting", "AnotherOneForTest" };
  Packet* fakePacket5 = ReceiverEventCreator.CreateFakePacket(fakeData, 4, true, 6, 0);

  MTEvent* event12 = ReceiverEventCreator.CreateReceiveEvent(flow_id, time, fakePacket5);
  transport->AddEventToScheduler(event12);

  */


  Simulator::Schedule(Seconds(1), &QUIC_TEL::Start, transport,  saddr, daddr, context);
  Simulator::Run ();
  Simulator::Destroy ();
  std::cout<<"########## ENDING NOW ##########"<<std::endl;
}
