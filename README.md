# chan
Header only Go like channels for c++
___
This is highly inspired from this [article](https://st.xorian.net/blog/2012/08/go-style-channel-in-c/)

It is simply a thread-safe queue, that can be used for thread syncronization tasks.

### Available methods:
- push()
- get()
- close()
- closed()
___
### Example:
Workers listen for messages on the channel for tasks to preform until the channel is closed and drained.
```c++
#include <iostream> 
#include "chan.h"

void worker(int ID, Chan<int> *chan) {
    int msg;
    while(chan->get(msg)) {
        // do something with the message
        std::cout << ID << ">" << msg << std::endl;
    }
}

int main() 
{ 
    Chan<int> chan;
    // messages
    for(int i=0; i<20; i++) chan.push(i);
    std::thread th1(worker, 1, &chan);
    std::thread th2(worker, 2, &chan); 

    th1.join();
    th2.join();
    return 0; 
} 
```