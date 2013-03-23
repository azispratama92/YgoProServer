#ifndef _NetServerInterface_H_
#define _NetServerInterface_H_
#include "network.h"
#include <list>


namespace ygo
{
class DuelPlayer;

class RoomManager;
class GameServer;
struct DuelPlayerInfo
{
    DuelPlayerInfo():zombiePlayer(false),isReady(false),secondsWaiting(0)
    {
    };
bool isReady;
float secondsWaiting;
unsigned char last_state_in_timeout;
bool zombiePlayer;

std::list< std::pair<bool,std::wstring> > pendingMessages;

};

class CMNetServerInterface
{
private:
    unsigned short last_sent;
protected:
    RoomManager* roomManager;
    static char net_server_read[0x20000];
    static char net_server_write[0x20000];
    std::map<DuelPlayer*, DuelPlayerInfo> players;
    GameServer* gameServer;
    void playerReadinessChange(DuelPlayer *dp, bool isReady);
public:
    CMNetServerInterface(RoomManager* roomManager,GameServer*gameServer);
    virtual ~CMNetServerInterface() {};
    virtual void ExtractPlayer(DuelPlayer* dp)=0;
    virtual void InsertPlayer(DuelPlayer* dp)=0;
    virtual void LeaveGame(DuelPlayer* dp)=0;
    virtual void HandleCTOSPacket(DuelPlayer* dp, char* data, unsigned int len)=0;
    DuelPlayer* getFirstPlayer();

    virtual void SendMessageToPlayer(DuelPlayer*dp, char*msg);
    virtual void SystemChatToPlayer(DuelPlayer*dp, const std::wstring);
    void SendPacketToPlayer(DuelPlayer* dp, unsigned char proto);
    template<typename ST>
    void SendPacketToPlayer(DuelPlayer* dp, unsigned char proto, ST& st)
    {
        char* p = net_server_write;
        BufferIO::WriteInt16(p, 1 + sizeof(ST));
        BufferIO::WriteInt8(p, proto);
        memcpy(p, &st, sizeof(ST));
        last_sent = sizeof(ST) + 3;
        if(dp)
            bufferevent_write(dp->bev, net_server_write, last_sent);

    }
    void BroadcastSystemChat(std::wstring);
    void SendBufferToPlayer(DuelPlayer* dp, unsigned char proto, void* buffer, size_t len);
    void ReSendToPlayer(DuelPlayer* dp);
    int getNumPlayers();
};
}
#endif
