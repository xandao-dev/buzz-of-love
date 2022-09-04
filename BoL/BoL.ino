#include <NonBlockingRtttl.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <sstream>

#define SERVICE_UUID "94e66cfe-26e7-410f-acca-e9274d037f7f"
#define CHARACTERISTIC_UUID_RX "33b7b79f-2e46-458a-9cc3-6728e483bad0"
#define CHARACTERISTIC_UUID_TX "d359b810-c08f-4c1a-ac0d-b29d2dda5475"

#define BUZZER_PIN 25

#define tetris "tetris:d=4,o=5,b=160:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6,8e6,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,a"
#define mario "mario:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6"
#define pimp "50 Cent P.I.M.P.:d=4,o=5,b=160:8c6,8c6,8c6,d#6,8d#6,8d#6,8c,8d#6,c6,c6,p,8c,8c6,8c6,8c6,d#6,8d#6,8d#6,8d#6,8d6,8c6,8d#6,d6,p,8g,8d6,8d6,8d6,f6,8f6,8f6,8g,8f6,8f6,8f6,d6,p,8g,8d6,8d6,8d6,8f6,8f6,8f6,8f6,8f6,8d#6,8d6,8d#6,c6,p,8c,8c6,8c6,8c6,d#6,8d#6,8d#6,8c,8d#6,c6,c6,p,8c"
#define jamesBond "James Bond 007:d=4,o=6,b=112:16c.5,32d.5,32d.5,16d.5,8d.5,16c.5,16c.5,16c.5,16c.5,32d#.5,32d#.5,16d#.5,8d#.5,16d.5,16d.5,16d.5,16c.5,32d.5,32d.5,16d.5,8d.5,16c.5,16c.5,16c.5,16c.5,32d#.5,32d#.5,16d#.5,8d#.5,16d.5,16d.5,16d.5,16c.5,32d.5,32d.5,16d.5,8d.5,16c.5,16c.5,16c.5,16c.5,32d#.5,32d#.5,16d#.5,8d#.5,16d.5,16d.5,16c.5,16b.5,2a#5,16g.5,16f.5,2g.5"
#define abbaMoney "Abba - Money Money Money:d=4,o=5,b=112:8e7,8e7,8e7,8e7,8e7,8e7,16e6,16a6,16c7,16e7,8d#7,8d#7,8d#7,8d#7,8d#7,8d#7,16f6,16a6,16c7,16d#7,d7,8c7,8a6,8c7,c7,2a6,32a6,32c7,32e7,8a7"
#define familiaAdams "Adams Family Theme:d=4,o=5,b=160:8c6,f6,8a6,f6,8c6,b,g.6,8f6,e6,8g6,e6,8e,a,f.6,8c6,f6,8a6,f6,8c6,b,g.6,8f6,e6,8c6,d6,8e6,f.6,8c6,8d6,8e6,8f6,1p,8d6,8e6,8f#6,8g6,1p,8d6,8e6,8f#6,8g6,p,8d6,8e6,8f#6,8g6,p,c6,8e6,1f6"
#define becauseIGotHigh "Because I Got High:d=4,o=5,b=100:16g,16g,16g,16g,8g,16g,g,16g,16g,16g,8f#,2e,p,16g,16g,16g,16a,16a,16p,16a,16a,16a,16a,a,16f#,16f#,16f#,8e,2d,2p,a,16b,16b,16b,8b,8a,8g,8g,16g,16d6,16d6,16b,16d6,c6,8p,8e6,16e6,16d6,16d6,8c6,8b,16b,16c6,16c6,8g,8a,16g,16a,16a,16f#,8a,g,16p"
#define takeOnMe "Take On Me:d=8,o=5,b=160:g,g,g,4d#,4c,4f,4f,f,a,a,a#,c6,a#,a#,a#,4f,4d#,4g,4g,g,f,f,g,f,g,g,g,4d#,4c,4f,4f,f,a,a,a#,c6,a#,a#,a#,4f,4d#,4g,4g,g,f,f,g,f"
#define barbieGirl "Barbie Girl:d=4,o=5,b=125:8g#,8e,8g#,8c#6,a,p,8f#,8d#,8f#,8b,g#,8f#,8e,p,8e,8c#,f#,c#,p,8f#,8e,g#,f#"
#define asterixObelix "Asterix & Obelix:d=16,o=7,b=80:f6,c,c,c,8a6,8f6,g6,d,d,d,8a#6,8g6,a6,e,e,e,8c,8a6,f,f,e,d,4c,8p,f6,c,c,c,8a6,8f6,g6,d,d,d,8a#6,8g6,a6,e,e,e,8c,8a6,f,f,e,d,4c"
#define axelF "Axel-F:d=4,o=5,b=125:g,8a#.,16g,16p,16g,8c6,8g,8f,g,8d.6,16g,16p,16g,8d#6,8d6,8a#,8g,8d6,8g6,16g,16f,16p,16f,8d,8a#,2g,p,16f6,8d6,8c6,8a#,g,8a#.,16g,16p,16g,8c6,8g,8f,g,8d.6,16g,16p,16g,8d#6,8d6,8a#,8g,8d6,8g6,16g,16f,16p,16f,8d,8a#,2g"
#define batman "Batman:d=4,o=5,b=200:8f,8f,8e,8e,8d#,8d#,8e,8e,8f,8f,8e,8e,8d#,8d#,8e,8e,8f,8f,8e,8e,8d#,8d#,8e,8e,g#,2g#,p,8f,8f,8e,8e,8d#,8d#,8e,8e,8f,8f,8e,8e,8d#,8d#,8e,8e,8f,8f,8e,8e,8d#,8d#,8e,8e,g#,2g#,p,8f,8f,8e,8e,8d#,8d#,8e,8e,8f,8f,8e,8e,8d#,8d#,8e,8e,8f,8f,8e,8e,8d#,8d#,8e,8e,g#,2g#,p,f"
#define beethoven5th "Beethoven 5th Symphonie:d=4,o=5,b=180:8f,8f,8f,1c#,8p,8d#,8d#,8d#,1c,8p,8f,8f,8f,8c#,8f#,8f#,8f#,8f,8c#6,8c#6,8c#6,2a#,8p,8f,8f,8f,8c,8f#,8f#,8f#,8f,8d#6,8d#6,8d#6,1c6,8f6,8f6,8d#6,8c#6,8c#,8c#,8d#,8f,8f6,8f6,8d#6,8c#6,8c#,8c#,8d#,8f,8f6,8f6,8d#6,c#6,p,a#,p,2f6"
#define beethovenFurElise "Beethoven Fur Elise:d=8,o=5,b=125:32p,e6,d#6,e6,d#6,e6,b,d6,c6,4a.,32p,c,e,a,4b.,32p,e,g#,b,4c.6,32p,e,e6,d#6,e6,d#6,e6,b,d6,c6,4a.,32p,c,e,a,4b.,32p,d,c6,b,2a"
#define livingOnAPrayer "Living On A Prayer:d=4,o=6,b=125:2g.,8p,8g,g,f#,e,d,b.,c.7,8p,c7,8c7,8b,8g,g,a,8g,8g,8a,g.,8p,8g,8g,8g,f#,e,d,b.,c.7,8p,c7,8c7,8b,8g,g,a,1p"
#define coldplayClocks "Coldplay - Clocks:d=8,o=6,b=140:d#7,a#,g,d#7,a#,g,d#7,a#,c#7,a#,f,c#7,a#,f,c#7,a#,c#7,a#,f,c#7,a#,f,c#7,a#,c7,g#,f,c7,g#,f,c7,g#,d#7,a#,g,d#7,a#,g,d#7,a#,c#7,a#,f,c#7,a#,f,c#7,a#,c#7,a#,f,c#7,a#,f,c#7,a#,c7,g#,f,c7,g#,f,c7,g#"
#define finalCountdown "The Final Countdown:d=4,o=5,b=125:16c#6,16b,c#6,f#,p.,16d6,16c#6,8d6,8c#6,b,p.,16d6,16c#6,d6,f#,p.,16b,16a,8b,8a,8g#,8b,a.,16c#6,16b,c#6,f#,p.,16d6,16c#6,8d6,8c#6,b,p.,16d6,16c#6,d6,f#,p.,16b,16a,8b,8a,8g#,8b,a.,16g#,16a,b.,16a,16b,8c#6,8b,8a,8g#,f#,d6,2c#.6,16c#6,16d6,16c#6,16b,1c#6,2p"
#define nellyDilemma "Nelly - Dilemma:d=4,o=6,b=160:a5,8f5,d5,8a5,8p,2g5,p,8e5,8f5,g5,8e5,c5,8g5,8p,2f5,p,8f5,8g5,a5,8f5,8d5,8p,8a5,8p,2g5,8p,8e5,8f5,8e5,8f5,g5,8e5,c5,8g5,8p,2f5"
#define eiffel65Blue "Eiffel 65 - Blue:d=16,o=5,b=140:4a,4a#,8g,8a#,8c6,8f,8a,4a#,8g,8a#,8d6,4d#6,8d6,8c6,4a#,8g,8a#,8c6,8f,8a,4a#,8g,8a#,8d6,4d#6,8d6,8c6,4a#,8g,8a#,8c6,8f,8a,4a#,8g,8a#,8d6,4d#6,8d6,8c6,4a#,8g,8a#,8a,8f,8f,2g"
#define flinstones "Flintstones:d=4,o=5,b=200:g#,c#,8p,c#6,8a#,g#,c#,8p,g#,8f#,8f,8f#,8g,8g#,c#,d#,2f,2p,g#,c#,8p,c#6,8a#,g#,c#,8p,g#,8f#,8f,8f#,8g,8g#,c#,d#,2c#,2p,c6,f,8p,8c#6,8p,8c#6,8c#6,8c6,8c6,8a#,a#,p,a#,d#,8p,8c6,8p,8a#,8a#,16g#.,32p,8g,8a#,16p,g#,p,g#,c#,8p,c#6,8a#,g#,c#,8p,g#,8f#,8f,8f#,8g,8g#,c#,d#,8p,8f,8f#,8g#,c#,d#,8p,8f,8f#,8g#,c#6,d#6,2c#6,8p,32p"
#define ghostbusters "Ghostbusters:d=16,o=5,b=112:g,g,8b,8g,8a,4f.,g,g,g,g,8f,4g.,g,g,8b,8g,8a,4f.,g,g,g,g,8f,8a,8g,4d.,g,g,8b,8g,8a,4f.,g,g,g,g,8f,4g."
#define halloween "Halloween:d=4,o=5,b=180:8d6,8g,8g,8d6,8g,8g,8d6,8g,8d#6,8g,8d6,8g,8g,8d6,8g,8g,8d6,8g,8d#6,8g,8c#6,8f#,8f#,8c#6,8f#,8f#,8c#6,8f#,8d6,8f#,8c#6,8f#,8f#,8c#6,8f#,8f#,8c#6,8f#,8d6,8p,8d6,8g,8g,8d6,8g,8g,8d6,8g,8d#6,8g,8d6,8g,8g,8d6,8g,8g,8d6,8g,8d#6,8g,8c#6,8f#,8f#,8c#6,8f#,8f#,8c#6,8f#,8d6,8f#,8c#6,8f#,8f#,8c#6,8f#,8f#,8c#6,8f#,8d6"
#define harryPotter "Harry Potter:d=8,o=6,b=100:b5,e.,16g,f#,4e,b,4a.,4f#.,e.,16g,f#,4d,f,2b5,p,b5,e.,16g,f#,4e,b,4d7,c#7,4c7,g#,c.7,16b,a#,4a#5,g,2e"
#define indianaJones "Indiana Jones:d=4,o=5,b=250:e,8p,8f,8g,8p,2c.6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,2f.6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,1c6"
#define looneyTunes "Looney Tunes:d=4,o=5,b=140:c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f"
#define pinkyCerebro "Pinky e o Cerebro:d=16,o=6,b=125:b,8e,d#,8e,g,4d#.,b,8e,d#,8e,g,2d#,4e,8e,g,8a#,4a#,a#,8b,a,4g,4f#.,e,8a,g#,8a,b,4g#.,e,8a,g#,8a,b,4g#.,c,8b,4b,b,8b,b,8c#,d#,4e,4d,4c,4b5,4e,4d,4c,4b5,4p,4e,b,a#,b,4c,4b5,8p"
#define popeye "Popeye:d=4,o=5,b=225:8a#,8a#,8a#,8a#,8g#,8p,8g,2a#,8p,8a#,8c6,8g#,8c6,8d#6,8p,8c6,2a#,8p,8a#,8c6,8g#,8c6,8d#6,8d6,8c6,8a#,8c6,8a#,8g,8d#,8a#,8a#,8a#,8a#,8c6,8p,8d6,2d#6"
#define simpsons "Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6"
#define smurfs "Smurfs:d=32,o=5,b=200:4c6,16p,4f6,p,16c6,p,8d6,p,8a#,p,4g,16p,4c6,p,16a,p,8f,p,8a,p,4g,4p,g,g#,a,a#,b,4c6,16p,4f6,p,16c6,p,8d6,p,8a#,p,4g,16p,4c6,p,16a,p,8a#,p,8e,p,4f"
#define starWars "StarWars:d=4,o=5,b=80:8d.,8d.,8d.,8a#4,16f,8d.,8a#4,16f,d.,32p,8a.,8a.,8a.,8a#,16f,8c#.,8a#4,16f,d.,32p,8d.6,8d,16d,8d6,32p,8c#6,16c6,16b,16a#,8b,32p,16d#,8g#,32p,8g,16f#,16f,16e,8f,32p,16a#4,8c#,32p,8a#4,16c#,8f.,8d,16f,a.,32p,8d.6,8d,16d,8d6,32p,8c#6,16c6,16b,16a#,8b,32p,16d#,8g#,32p,8g,16f#,16f,16e,8f,32p,16a#4,8c#,32p,8a#4,16f,8d.,8a#4,16f,d."
#define superman "Superman:d=4,o=6,b=200:8d5,8d5,8d5,8g.5,16p,8g5,2d,8p,8d,8e,8d,8c,1d,8p,8d5,8d5,8d5,8g.5,16p,8g5,2d,8d,8d,8e,8c,8g5,8e,2d.,p,8g5,8g5,8g5,2f#.,d.,8g5,8g5,8g5,2f#.,d.,8g5,8g5,8g5,8f#,8e,8f#,2g.,8g5,8g5,8g5,2g.5"
#define tomJerry "Tom e Jerry:d=8,o=6,b=160:d,e,b5,4d.,p,4d,e,b5,4d.,p,c,b5,c,d,1e.,32p,d,e,f#,g,f#.,16g,a,4d.,p,4d,e,b5,2d,4f#,g#,d#,2f#,4f#,g#,e,f#,d#,e,c#,d#,b5.,16a5,b5,4d.,p,4d,e,b5,2d,4c,d,a5,4c,c#,4d,2a5,1g5"
#define sweetChild "Sweet Child O Mine:d=8,o=5,b=140:c,c6,g,f,f6,g,e6,g,c,c6,g,f,f6,g,e6,g,d,c6,g,f,f6,g,e6,g,d,c6,g,f,f6,g,e6,g,f,c6,g,f,f6,g,e6,g,f,c6,g,f,f6,g,e6,g,c,c6,g,f,f6,g,e6,g,c,c6,g,f,f6,g,e6,g"
#define tururu "Naruto - tururu:d=4,o=5,b=73:16f#,16e,2b4,8p,8e,8g,4a,2e.,2g,8f#,8g,8a,8d,2b,2a,16f#,16e,2b4,8p,8e,8g,4a,2e.,2g,8f#,8g,8a,8d,2e,8p,8e,8f#,8g,8a,8b,8e,4g,4f#,8e,1e,8g,8a,8g,4f#.,4b,2b,2a,8a,8b,8e,4g,4f#,8e,1e,8g,8a,8g,4f#.,4b4,2e."
#define dragoste "Dragoste:d=8,o=5,b=160:4d6,c#6,4d6,4b.,b,c#6,b,a,2f#,d6,f#6,e6,d6,2a,a,e6,d6,c#6,d6,c#6,4b,b4,c#,b4,2b,p,b4,c#,b4,2a,p,b4,c#,b4,2a,p,b4,c#,b4,4d6,b"
#define allStar "All Star:d=4,o=5,b=40:16g.,32d.6,32b.,16b.,32a.,32g.,32g.,16c.6,32b.,32b.,32a.,32a.,16g.,32g.,32d.6,32b.,32b.,32a.,32a.,32g.,32g.,16e.,d,32g.,32g.,32d.6,32b.,32b.,32a.,32a.,32g.,32g.,16c.6,32b.,32b.,32a.,32a.,32g.,32g.,16d.6,32b.,32b.,16a.,32g.,32g.,16a.,8e"
#define lionKing "The Lion King:d=4,o=5,b=140:1p,1p,1p,g,32p,8a,32p,8b.,16p,a,32p,16b,32p,8c.6,16p,16b.,16p,8a.,16p,8g.,32p,16a,32p,16b.,16p,16d.,8p,16d.,8p,16d.,8p,1d,8p,g,32p,8a,32p,8b.,16p,a,32p,16b,32p,8c.6,16p,16b.,16p,8a.,16p,8g.,32p,16a,32p,16b.,16p,16d.,8p,16d.,8p,16d.,8p,1d"
#define piratesCaribbean "Pirates Of The Caribbean:d=4,o=6,b=200:8d,8e,2f,8g,8a,g,f,e,f,g,a,g,p,8f,8g,a,p,8g,8f,e,f,e,d,p,8e,8c,d,8p,p,8d,8e,f,p,8e,8f,g,f,g,a,g,f,d"
#define jingleBell1 "Jingle Bell Rock:d=4,o=6,b=112:8a.5,8c.,16d.,16d,8c.,8g_5,16g_5,16c,16p,d.,8p,16a5,8b5,16a5,8g.5,2c.,8p,8c,16c,8c.,16p,8b5,16b5,8b.5,8a5,16b5,8a5,e.5,8p,8a5,16b5,8a5,8e.5,8g5,16p,8a5,16b5,8a5,f.5,8p,8d5,8e.5,16f5,16g5,8p,8a5,16g5,8d5,16e5,16f5,16p,2g5"
#define jingleBell2 "Jingle Bell:d=4,o=6,b=35:32c.6,32c.6,16c.6,32c.6,32c.6,16c.6,32c.6,32d#.6,32g#.5,32a#.5,8c.6,32c#.6,32c#.6,16c#6,32c#6,32c#.6,32c.6,32c.6,32c6,32c6,32c.6,32a#.5,32a#.5,32c.6,16a#.5,16d#.6,32c.6,32c.6,16c.6,32c.6,32c.6,16c.6,32c.6,32d#.6,32g#.5,32a#.5,8c.6,32c#.6,32c#.6,16c#6,32c#6,32c#.6,32c.6,32c.6,32c6,32c6,32d#.6,32d#.6,32c#.6,32a#.5,8g#5,32g#.5,32g#.5,32f.6,32d#.6,32c#.6,8g#5,32g#.5,32g#.5,32f.6,32d#.6,32c#.6,8a#5,32a#.5,32a#.5,32f#.6,32f.6,32d#.6,32g#.6,32g#.6,32g#.6,32g#6,32g#6,32a#.6,32g#.6,32f#.6,32d#.6,16c#.6"
#define imagine "Jhon Lennon - Imagine:d=8,o=6,b=112:g.,c.,g.,c.,g.,c.,b.,c.,a.,c.,a.,c.,a.,c.,16a.,16a#.,b.,g.,c.,g.,c.,g.,c.,b.,c.,a.,c.,a.,c.,a.,c.,16a.,16a#.,b"
#define laCucaracha "La Cucaracha:d=16,o=6,b=127:c5,c5,c5,8f5,8a5,p,c5,c5,c5,8f5,8a5,p,f5,f5,e5,e5,d5,d5,8c5,p,p,c5,c5,c5,8e5,8g5,p,c5,c5,c5,8e5,8g5,p,c,d,c,a_5,a5,g5,8f5"
#define inTheEnd1 "Linkin Park - InTheEnd 1:d=4,o=6,b=200:d.5,32p,a.5,32p,a.5,32p,f.5,32p,e.5,32p,e.5,32p,e.5,32p,e.5,32p,8e.5,8f.5,d.5,32p,a.5,32p,a.5,32p,f.5,32p,e.5,32p,e.5,32p,e.5,32p,8e.5,8f.5,d.5,32p,a.5,32p,a.5,32p,f.5,32p,e.5,32p,e.5,32p,e.5,32p,8e.5,8f.5,d.5"
#define inTheEnd2 "Linkin Park - InTheEnd 2:d=8,o=6,b=112:4c#,4d#,4f.,f,f.,c#.,2c#,4p,16c#,c#,d#,4f.,f,16f,16d#,f,f#,f,4d#,p,a#5,4c#,4d#,4f.,f,f.,c#.,2c#,4p,16c#,c#,d#,4f.,f,16f,16d#,f,d#,4f#,f,d#,4c#."
#define funkyTown "Funky Town:d=4,o=4,b=125:8c6,8c6,8a#5,8c6,8p,8g5,8p,8g5,8c6,8f6,8e6,8c6,2p,8c6,8c6,8a#5,8c6,8p,8g5,8p,8g5,8c6,8f6,8e6,8c6"
#define blackOrWhite "Michal J. - Black Or White:d=8,o=6,b=125:32p,b5,e,g#,a,g#,e,b5,a5,g#5,a5,2b5,p,16b5,16b5,e,g#,a,g#,e,b5,a5,g#5,a5,4b.5,p,16b5,16b5,c#,16c#,e,16e,f#.,16e,e,p,16e,16e,c#,c#,e,e,f#,e,f#,4g#."
#define billieJean "Michael J. - Billie Jean e:d=16,o=6,b=125:8e,8e,8d,4p,8b5,32e,p,32p,4e,4d,2p,32e,p,32p,32e,p,32p,8e,8d,4p,8b5,8e,8g,p,32p,4a,8g,8f#,2e,8p,p,8e,8p,8e,4b,4a,8e,4c,2b5,b,g,e,c,2b5"
#define thriller "Michael J. -Thriller:d=8,o=5,b=112:b,d6,b,e6,16p,2d6,4p,d6,16p,c#6,16p,b,4p,p,b,b,32a,16p,32p,a,16g,g,16p,e,g,a,b,a,a,16g,b"
#define happyBirth1 "Happy Birthday 1:d=4,o=5,b=140:8g.5,16g5,4a5,4g5,4c6,2b5,8g.5,16g5,4a5,4g5,4d6,2c6,8g.5,16g5,4g6,4e6,4c6,4b5,4a5,8f.6,16f6,4e6,4c6,4d6,2c6,8g.5,16g5,4a5,4g5,4c6,2b5,8g.5,16g5,4a5,4g5,4d6,2c6,8g.5,16g5,4g6,4e6,4c6,4b5,4a5,8f.6,16f6,4e6,4c6,4d6,2c6"
#define happyBirth2 "Happy Birthday 2:d=4,o=5,b=200:8e,8p,8e,f#.,e.,a.,g#.,p,8e6,8p,8e6,f#.6,e.6,b.6,a.6,p,8e,8p,16e,16a,e.6,c#.6,8a,8b,8a,g#.,f#.,p,8d6,8p,8d6,c#.6,a.,p,b.,a,32c6,8c#6,8d6,8d#6,8e6,8f#6,a6,16p,32a,16c#6,8a6"
#define enternainer "The Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6"
#define yankeeDoodle1 "Yankee Doodle Dandy 1:d=16,o=6,b=63:c,c,d,e,c,e,d,g5,c,c,d,e,8c,8b5,c,c,d,e,f,e,d,c,b5,g5,a5,b5,8c,8c,a.5,32b5,a5,g5,a5,b5,8c,g.5,32a5,g5,f5,8e5,8g5,a.5,32b5,a5,g5,a5,b5,c,a5,g5,c,b5,d,8c,8c"
#define yankeeDoodle2 "Yankee Doodle Dandy 2:d=8,o=6,b=125:g,g,a,b,g,b,4a,g,g,a,b,4g,4f#,g,g,a,b,c7,b,a,g,f#,d,e,f#,4g,4g,e.,16f#,e,d,e,f#,4g,d.,16e,d,c,4b5,4d,e.,16f#,e,d,e,f#,g,e,d,g,f#,a,4g,4g"
#define mortalKombat "Mortal Kombat:d=4,o=6,b=35:32a#5,32a#5,32c#6,32a#5,32d#6,32a#5,32f6,32d#6,32c#6,32c#6,32f6,32c#6,32g#6,32c#6,32f6,32c#6,32g#5,32g#5,32c6,32g#5,32c#6,32g#5,32d#6,32c#6,32f#5,32f#5,32a#5,32f#5,32c#6,32f#5,32c#6,32c6,32a#5,32a#5,32c#6,32a#5,32d#6,32a#5,32f6,32d#6,32c#6,32c#6,32f6,32c#6,32g#6,32c#6,32f6,32c#6,32g#5,32g#5,32c6,32g#5,32c#6,32g#5,32d#6,32c#6,32f#5,32f#5,32a#5,32f#5,32c#6,32f#5,32c#6,32c6"
#define heyYa "Hey Ya:d=16,o=6,b=160:8e.7,p,4d7,8p,a,p,4b,c7,p,4b,a,p,g,p,a,p,4b,c7,p,4b,a,p,g,p,a,p,8b.,p,8c7,8p,2b.,4p,8e.7,p,4d7,8p,a,p,4b,c7,p,4b,a,p,g,p,a,p,4b,c7,p,4b,a,p,g,p,a,p,8b.,p,8c7,8p,2b."
#define bohemian "Bohemian Rapsody:d=4,o=5,b=80:16e,2e.,16p,8c,8d,16e,2e,16p,8p,16d,16e,8f,16g,16f,16p,8e,d,16p,8d,8e,8f,16g,16f,16p,8e,2d,16p,16e,2e,16p,8p,8e,8g,8b.,16a,2a,8p,8c6,8c6,8c6,8c6,8c6,8c.6,16a,8f.,8e.,2d.,8p,16a,2a,16p,8p,8g,16a,16a#,2a.,8p,16a,16a,8a#.,16a#,8a#,8a,g.,16p,16c,8c,8g,8g,8a,8a,8a#,8a#,8c6,16a#,a,16p,16g,16a,c.6,16g,16a,2f,16c#,8d#,8c#.,16d#,2c"
#define weAreChampions "We Are The Champions:d=4,o=5,b=100:2f,8e,8f,e,c,8p,8a4,d,a.4,2p,8c,2f,8g,8a,c6,a.,8d.,8e,2c,2p,d.,c,8d,c.,a#4,8p,a#.,a,8a#,a.,g,8p,a.,f,8a#,a.,8f,8p,8a#,g#.,f.,8a#,g#.,f.,2p"
#define otherSide "Other Side:d=4,o=5,b=125:e.,e.,e,a.,a,8b,8a,8g,1e,d.,d.,d,e.,e.,e,a.,a,8b,8a,8g,e.,e,8e,e,d.,d.,d,e.,e.,e,a.,a,8b,8a,8g,e.,e,8e,e,d.,d.,d,e.,e.,e,a.,a.,a,e.6,e.6,e6,b,b,b,b,a"
#define underBridge "Under The Bridge:d=4,o=5,b=100:8g#,8g#,8g#,f#,8e,8p,8b4,8c#,8e,8c#,8f#,8p,8e,p,8g#,8g#,8g#,8f#,8p,8e,8p,8b4,8c#,8e,8c#,8f#,p,8e,8f#,8g#,8g#,8g#,8f#,8p,8e,8p,8b4,8c#,8e,8c#,8f#,8p,8e,p,8g#,8b,8c#6,8f#,8p,8e.,8p,8b4,8c#,8e,8c#,2f#"
#define eyeTiger "Eye Of The Tiger:d=4,o=5,b=112:8c6,16c,16c,16c,16p,16c,16c,16c6,8p,16a#,8p,16c6,16p,16c,16p,16c,16c,16c,16p,16c,16c,16c6,8p,16a#,8p,16c6,16p,16c,16p,16c,16c,16c,16p,16c,16c,16c6,8p,16a#,8p,2g#.,16c,16c,16c,16p,16c,16c,8c6,16c,16c,16c,16p,16c,16c,16c6,8p,16a#,8p,16c6,16p,16c,16p,16c,16c,16c,16p,16c,16c,16c6,8p,16a#,8p,16c6,16p,16c,16p,16c,16c,16c,16p,16c,16c,16c6,8p,16a#,8p,2g#.,16c,16c,16c,16p,16c,16c,8c"
#define rockyGonnaFly "Rocky 2 - Gonna Fly:d=4,o=5,b=125:16e,8g,16p,2a,8p,16a,8b,16p,2e,16p,32p,16e,8g,16p,2a,16p,32p,16a,8b,16p,1e,8p,16p,16d6,16c6,8d6,16p,16c6,16d6,e6,p,16c6,16c6,8b,16b,8a,16a,g,8f6,1e6"
#define stairwayHeaven "Stairway to Heaven:d=4,o=5,b=63:8a6,8c6,8e6,8a6,8b6,8e6,8c6,8b6,8c7,8e6,8c6,8c7,8f#6,8d6,8a6,8f#6,8e6,8c6,8a6,c6,8e6,8c6,8a,8g,8g,8a,a"
#define contra "Contra:d=4,o=6,b=285:a#5,a#5,c#,a#5,e.,d#.,c#,a#5,a#5,c#,a#5,e.,d#.,c#,a#5,a#5,c#,a#5,e.,d#.,c#,a#5,a#5,c#,a#5,d#.,e.,f,c,c,d#,c,f#.,f.,d#,c,c,d#,c,f#.,f.,d#,c,c,d#,c,f#.,f.,d#,c,c,d#,c,f.,f#.,g"
#define donkeyKong "Donkey Kong:d=4,o=5,b=125:32p,8c,8c,a.,p,8f,8g,8f,d.,p,8d,8d,a#.,p,8g,8a,8g,e.,p,8e,8e,c.6,p,8a,8a#,8c6,d.6,p,8f,8g,a,f,8p,8e,8f,8g,d."
#define pacman "Pacman:d=32,o=5,b=112:32p,b,p,b6,p,f#6,p,d#6,p,b6,f#6,16p,16d#6,16p,c6,p,c7,p,g6,p,e6,p,c7,g6,16p,16e6,16p,b,p,b6,p,f#6,p,d#6,p,b6,f#6,16p,16d#6,16p,d#6,e6,f6,p,f6,f#6,g6,p,g6,g#6,a6,p,b.6"
#define zelda "Zelda - Majora Mask:d=4,o=5,b=200:2a#,2f,p,8a#,8a#,8c6,8d6,8d#6,2f6,2p,f6,f6,8f#6,8g#6,2a#6,2p,a#6,8a#6,8p,8g#6,8f#6,g#.6,8f#6,2f6,2p,2f6,d#6,8d#6,8f6,2f#6,2p,f6,d#6,c#6,8c#6,8d#6,2f6,2p,d#6,c#6,c6,8c6,8d6,2e6,2p,2g6,1f6"
#define centuryFox "20th Century Fox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b"

