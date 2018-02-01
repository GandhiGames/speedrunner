//
//  EventSystem.hpp
//  speedrunner
//
//  Created by Robert Wells on 27/01/2018.
//  Copyright © 2018 Robert Wells. All rights reserved.
//

#ifndef EventSystem_hpp
#define EventSystem_hpp

#include <functional>
#include <map>

#include "Event.hpp"

typedef std::multimap<const std::type_info*, const std::function<void(const Event&)>> EventMap;

class EventSystem
{
public:
    template<typename T> void AddListener(const std::function<void(const Event&)>& func)
    {
        m_eventMap.emplace(&typeid(T), func);
    }
    
    template<typename T> void RemoveListener(const std::function<void(const Event&)>& func)
    {
        //TODO: implement
        /*
        bool found = false;
        
        auto range = m_eventMap.equal_range(&typeid(T));
        auto it = range.first;
        for( ;it != range.second; ++it)
        {
            if(it->second == &func)
            {
                found = true;
                break;
            }
        }
        
        if(found)
        {
            m_eventMap.erase(it);
        }
         */
    }
    
    void Raise(const Event& event)
    {
        auto range = m_eventMap.equal_range(&typeid(event));
        for(auto it = range.first; it != range.second; ++it)
        {
            it->second(event);
        }
        
    }
    
private:
    EventMap m_eventMap;
};

/*
 
 #include <functional>
 #include <map>
 #include <typeinfo>
 #include <iostream>
 
 struct Event {
 virtual ~Event() {}
 };
 struct TestEvent : Event {
 std::string msg;
 TestEvent(std::string msg) : msg(msg) {}
 };
 
 
 // Has the limitation that on<T> will not catch subtypes of T, only T.
 // That may or may not be a problem for your usecase.
 //
 // It also doesn't (yet) let you use the subtype as an argument.

 
 EventMap event::eventMap;
 
 int main() {
 // bind some event handlers
 event::on<TestEvent>([](const Event& e) { std::cout << "hi from " << static_cast<const TestEvent&>(e).msg << std::endl; });
 event::on<TestEvent>([](const Event& e) { std::cout << "hi two" << std::endl; });
 
 // fire some events
 event::emit(TestEvent("pinkie"));
 event::emit(TestEvent("the brain"));
 }
 */

#endif /* EventSystem_hpp */
