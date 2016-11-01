

#ifndef CLIENT_WITHCHATBOX_EVENTMANAGER_H
#define CLIENT_WITHCHATBOX_EVENTMANAGER_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>

enum class EventType{
    KeyDown = sf::Event::KeyPressed,
    KeyUp = sf::Event::KeyReleased,
    MButtonDown = sf::Event::MouseButtonPressed,
    MButtonUp = sf::Event::MouseButtonReleased,
    MouseWheel = sf::Event::MouseWheelMoved,
    WindowResized = sf::Event::Resized,
    GainedFocus = sf::Event::GainedFocus,
    LostFocus = sf::Event::LostFocus,
    MouseEntered = sf::Event::MouseEntered,
    MouseLeft = sf::Event::MouseLeft,
    Closed = sf::Event::Closed,
    TextEntered = sf::Event::TextEntered,
    Keyboard = sf::Event::Count + 1, Mouse, Joystick
};

struct EventInfo{
    EventInfo(){ m_code = 0; }
    EventInfo(int l_event){ m_code = l_event; }
    union{
        int m_code;
    };
};

struct EventDetails{
    EventDetails(const std::string& l_bindName)
            : m_name(l_bindName){
        Clear();
    }
    std::string m_name;

    sf::Vector2i m_size;
    sf::Uint32 m_textEntered;
    sf::Vector2i m_mouse;
    int m_mouseWheelDelta;
    int m_keyCode; // Single key code.

    void Clear(){
        m_size = sf::Vector2i(0, 0);
        m_textEntered = 0;
        m_mouse = sf::Vector2i(0, 0);
        m_mouseWheelDelta = 0;
        m_keyCode = -1;
    }
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct Binding{
    Binding(const std::string& l_name) : m_name(l_name), m_details(l_name), c(0){}
    ~Binding(){}
    void BindEvent(EventType l_type, EventInfo l_info = EventInfo()){
        m_events.emplace_back(l_type, l_info);
    }

    Events m_events;
    std::string m_name;
    int c; // Count of events that are "happening".

    EventDetails m_details;
};

using Bindings = std::unordered_map<std::string, Binding*>;
// Callback container.
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
// State callback container, mapping a StateType to a CallbackContainer type,
// so that we can have only one CallbackContainer per state in addition to only one callback function per name.
enum class StateType;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;

class EventManager{
public:
    EventManager();
    ~EventManager();

    bool AddBinding(Binding *l_binding);
    bool RemoveBinding(std::string l_name);

    void SetCurrentState(StateType l_state);
    void SetFocus(const bool& l_focus);

    // Needs to be defined in the header!
    template<class T>
    bool AddCallback(StateType l_state, const std::string& l_name,
                     void(T::*l_func)(EventDetails*), T* l_instance)
    {
        // Attempt to insert a new element to the m_callbacks map, pairing together the state argument and a new CallbackContainer.
        // Since a map can only have one element with a specific index, in this case it's the StateType, the emplace
        // method always returns a pair of elements, the first of which is an iterator.
        // If the insertion succeeded, the iterator points to the newly created element, if an element with the specified index
        // already existed, the iterator points to that element instead.
        auto itr = m_callbacks.emplace(l_state, CallbackContainer()).first; // itr = newly inserted element OR already existing element.
        // function binding (see first implementation of EventManager)
        auto temp = std::bind(l_func, l_instance, std::placeholders::_1);
        // Insert the actual callback into the CallbackContainer type, which is the second value in the pair that makes up the m_callbacks elements.
        // The second value of a pair that gets returned by the insert method of a map is a Boolean that represents the success of an insertion (which is returned here).
        return itr->second.emplace(l_name, temp).second;
    }

    bool RemoveCallback(StateType l_state, const std::string& l_name){
        // itr->second is the CallbackContainer, for a given key(a state).
        auto itr = m_callbacks.find(l_state);           // Find CallbackContainer for the given state.
        if (itr == m_callbacks.end()){ return false; }
        auto itr2 = itr->second.find(l_name);           // From the CallbackContainer, find the callback by name,
        if (itr2 == itr->second.end()){ return false; }
        itr->second.erase(l_name); // Erase callback by name, from the CallbackContainer.
        return true;
    }

    void HandleEvent(sf::Event& l_event);
    void Update();

    // Getters.
    sf::Vector2i GetMousePos(sf::RenderWindow* l_wind = nullptr){
        return (l_wind ? sf::Mouse::getPosition(*l_wind) : sf::Mouse::getPosition());
    }
private:
    void LoadBindings();

    StateType m_currentState;
    Bindings m_bindings;
    Callbacks m_callbacks;

    bool m_hasFocus;
};
#endif //CLIENT_WITHCHATBOX_EVENTMANAGER_H
