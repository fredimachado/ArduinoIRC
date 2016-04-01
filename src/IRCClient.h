/*
  IRCClient.h - Internet Relay Chat library v0.0.1 - 2016-3-30
  Copyright (C) 2016 Fredi Machado.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef IRCClient_h
#define IRCClient_h

#include "Arduino.h"
#include "Client.h"

#ifdef ESP8266
#include <functional>
#define IRC_CALLBACK_SIGNATURE std::function<void(String data)> callback
#else
#define IRC_CALLBACK_SIGNATURE void (*callback)(String data)
#endif

class IRCClient
{
  private:
    Client* client;
    IRC_CALLBACK_SIGNATURE;
    const char* host;
    uint16_t port;
    bool isConnected;
    String nickname;
    void sendIRC(String data);
    void parse(String data);
    void executeCallback(String data);

  public:
    IRCClient(const char*, uint16_t, Client& client);
    IRCClient& setCallback(IRC_CALLBACK_SIGNATURE);
    boolean connect(String nickname, String user);
    boolean loop();
    boolean connected();
    bool debugDataSent;
};

#endif /* IRCClient_h */
