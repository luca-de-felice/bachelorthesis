//
// Copyright (C) 2004 Andras Varga
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_UDPSINK_H
#define __INET_UDPSINK_H

#include "inet/common/INETDefs.h"

#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"

namespace inet {

/**
 * Consumes and prints packets received from the Udp module. See NED for more info.
 */
class INET_API ReplicaApp : public ApplicationBase, public UdpSocket::ICallback
{
  protected:
    enum SelfMsgKinds { START = 1, STOP };
    enum msgType {REQUEST, PREPARE, VOTE, COMMIT, ACKNOWLEDGE, RESPONSE};

    UdpSocket socket;
    int localPort, destPort = -1;
    std::vector<L3Address> destAddresses;
    L3Address destAddr;
    simtime_t startTime;
    simtime_t stopTime;
    cMessage *selfMsg = nullptr;
    int numReceived, numSent = 0;
    int currentlyProcessing = 0;

    double  successProbability;

    int numReplicas;

    cPar *messageLengthPar = nullptr;

  public:
    ReplicaApp() {}
    virtual ~ReplicaApp();

  protected:
    virtual void processPacket(Packet *msg);
    virtual void setSocketOptions();

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;

    virtual Packet* createCommitResponsePacket(int transactionID, msgType type);

    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;

    virtual void processStart();
    virtual void processStop();

    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;
};

} // namespace inet

#endif // ifndef __INET_UDPSINK_H

