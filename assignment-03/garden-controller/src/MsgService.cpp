#include "Arduino.h"
#include "MsgService.h"

MsgServiceSerial MsgSerial;

bool MsgServiceSerial::isMsgAvailable() {
    return msgAvailable;
}

Msg* MsgServiceSerial::receiveMsg() {
    if (msgAvailable) {
        Msg* msg = currentMsg;
        msgAvailable = false;
        currentMsg = NULL;
        content = "";
        return msg;
    } else {
        return NULL;
    }
}

void MsgServiceSerial::init() {
    Serial.begin(9600);
    content.reserve(256);
    content = "";
    currentMsg = NULL;
    msgAvailable = false;
}

void MsgServiceSerial::sendMsg(const String &msg) {
    Serial.println(msg);
}

void serialEvent() {
    while (Serial.available()) {
        char ch = (char)Serial.read();

        if (ch == '\n') {
            MsgSerial.setCurrentMsg(MsgSerial.getContent());
            MsgSerial.setIsMessageAvailable(true);
        } else {
        MsgSerial.addToContent(ch);
        }
    }
}

String MsgServiceSerial::getContent() {
    return content;
}

void MsgServiceSerial::addToContent(char ch) {
    content += ch;
}

void MsgServiceSerial::setCurrentMsg(String content) {
    currentMsg = new Msg(content);
}

void MsgServiceSerial::setIsMessageAvailable(bool value) {
    msgAvailable = value;
}



MsgServiceBT::MsgServiceBT(int rxPin, int txPin) {
    channel = new SoftwareSerial(rxPin, txPin);
}

void MsgServiceBT::init() {
    //content.reserve(256);
    channel->begin(9600);
    availableMsg = NULL;
}

bool MsgServiceBT::sendMsg(Msg msg) {
    channel->println(msg.getContent());
}

bool MsgServiceBT::isMsgAvailable(){
    while (channel->available()) {
        char ch = (char) channel->read();
        if (ch == '\n') {
            availableMsg = new Msg(content); 
            content = "";
            return true;    
        } else {
            content += ch;
        }
    }
    return false;  
}

Msg* MsgServiceBT::receiveMsg() {
    if (availableMsg != NULL) {
        Msg* msg = availableMsg;
        availableMsg = NULL;
        return msg;
    } else {
        return NULL;
    }
}
