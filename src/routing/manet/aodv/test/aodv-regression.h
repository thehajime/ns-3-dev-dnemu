/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
 *
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
 *
 * Authors: Pavel Boyko <boyko@iitp.ru>
 */

#ifndef AODVREGRESSION_H_
#define AODVREGRESSION_H_

#include "ns3/test.h"
#include "ns3/nstime.h"
#include "ns3/node-container.h"

namespace ns3 {
namespace aodv {

/**
 * \ingroup aodv
 * 
 * \brief AODV chain regression test
 *
 * TODO: describe test topology and traffic 
 * 
 * \verbatim
 Expected packets time diagram.
           1       2       3       4       5
    <------|------>|       |       |       |        RREQ (orig 10.1.1.1, dst 10.1.1.5, G=1, U=1, hop=0, ID=1, org_seqno=1) src = 10.1.1.1
           |<------|------>|       |       |        RREQ (orig 10.1.1.1, dst 10.1.1.5, G=1, U=1, hop=1, ID=1, org_seqno=1) src = 10.1.1.2
           |       |<------|------>|       |        RREQ (orig 10.1.1.1, dst 10.1.1.5, G=1, U=1, hop=2, ID=1, org_seqno=1) src = 10.1.1.3
           |       |       |<------|------>|        RREQ (orig 10.1.1.1, dst 10.1.1.5, G=1, U=1, hop=3, ID=1, org_seqno=1) src = 10.1.1.4
           |       |       |       |<------|------> ARP request. Who has 10.1.1.4? Tell 10.1.1.5
           |       |       |       |======>|        ARP reply
           |       |       |       |<======|        RREP (orig 10.1.1.1, dst 10.1.1.5, hop=0, dst_seqno=0) src=10.1.1.5
           |       |       |<------|------>|        ARP request. Who has 10.1.1.3? Tell 10.1.1.4
           |       |       |======>|       |        ARP reply
           |       |       |<======|       |        RREP (orig 10.1.1.1, dst 10.1.1.5, hop=1, dst_seqno=0) src=10.1.1.4
           |       |<------|------>|       |        ARP request. Who has 10.1.1.2? Tell 10.1.1.3
           |       |======>|       |       |        ARP reply
           |       |<======|       |       |        RREP (orig 10.1.1.1, dst 10.1.1.5, hop=2, dst_seqno=0) src=10.1.1.3
           |<------|------>|       |       |        ARP request. Who has 10.1.1.1? Tell 10.1.1.2
           |======>|       |       |       |        ARP reply
           |<======|       |       |       |        RREP (orig 10.1.1.1, dst 10.1.1.5, hop=3, dst_seqno=0) src=10.1.1.2
   <-------|------>|       |       |       |        ARP request. Who has 10.1.1.2? Tell 10.1.1.1
           |<======|       |       |       |
           |======>|       |       |       |        ICMP (ping) request 0 from 10.1.1.1 to 10.1.1.5; src=10.1.1.1 next_hop=10.1.1.2
           |<------|------>|       |       |        ARP request. Who has 10.1.1.3? Tell 10.1.1.2
           |       |<======|       |       |        ARP reply
           |       |======>|       |       |        ICMP (ping) request 0 from 10.1.1.1 to 10.1.1.5; src=10.1.1.2 next_hop=10.1.1.3
           |       |<------|------>|       |        ARP request. Who has 10.1.1.4? Tell 10.1.1.3
           |       |       |<======|       |        ARP reply
           |       |       |======>|       |        ICMP (ping) request 0 from 10.1.1.1 to 10.1.1.5; src=10.1.1.3 next_hop=10.1.1.4
           |       |       |<------|------>|        ARP request. Who has 10.1.1.5? Tell 10.1.1.4
           |       |       |       |<======|        ARP reply
           |       |       |       |======>|        ICMP (ping) request 0; src=10.1.1.4 next_hop=10.1.1.5
           |       |       |       |<======|        ICMP (ping) reply 0; src=10.1.1.5 next_hop=10.1.1.4
           |       |       |<======|       |        ICMP (ping) reply 0; src=10.1.1.4 next_hop=10.1.1.3
           |       |<======|       |       |        ICMP (ping) reply 0; src=10.1.1.3 next_hop=10.1.1.2
           |<======|       |       |       |        ICMP (ping) reply 0; src=10.1.1.2 next_hop=10.1.1.1
           |       |       |       |<------|------> Hello
           |<------|------>|       |       |        Hello
    <------|------>|       |       |       |        Hello
           |       |<------|------>|       |        Hello
           |======>|       |       |       |        ICMP (ping) request 1; src=10.1.1.1 next_hop=10.1.1.2
           |       |       |<------|------>|        Hello
           |       |======>|       |       |        ICMP (ping) request 1; src=10.1.1.2 next_hop=10.1.1.3
           |       |       |======>|       |        ICMP (ping) request 1; src=10.1.1.3 next_hop=10.1.1.4
           |       |       |       |======>|        ICMP (ping) request 1; src=10.1.1.4 next_hop=10.1.1.5
           |       |       |       |<======|        ICMP (ping) reply 1; src=10.1.1.5 next_hop=10.1.1.4
           |       |       |<======|       |        ICMP (ping) reply 1; src=10.1.1.4 next_hop=10.1.1.3
           |       |<======|       |       |        ICMP (ping) reply 11; src=10.1.1.3 next_hop=10.1.1.2
           |<======|       |       |       |        ICMP (ping) reply 1; src=10.1.1.2 next_hop=10.1.1.1
           |       |       |       |<------|------> Hello
           |<------|------>|       |       |        Hello
    <------|------>|       |       |       |        Hello
           |       |       |<------|------>|        Hello
           |       |<------|------>|       |        Hello
           |======>|       |       |       |        ICMP (ping) request 2; src=10.1.1.1 next_hop=10.1.1.2
           |       |======>|       |       |        ICMP (ping) request 2; src=10.1.1.2 next_hop=10.1.1.3
           |       |       |======>|       |        ICMP (ping) request 2; src=10.1.1.3 next_hop=10.1.1.4
           |       |       |       |======>|        ICMP (ping) request 2; src=10.1.1.4 next_hop=10.1.1.5
           |       |       |       |<======|        ICMP (ping) reply 2; src=10.1.1.5 next_hop=10.1.1.4
           |       |       |<======|       |        ICMP (ping) reply 2; src=10.1.1.4 next_hop=10.1.1.3
           |       |<======|       |       |        ICMP (ping) reply 2; src=10.1.1.3 next_hop=10.1.1.2
           |<======|       |       |       |        ICMP (ping) reply 2; src=10.1.1.2 next_hop=10.1.1.1
           |       |       |       |<------|------> Hello
    <------|------>|       |       |       |        Hello
           |       |<------|------>|       |        Hello
           |<------|------>|       |       |        Hello
           |       |       |<------|------>|        Hello
           |======>|       |       |       |        ICMP (ping) request 3; src=10.1.1.1 next_hop=10.1.1.2
           |       |======>|       |       |        ICMP (ping) request 3; src=10.1.1.2 next_hop=10.1.1.3
           |       |       |======>|       |        ICMP (ping) request 3; src=10.1.1.3 next_hop=10.1.1.4
           |       |       |       |======>|        ICMP (ping) request 3; src=10.1.1.4 next_hop=10.1.1.5
           |       |       |       |<======|        ICMP (ping) reply 3; src=10.1.1.5 next_hop=10.1.1.4
           |       |       |<======|       |        ICMP (ping) reply 3; src=10.1.1.4 next_hop=10.1.1.3
           |       |<======|       |       |        ICMP (ping) reply 3; src=10.1.1.3 next_hop=10.1.1.2
           |<======|       |       |       |        ICMP (ping) reply 3; src=10.1.1.2 next_hop=10.1.1.1
           |       |       |       |<------|------> Hello
    <------|------>|       |       |       |        Hello
           |<------|-->    |       |       |        Hello   |
           |       |    <--|-->    |       |        Hello   |Node 3 move away => nobody hear his packets and node 3 doesn't hear anything !
           |       |       |    <--|------>|        Hello   |
           |======>|       |       |       |        ICMP (ping) request 4; src=10.1.1.1 next_hop=10.1.1.2
           |       |==>    |       |       |        ICMP (ping) request 4; src=10.1.1.2 next_hop=10.1.1.3.   7 retries.
           |<======|       |       |       |        RERR (unreachable dst 10.1.1.3 & 10.1.1.5) src=10.1.1.2
           |       |       |       |<------|------> Hello
    <------|------>|       |       |       |        Hello
           |<------|-->    |       |       |        Hello
           |       |    <--|-->    |       |        Hello
           |       |       |    <--|------>|        Hello
    <------|------>|       |       |       |        RREQ (orig 10.1.1.1, dst 10.1.1.5, G=1, hop=0, ID=2, org_seqno=2) src = 10.1.1.1
           |<------|-->    |       |       |        RREQ (orig 10.1.1.1, dst 10.1.1.5, G=1, hop=1, ID=2, org_seqno=1) src = 10.1.1.2
           |       |       |       |<------|------> Hello
           |       |       |    <--|------>|        Hello
           |       |    <--|-->    |       |        Hello
    <------|------>|       |       |       |        Hello
    <------|------>|       |       |       |        RREQ (orig 10.1.1.1, dst 10.1.1.5, G=1, hop=0, ID=3, org_seqno=3) src = 10.1.1.1
           |<------|-->    |       |       |        Hello
           |<------|-->    |       |       |        RREQ (orig 10.1.1.1, dst 10.1.1.5, G=1, hop=1, ID=3, org_seqno=3) src = 10.1.1.2
           |       |       |       |======>|        RERR (unreachable dst 10.1.1.1 & 10.1.1.3) src=10.1.1.4
           |       |       |       |<------|------> Hello
           |       |       |    <--|------>|        Hello
           |       |    <--|-->    |       |        Hello
           |<------|-->    |       |       |        Hello
    <------|------>|       |       |       |        Hello
    <------|------>|       |       |       |        RREQ (orig 10.1.1.1, dst 10.1.1.5, G=1, hop=0, ID=4, org_seqno=4) src = 10.1.1.1
           |<------|-->    |       |       |        RREQ (orig 10.1.1.1, dst 10.1.1.5, G=1, hop=1, ID=4, org_seqno=4) src = 10.1.1.2

..................................................................
 * \endverbatim
 */
class ChainRegressionTest : public TestCase
{
public:
  ChainRegressionTest ();
  ~ChainRegressionTest ();  
private:
  /// Unique PCAP files prefix for this test
  static const char * const PREFIX;
  /// XXX It is important to have pointers here
  NodeContainer * m_nodes;
  
  /// Total simulation time
  const Time m_time;
  /// Chain size
  const uint32_t m_size;
  /// Chain step, meters
  const double m_step;
  
  /// Create test topology
  void CreateNodes ();
  /// Create devices, install TCP/IP stack and applications
  void CreateDevices ();
  /// Compare traces with reference ones
  void CheckResults ();
  /// Go
  bool DoRun ();
};

}
}
#endif /* AODVREGRESSION_H_ */
