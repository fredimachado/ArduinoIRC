/*
  IRCMessage.h - Internet Relay Chat library v0.1.0 - 2016-4-9
  Copyright (C) 2016 Fredi Machado.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef IRCMessage_h
#define IRCMessage_h

struct IRCMessage {
  IRCMessage(String data) :
    original(data) {};

  String original;
  String prefix;
  String nick;
  String user;
  String host;
  String command;
  String parameters;
  String text;
};

#endif /* IRCMessage_h */
