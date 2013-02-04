
#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H
#include <vector>


namespace ygo {

    class GameServer;


    class RoomManager
    {
        private:

        static int RoomManagerThread(void* );
        static void keepAlive(evutil_socket_t fd, short events, void* arg);
        public:
        event_base* net_evbase;
            std::vector<CMNetServer *> elencoServer;
        GameServer* gameServer;
        void setGameServer(ygo::GameServer*);

        public:
        RoomManager();
        ~RoomManager();
        CMNetServer* getFirstAvailableServer();
    };




}
#endif