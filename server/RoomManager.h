
#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H
#include <list>
#include <mutex>
#include "WaitingRoom.h"
#include "DuelRoom.h"
namespace ygo {

    class GameServer;


    class RoomManager
    {
        private:
        event* keepAliveEvent;

        std::set<std::string> bannedIPs;

        public:
        void ban(std::string);
        private:
        bool isBanned(std::string);


        static const int SecondsBeforeFillAllRooms = 3;
        static const int RemoveDeadRoomsRatio = 3;

        WaitingRoom* waitingRoom;
        void removeDeadRooms();
        bool FillRoom(DuelRoom* room);
        bool FillAllRooms();

        static void keepAlive(evutil_socket_t fd, short events, void* arg);
        public:
        event_base* net_evbase;
        std::list<DuelRoom *> elencoServer;
        std::set<DuelRoom *> playingServer;
        std::set<DuelRoom *> zombieServer;
        GameServer* gameServer;
        void setGameServer(ygo::GameServer*);

        public:
         DuelRoom* createServer(unsigned char mode);

        RoomManager();
        ~RoomManager();
        void BroadcastMessage(std::wstring, bool, bool crossServer = false);
        void BroadcastMessage(std::string, bool, bool crossServer = false);
        void BroadcastMessage(std::wstring message, bool isAdmin,CMNetServerInterface* origin = nullptr);

        void notifyStateChange(DuelRoom* room,DuelRoom::State oldstate,DuelRoom::State newstate);
        bool InsertPlayerInWaitingRoom(DuelPlayer*dp);
        bool InsertPlayer(DuelPlayer*dp);
        bool InsertPlayer(DuelPlayer*dp,unsigned char mode);
        DuelRoom* getFirstAvailableServer(DuelPlayer* referencePlayer);
        DuelRoom* getFirstAvailableServer(DuelPlayer* referencePlayer,unsigned char mode,bool);
        int getNumPlayers();
        int getNumRooms();
        void HandleCTOSPacket(DuelPlayer* dp, char* data, unsigned int len);

        std::vector<DuelRoom *> getCompatibleRoomsList(int referenceScore);
        void tryToInsertPlayerInServer(DuelPlayer*dp,DuelRoom* serv);
        static int maxScoreDifference(int referenceScore){return std::max(400,(referenceScore>3000)?(referenceScore/3):(referenceScore/4));}
    };




}
#endif
