#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


class Event
{
public:
    int time;
    char startOrFinish;
    string eventName;
};


void swap(vector<Event> &events, int firstIndex, int secondIndex){
    Event temp;
    temp.eventName = events[firstIndex].eventName;
    temp.time = events[firstIndex].time;
    temp.startOrFinish = events[firstIndex].startOrFinish;
    events[firstIndex].eventName = events[secondIndex].eventName;
    events[firstIndex].time = events[secondIndex].time;
    events[firstIndex].startOrFinish = events[secondIndex].startOrFinish;
    events[secondIndex].eventName = temp.eventName;
    events[secondIndex].time = temp.time;
    events[secondIndex].startOrFinish = temp.startOrFinish;
}


void minHeap(vector<Event> &events, int size, int i){
    int smallestIndex = i;
    int leftIndex = 2 * i + 1;
    int rightIndex = 2 * i + 2;
    if(leftIndex < size && events[leftIndex].time < events[smallestIndex].time)
        smallestIndex = leftIndex;
    if(rightIndex < size && events[rightIndex].time < events[smallestIndex].time)
        smallestIndex = rightIndex;
    if(smallestIndex != i){ // swap
        swap(events, smallestIndex, i);
        minHeap(events, size, smallestIndex);
    }
}


void heapSort(vector<Event> &events, int size){
    for(int i = size/2 - 1; i >= 0; i--)
        minHeap(events, size , i);
}


void removeEvent(vector<Event> &events){
    vector<Event>::iterator it;
    it = events.begin();
    events.erase(it);
    heapSort(events, (int) events.size());
}


void startEvents(vector<Event> &events, int time){
    if(events.size() != 0) {
        if(events[0].time != time)
            cout << "TIME " << time << ": NO EVENT" << endl;
        else{
            if(events[0].startOrFinish == 's'){
                cout << "TIME " << time << ": " << events[0].eventName << " STARTED" << endl;
                removeEvent(events);
                if(events[0].time == time){
                    startEvents(events, time);
                    return;
                }
            }
            else if(events[0].startOrFinish == 'f'){
                cout << "TIME " << time << ": " << events[0].eventName << " ENDED" << endl;
                removeEvent(events);
                if(events[0].time == time){
                    startEvents(events, time);
                    return;
                }
            }
        }
        if(events.size() != 0){
            time++;
            startEvents(events, time);
        }
        return;
    }
    cout << "TIME " << time << ": NO MORE EVENTS, SCHEDULER EXITS" << endl;
}

void readFile(const char* fileName){
    ifstream eventFile;
    eventFile.open(fileName);
    if(eventFile.is_open() && eventFile.good()){
        string line;
        vector<Event> allEvents;
        Event currentEvent;
        string currentEventName;
        int startTime;
        int finishTime;
        char delimiter = ' ';
        string token;
        while(getline(eventFile, line)){
            stringstream ss(line);
            getline(ss, token, delimiter);
            currentEventName = token;
            getline(ss, token, delimiter);
            startTime = stoi(token);
            getline(ss, token, delimiter);
            finishTime = stoi(token);
            if(startTime <= 0){
                cout << currentEventName << ": Event start time cannot be less than or equal to 0." << endl;
                return;
            }
            if(finishTime <= 0){
                cout << currentEventName << ": Event end time cannot be less than or equal to 0." << endl;
                return;
            }
            if(finishTime <= startTime){
                cout << currentEventName << ": Event endsa time cannot be less than or equal to start time." << endl;
                return;
            }
            currentEvent.eventName = currentEventName;
            currentEvent.time = startTime;
            currentEvent.startOrFinish = 's';
            allEvents.push_back(currentEvent);
            currentEvent.eventName = currentEventName;
            currentEvent.time = finishTime;
            currentEvent.startOrFinish = 'f';
            allEvents.push_back(currentEvent);
        }
        heapSort(allEvents, (int) allEvents.size());
        startEvents(allEvents, 1);
    }
}


int main(int argc, const char * argv[]) {
    if(argc == 2)
        readFile(argv[1]);
    else if(argc == 1){
        cout << "No filename is given." << endl;
        return -1;
    }
    else{
        cout << "More than 1 command line argument is given." << endl;
        return -1;
    }
    return 0;
}
