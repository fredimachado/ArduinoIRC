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
			} else {
				message += c;
			}
			
			parse(message);
        }
        return true;
    }
    return false;
}

void IRCClient::parse(String data) {
    // TODO: Parse IRC data
    if (callback) {
        callback(data);
    }
}

boolean IRCClient::connected() {
    if (client == NULL) {
        return false;
    }

    if (client->connected() == 0 && this->isConnected) {
        this->isConnected = false;
        client->flush();
        client->stop();
        return false;
    }

    return true;
}

void IRCClient::sendIRC(String data) {
    client->print(data + "\r\n");
}
