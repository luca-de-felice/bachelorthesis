//
// Copyright (C) 2004 Andras Varga
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2011 Zoltan Bojthe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#ifndef __INET_UDPBASICBURST_H
#define __INET_UDPBASICBURST_H

#include <map>
#include <vector>

#include "inet/common/INETDefs.h"

#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"

namespace inet {

/**
 * UDP application. See NED for more info.
 */
class INET_API ClientAppNoCoord : public ApplicationBase, public UdpSocket::ICallback
{
  public:
    enum ChooseDestAddrMode {
        ONCE = 1, PER_BURST, PER_SEND
    };



  protected:
    enum SelfMsgKinds { START = 1, SEND, STOP };
    enum msgType {PREPARE, VOTE, RESPONSE};

    typedef std::map<int, int> SourceSequence;

    // parameters
    std::vector<L3Address> destAddresses;
    simtime_t delayLimit;
    simtime_t startTime;
    simtime_t stopTime;


    simtime_t lastSent;
    simtime_t maxReturnTime;

    int localPort = -1, destPort = -1;

    // volatile parameters:
    cPar *sendIntervalPar = nullptr;

    // state
    UdpSocket socket;
    L3Address destAddr;
    SourceSequence sourceSequence;
    cMessage *timerNext = nullptr;
    simtime_t nextPkt;
    simtime_t nextBurst;
    simtime_t nextSleep;
    bool haveSleepDuration = false;
    bool dontFragment = false;

    bool joinLocalMulticastGroups;

    // statistics:
    static int counter;    // counter for generating a global number for each packet

    int numSent = 0;
    int numReceived = 0;
    int numDeleted = 0;
    int numDuplicated = 0;
    int successfulCommit = 0;

    std::map<int, int> responsesReceived;


    //can remove? {
    std::map<int, std::set<int>> transactions;
    std::map<int, bool> decided;
    //}

    static int transactionID;

    static simsignal_t outOfOrderPkSignal;

  protected:
    // chooses random destination address
    virtual Packet *createPreparePacket(int transId);

    virtual void processPacket(Packet *msg);

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;

    virtual void broadcastAll(int transactionId);

    virtual void processStart();
    virtual void processSend();
    virtual void processStop();

    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;

  public:
    ClientAppNoCoord() {}
    ~ClientAppNoCoord();
};

} // namespace inet

#endif // ifndef __INET_UDPBASICBURST_H

