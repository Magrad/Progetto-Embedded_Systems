#ifndef __MSG_SERVICE_H__
#define __MSG_SERVICE_H__

#include "Arduino.h"
#include "SoftwareSerial.h"

class Msg {

private:
    String content;

public:
    Msg(String content) {
        this->content = content;
    }

    String getContent() {
        return content;
    }
};

class MsgServiceSerial {

private:
    String content;
    Msg* currentMsg;
    bool msgAvailable;

    void serialEvent();

public:
    void init();

    bool isMsgAvailable();
    Msg* receiveMsg();
    void sendMsg(const String &msg);

    String getContent();
    void addToContent(char ch);
    void setCurrentMsg(String content);
    void setIsMessageAvailable(bool value);
};

extern MsgServiceSerial MsgSerial;

class MsgServiceBT {

private:
    String content;
    Msg* availableMsg;
    SoftwareSerial* channel;
    
public: 
    MsgServiceBT(int rxPin, int txPin);  
    void init();  
    bool isMsgAvailable();
    Msg* receiveMsg();
    bool sendMsg(Msg msg);
};

#endif
