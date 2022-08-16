#include <Arduino.h>
// #include <vector>
// #include <map>

// std::vector<int> relayPins = {42};
// std::vector<int> relayPins = {44};
// std::vector<int> relayPins = {48};
// std::vector<int> relayPins = {46};
// std::vector<int> relayPins = {50};
// std::vector<int> relayPins = {52};
// std::vector<int> relayPins = {42};
// std::vector<std::map<String, int>> relays;

void setup() {
  // Serial.begin(9600);           // initialize serial
  Serial.print("setup\n");
  // for (const int& item : relayPins) {
  //   std::map<String, int> cache;
  //   cache["pin"] = item;
  //   cache["state"] = LOW;
  //   relays.push_back(cache);
  // }
  // for (std::map<String, int>& item : relays) {
  //   pinMode(item["pin"], OUTPUT);
  // }
  pinMode(42, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(50, OUTPUT);
  pinMode(52, OUTPUT);

  Serial.print("setup finish\n");
}

void loop() {
    // control relay arccoding to the toggled state
    // int index = 0;
    digitalWrite(44, LOW);
    digitalWrite(48, LOW);
    digitalWrite(46, LOW);
    digitalWrite(50, LOW);
    digitalWrite(52, LOW);
    digitalWrite(42, HIGH);
    delay(1500);

    digitalWrite(42, LOW);
    digitalWrite(44, LOW);
    digitalWrite(48, LOW);
    digitalWrite(46, LOW);
    digitalWrite(50, LOW);
    digitalWrite(52, LOW);
    delay(1500);

    digitalWrite(42, LOW);
    digitalWrite(48, LOW);
    digitalWrite(46, LOW);
    digitalWrite(50, LOW);
    digitalWrite(52, LOW);
    digitalWrite(44, HIGH);
    delay(1500);

    digitalWrite(42, LOW);
    digitalWrite(44, LOW);
    digitalWrite(48, LOW);
    digitalWrite(46, LOW);
    digitalWrite(50, LOW);
    digitalWrite(52, LOW);
    delay(1500);

    digitalWrite(44, LOW);
    digitalWrite(42, LOW);
    digitalWrite(46, LOW);
    digitalWrite(50, LOW);
    digitalWrite(52, LOW);
    digitalWrite(48, HIGH);
    delay(1500);

    digitalWrite(42, LOW);
    digitalWrite(44, LOW);
    digitalWrite(48, LOW);
    digitalWrite(46, LOW);
    digitalWrite(50, LOW);
    digitalWrite(52, LOW);
    delay(1500);

    digitalWrite(44, LOW);
    digitalWrite(48, LOW);
    digitalWrite(42, LOW);
    digitalWrite(50, LOW);
    digitalWrite(52, LOW);
    digitalWrite(46, HIGH);
    delay(1500);

    digitalWrite(42, LOW);
    digitalWrite(44, LOW);
    digitalWrite(48, LOW);
    digitalWrite(46, LOW);
    digitalWrite(50, LOW);
    digitalWrite(52, LOW);
    delay(1500);
    
    digitalWrite(44, LOW);
    digitalWrite(48, LOW);
    digitalWrite(46, LOW);
    digitalWrite(42, LOW);
    digitalWrite(52, LOW);
    digitalWrite(50, HIGH);
    delay(1500);

    digitalWrite(42, LOW);
    digitalWrite(44, LOW);
    digitalWrite(48, LOW);
    digitalWrite(46, LOW);
    digitalWrite(50, LOW);
    digitalWrite(52, LOW);
    delay(1500);

    digitalWrite(44, LOW);
    digitalWrite(48, LOW);
    digitalWrite(46, LOW);
    digitalWrite(50, LOW);
    digitalWrite(42, LOW);
    digitalWrite(52, HIGH);
    delay(1500);

    digitalWrite(42, LOW);
    digitalWrite(44, LOW);
    digitalWrite(48, LOW);
    digitalWrite(46, LOW);
    digitalWrite(50, LOW);
    digitalWrite(52, LOW);
    delay(1500);
    // for (std::map<String, int>& item : relays) {
    //   int relayState = digitalRead(item["pin"]);
    //   int newState = relayState == LOW ? HIGH : LOW;
    //   // relays[index]["state"] = newState;
    //   digitalWrite(item["pin"], newState);
    //   delay(11500);
    //   digitalWrite(item["pin"], (newState == LOW ? HIGH : LOW));
    //   delay(11500);
    //   String consoleMess = "relay" + String(index) + " is " + (newState == LOW ? "opening" : "closing") + "\n";
    //   Serial.println(consoleMess);
      
    //   index ++;
    // }
}