#ifndef CLIENT_SHAREDCONTEXT_H
#define CLIENT_SHAREDCONTEXT_H

#include "Window.h"
#include "EventManager.h"

struct SharedContext{
    SharedContext()
            :m_wind(nullptr), m_eventManager(nullptr){}
    Window* m_wind;
    EventManager* m_eventManager;
};

#endif