const char *songs[] = {"", tetris, mario, pimp, jamesBond, abbaMoney, familiaAdams, becauseIGotHigh, takeOnMe,
                       barbieGirl, asterixObelix, axelF, batman, beethoven5th, beethovenFurElise, livingOnAPrayer,
                       coldplayClocks, finalCountdown, nellyDilemma, eiffel65Blue, flinstones, ghostbusters, halloween,
                       harryPotter, indianaJones, looneyTunes, pinkyCerebro, popeye, simpsons, smurfs, starWars, superman,
                       tomJerry, sweetChild, tururu, dragoste, allStar, lionKing, piratesCaribbean, jingleBell1, jingleBell2,
                       imagine, laCucaracha, inTheEnd1, inTheEnd2, funkyTown, blackOrWhite, billieJean, thriller, happyBirth1,
                       happyBirth2, enternainer, yankeeDoodle1, yankeeDoodle2, mortalKombat, heyYa, bohemian, weAreChampions,
                       otherSide, underBridge, eyeTiger, rockyGonnaFly, stairwayHeaven, contra, donkeyKong, pacman, zelda,
                       centuryFox};
int currentMusic = 0;
int nMusicas = sizeof(songs) / sizeof(char *) - 1;
bool deviceConnected = false;
bool repeatSong = false;
bool randomPlay = false;

class ServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic)
  {
    std::string rxValue = characteristic->getValue();

    if (rxValue.length() > 0)
    {
      for (int i = 0; i <= nMusicas; i++)
      {
        std::string currentMusicString;
        std::stringstream ss;
        ss << i;
        currentMusicString = ss.str();

        if (rxValue == currentMusicString)
          currentMusic = i;
      }
      if(rxValue.find("repeaton")!=-1)
        repeatSong = true;
      if(rxValue.find("repeatoff")!=-1)
        repeatSong = false;
      if(rxValue.find("randomon")!=-1)
        randomPlay = true;
      if(rxValue.find("randomoff")!=-1)
        randomPlay = false;  
    }
  }
};

void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);

  BLEDevice::init("Buzz Of Love"); // nome do dispositivo bluetooth

  BLEServer *server = BLEDevice::createServer(); //cria um BLE server

  server->setCallbacks(new ServerCallbacks()); //seta o callback do server

  BLEService *service = server->createService(SERVICE_UUID); //cria o service do BLE

  BLECharacteristic *characteristic = service->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE); // Cria um BLE Characteristic para recebimento de dados

  characteristic->setCallbacks(new CharacteristicCallbacks());

  service->start();

  server->getAdvertising()->start(); // Inicia a descoberta do ESP32
}

void loop()
{
  if(deviceConnected)
  {
    if ((currentMusic != 0 || randomPlay) && currentMusic <= nMusicas)
    {
      rtttl::begin(BUZZER_PIN, songs[currentMusic]);
      while (currentMusic != 0 && currentMusic <= nMusicas)
      {
        rtttl::play();
        if (rtttl::done())
          break;
      }
      if(randomPlay)
        currentMusic = random(1,nMusicas+1);
      else
        currentMusic = 0;
      rtttl::stop();
    }
    
  }
}
