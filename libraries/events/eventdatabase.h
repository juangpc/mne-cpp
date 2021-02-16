#ifndef  EVENTMANAGER_EVENTSDATABASE_H
#define EVENTMANAGER_EVENTSDATABASE_H

#include <memory>
#include <map>
#include <unordered_map>
#include <optional>
#include "event.h"


namespace EVENTSINTERNAL {

class EventDatabase
{
public:
    EventDatabase();

    size_t getNumEvents() const;

    std::optional<EventINT> findEventById(idNum eventId) const;
    std::optional<int> findEventBySample(idNum);

    std::unique_ptr<EventINT> getAllEvents() const;

    EventINT insertEvent(int sample, idNum groupId);
    EventINT insertEvent(const EventINT& event);

    bool EventDatabase::deleteEvent(idNum eventId);


private:
    std::optional<std::multimap<const int, EVENTSINTERNAL::EventINT>::const_iterator>
    findEventINT(idNum id) const;

    static std::multimap<int, EVENTSINTERNAL::EventINT>    m_EventsListBySample;
    static std::unordered_map<idNum, int>                  m_MapIdToSample;
    static std::map<idNum, EVENTSINTERNAL::EventGroupINT>  m_GroupsList;

    idNum generateNewEventId() const;
    idNum generateNewGroupId() const;

    static idNum                                    m_iEventIdCounter;
    static idNum                                    m_iGroupIdCounter;
};

} //namespace

#endif // DATABASE_H
