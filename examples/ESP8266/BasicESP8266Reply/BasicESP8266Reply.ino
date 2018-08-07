#include <ESP8266WiFi.h>
#include <IRCClient.h>

#define ssid         ""
#define password     ""

#define IRC_SERVER   "irc.rizon.net"
#define IRC_PORT     6667
#define IRC_NICKNAME "MCUIRCClient"
#define IRC_USER     "MCUIRCClient"

#define REPLY_TO     "NICK" // Reply only to this nick

WiFiClient wiFiClient;
IRCClient client(IRC_SERVER, IRC_PORT, wiFiClient);

void setup() {
  Serial.begin(115200);

  delay(100);

  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.printDiag(Serial);

  client.setCallback(callback);
  client.setSentCallback(debugSentCallback);
}

void loop() {
  if (!client.connected()) {
    Serial.println("Attempting IRC connection...");
    // Attempt to connect
    if (client.connect(IRC_NICKNAME, IRC_USER)) {
      Serial.println("connected");
    } else {
      Serial.println("failed... try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    return;
  }

  client.loop();
}

void callback(IRCMessage ircMessage) {
  // PRIVMSG ignoring CTCP messages
  if (ircMessage.command == "PRIVMSG" && ircMessage.text[0] != '\001') {
    String message("<" + ircMessage.nick + "> " + ircMessage.text);
    Serial.println(message);

    if (ircMessage.nick == REPLY_TO) {
      client.sendMessage(ircMessage.nick, "Hi " + ircMessage.nick + "! I'm your IRC bot.");
    }

    return;
  }
  Serial.println(ircMessage.original);
}

void debugSentCallback(String data) {
  Serial.println(data);
}
