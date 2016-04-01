/*
  IRCClient.cpp - Internet Relay Chat library v0.0.1 - 2016-3-30
  Copyright (C) 2016 Fredi Machado.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#include "IRCClient.h"

IRCClient::IRCClient(const char* host, uint16_t port, Client& client) {
  this->host = host;
  this->port = port;
  this->client = &client;
  this->isConnected = false;
}

IRCClient& IRCClient::setCallback(IRC_CALLBACK_SIGNATURE) {
  this->callback = callback;
  return *this;
}

boolean IRCClient::connect(String nickname, String user) {
  if (!connected()) {
    int result = client->connect(this->host, this->port);
    if (result == 1) {
      this->nickname = nickname;
      sendIRC("HELLO");
      sendIRC("NICK " + nickname);
      sendIRC("USER " + user + " 8 * :MCU IRC Client");
      this->isConnected = true;
      return true;
    }
    return false;
  }
  return true;
}

boolean IRCClient::loop() {
  if (connected() && client->available()) {
    String message = "";
    while (client->available()) {
      char c = client->read();
      if (c == '\r') {
        client->read(); // discard \n
        parse(message);
        message = "";
      } else {
        message += c;
      }
    }
    return true;
  }
  return false;
}

void IRCClient::parse(String data) {
  String original(data);
  String prefix;
  String nick;
  String user;
  String host;

  if (data[0] == ':') {
    prefix = data.substring(1, data.indexOf(" ") - 1);
    int index = prefix.indexOf("@");
    if (index != -1) {
      nick = prefix.substring(0, index);
      host = prefix.substring(index);
    }
    index = nick.indexOf("!");
    if (index != -1) {
      String temp = nick;
      nick = temp.substring(0, index);
      user = temp.substring(index);
    }

    data = data.substring(data.indexOf(" ") + 1);
  }

  String command = data.substring(0, data.indexOf(" "));
  command.toUpperCase();

  data = data.substring(data.indexOf(" ") + 1);

  if (command == "PING") {
    sendIRC("PONG " + data);
    executeCallback("Ping? Pong!");
    return;
  }

  if (command == "PRIVMSG") {
    int index = data.indexOf(" ");
    String to = data.substring(0, index);
    String text = data.substring(index + 2);

    if (text[0] == '\001') { // CTCP
      text = text.substring(1, text.length() - 1);
      executeCallback("[" + nick + " requested CTCP " + text +"]");
      if (to == this->nickname) {
        if (text == "VERSION") {
          sendIRC("NOTICE " + nick + " :\001VERSION Open source MCU IRC client by Fredi Machado - https://github.com/Fredi/ArduinoIRC \001");
          return;
        }
        // CTCP not implemented
        sendIRC("NOTICE " + nick + " :\001ERRMSG " + text + " :Not implemented\001");
        return;
      }
    }
  }

  executeCallback(original);
}

boolean IRCClient::connected() {
  if (client == NULL) {
    return false;
  }
  boolean rc = (int)client->connected();
  if (!rc && this->isConnected) {
    this->isConnected = false;
    client->flush();
    client->stop();
  }
  return rc;
}

void IRCClient::sendIRC(String data) {
  client->print(data + "\r\n");
  if (debugDataSent) {
    executeCallback("SENT: " + data);
  }
}

void IRCClient::executeCallback(String data) {
  if (callback) {
    callback(data);
  }
}
