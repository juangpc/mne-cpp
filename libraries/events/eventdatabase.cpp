#include "eventdatabase.h"

using namespace EVENTSINTERNAL;

idNum EventDatabase::m_iEventIdCounter(0);
idNum EventDatabase::m_iGroupIdCounter(0);

//=============================================================================================================

EventDatabase::EventDatabase()
{

}

//=============================================================================================================

idNum EventDatabase::generateNewEventId() const
{
    return ++m_iEventIdCounter;
}

//=============================================================================================================

idNum EventDatabase::generateNewGroupId() const
{
    return ++m_iGroupIdCounter;
}

//=============================================================================================================

size_t EventDatabase::getNumEvents() const
{
    return m_EventsListBySample.size();
}

//=============================================================================================================

std::optional<EventINT> EventDatabase::findEventById(idNum eventId) const
{
    auto eventInt = findEventINT(eventId);
    if(eventInt.has_value())
    {
        return eventInt.value()->second;
    }
    return {};
}

//=============================================================================================================

EventINT EventDatabase::insertEvent(int sample, idNum groupId)
{
    EventINT newEvent(generateNewEventId(), sample, groupId);
    m_EventsListBySample.emplace(std::make_pair(newEvent.getSample(),newEvent));
    m_MapIdToSample[newEvent.getId()] = newEvent.getSample();
    return newEvent;
}

//=============================================================================================================

EventINT EventDatabase::insertEvent(const EventINT& event)
{
    m_EventsListBySample.emplace(std::make_pair(event.getSample(),event));
    m_MapIdToSample[event.getId()] = event.getSample();
    return event;
}

//=============================================================================================================

bool EventDatabase::deleteEvent(idNum eventId)
{
    bool status(false);
    auto event = findEventINT(eventId);
    if(event.has_value())
    {
        m_EventsListBySample.erase(event.value());
        m_MapIdToSample.erase(eventId);

        if(m_pSharedMemManager->isInit())
        {
            m_pSharedMemManager->deleteEvent(event.value()->second.getSample());
        }

        status = true;
    }
    return status;
}

//=============================================================================================================

std::optional<std::multimap<const int, EVENTSINTERNAL::EventINT>::const_iterator>
EventDatabase::findEventINT(idNum eventId) const
{
    int sample = m_MapIdToSample.at(eventId);
    auto eventsRange = m_EventsListBySample.equal_range(sample);
    for(auto e = eventsRange.first; e != eventsRange.second; ++e)
    {
        if( e->second.getId() == eventId)
        {
            return e;
        }
    }
    return {};
}

