

#ifndef CLIENT_WITHCHATBOX_SHAREDCONTEXT_H
#define CLIENT_WITHCHATBOX_SHAREDCONTEXT_H

#include "Window.h"
#include "EventManager.h"
#include "Client.h"

struct SharedContext{
    SharedContext()
            :m_wind(nullptr),
             m_eventManager(nullptr),
             m_client(nullptr){}

    Window* m_wind;
    EventManager* m_eventManager;
    Client* m_client;
};

#endif //CLIENT_WITHCHATBOX_SHAREDCONTEXT_H
