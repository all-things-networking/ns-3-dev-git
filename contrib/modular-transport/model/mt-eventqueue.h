#include <queue>

class MTEvent;
class MTEventQueue{
    queue <MTEvent> eventQueue; //Maybe a priority Queue <Time, MTEvent> is better?
    public:
    MTEventQueue();
    InsertEvent();
    PopEvent();

}
