#include <WiFi.h>  // Include WiFi library for ESP32's WiFi functionality
#include <VS1053.h>  // Include library to control the VS1053 MP3 decoder
#include <U8g2lib.h>  // Include library for controlling the OLED display

// Define the VS1053 MP3 decoder pins
#define VS1053_CS     32  // Chip Select for VS1053
#define VS1053_DCS    33  // Data Command Select for VS1053
#define VS1053_DREQ   35  // Data Request pin for VS1053

// Button pins to switch between radio stations
#define BUTTON_NEXT  13  // Pin for the 'next station' button
#define BUTTON_PREV  12  // Pin for the 'previous station' button

// OLED Display setup with I2C communication
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  // Create OLED display object

// WiFi settings: replace with your own network credentials
const char *ssid = "000";  //
const char *password = "000";  //"http://icecast.vrtcdn.be/ra2ant-high.mp3","2 vrt Ant 2",1},

//{"http://icecast.vrtcdn.be/radio1-high.mp3","3 VRT radio 1",1}, 


// Radio station details " 1 RADIO MINERVA"," 2 VRT RADIO-1",
const char* stationNames[] = {
" 1 RADIO MINERVA",
" 2 VRT RADIO-1",
" 3 VRT CLASSICS",
" 4 181 fm",
" 5 Klassieke hits 181",
" 6 VRT RADIO 2 Ant",
" 7 VRT klara",
" 8 Omroep NL",
" 9 ERT Kosmos",
"10 FFH Lounge",
"11 FFH Summer",
"12 Hitfm",
"13 Greek",
"14 Beatles_128k",
"15 Radio centraal antw",
"16 VRT NIEUWS",
"17 Antenne Bayern",
"18 willy",
"19 RADIO 2 Antwerpen",
"20 104.6 RTL",
"21 Radio Horo",
"22 181FM CLASSICAL",
"23 Relaxing Jazz",
"24 Ibiz chillout",
"25 Latina Bachata",
"26 Zona Rumbera",
"27 Lounge Motion",
"28 Salsa4te",
"29 Chillout Loung",
"30 LolliRadio Italy",
"31 Platino Radio",
"32 Latina Bachata",
"33 Der Hitsender",
"34 Der Rapsender",
"35 Radio paradise 1",
"36 Radio Paradise 2",
"37 Radio Paradise 3",
"38 Radio Paradise 4",
"39 Radio Paradise 5",
"40 RMC",
"41 BFM Business",
"42 Europe 2", 
"43 Skyrock",
"44 Kiss FM",
"45 Jazz Radio",
"46 willy",
"47 Joe",
"48 113.fm The Lounge",



 
};  // Array of station names"streaming.radiominerva.be"icecast-servers.vrtcdn.be"

const char* stationHosts[] = {
"streaming.radiominerva.be", 
"icecast-servers.vrtcdn.be",
"icecast-servers.vrtcdn.be",
"listen.livestreamingservice.com",
"listen.livestreamingservice.com",
"icecast-servers.vrtcdn.be",
"icecast-servers.vrtcdn.be",
"icecast.omroep.nl",
"radiostreaming.ert.gr",
"mp3.ffh.de",
"mp3.ffh.de",
"stream.hitfm.be",
"realfm.live24.gr",
"listen.181fm.com",//14
"streams.movemedia.eu",//15
"progressive-audio.vrtcdn.be",//16
"s2-webradio.antenne.de",//17
"icecast-qmusicbe-cdp.triple-it.nl",//18
"vrt.streamabc.net",//19
"rtlberlin.streamabc.net",//20
"g5.turbohost.eu:8020",//21
"listen.181fm.com",//22
"stream-02-eu.relaxingjazz.com",//23
"media-ice.musicradio.com",//24
"latinabachata.ice.infomaniak.ch",//25
"stream20.usastreams.com",//26
"vm.motionfm.com",//27
"radiomusic.newradio.it",//28
"strm112.1.fm",//29
"stream.lolliradio.net",//30
"platinoradio-henrsosa.radioca.st",//31
"latinabachata.ice.infomaniak.ch",//32
"derhitsender.stream.laut.fm",//33
"derrapsender.stream.laut.fm",//34
"stream.radioparadise.com",//35
"stream.radioparadise.com",//36
"stream.radioparadise.com",//37
"stream.radioparadise.com",//38
"stream.radioparadise.com",//39
"audio.bfmtv.com",//40
"audio.bfmtv.com",//41
"europe2.lmn.fm",//42
"icecast.skyrock.net",//43
"kissfm2.ice.infomaniak.ch",//44
"jazzradio.ice.infomaniak.ch",//45
"icecast-qmusicbe-cdp.triple-it.nl",//46
"icecast-qmusicbe-cdp.triple-it.nl",//47
"50.31.185.139",//48


};  // Host URLs for the stations"/minerva"/radio1-high.mp3"

const char* stationPaths[] = {"/minerva",
"/radio1-high.mp3",
"/radio1_classics_high.mp3",
"/181-goodtime_128k.mp3",
"/181-greatoldies_128k.mp3",
"/ra2ant-high.mp3",
"/klaracontinuo-high.mp3",
"/radio1-bb-mp3",
"/ert-kosmos",
"/ffhchannels/hqlounge.mp3",
"/ffhchannels/hqsummerfeeling.mp3",
"/hitfm",
"/realfm",
"/181-beatles_128k.mp3",//14
"/centraal",//15
"/content/fixed/11_11niws-snip_hi.mp3",//16
"/antenne",//17
"/willy.mp3",//18
"/vrt-radio2antwerpen-mp3-128-8732799?sABC=68p9667o%230%230qr060035r0293noso0s5rn0s08r177q%23&aw_0_1st.playerid=&amsparams=playerid:;skey:1758029435",//19
"/rtlb-1046rtllive-mp3-192-4947175?sABC=68p97q18%230%230qr060035r0293noso0s5rn0s08r177q%23&aw_0_1st.playerid=&amsparams=playerid:;skey:1758035224",//20
"/stream256",//21
"/181-classical_128k.mp3",//22
"/stream/3/",//23
"/ChillMP3",//24
"/latinabachata.mp3",//25
"/8198/stream",//26
"/motionthree_aacp",//27
"/salsa4te",//28
"/chilloutlounge_mobile_mp3",//29
"/lolli_italia.mp3?FLID=3&amp;aw_0_req.gdpr=true&amp;type=.mp3",//30
"/stream",//31
"/latinabachata.mp3",//32
"/derhitsender?t302=2025-09-18_14-34-24&uuid=68574e97-e166-49c9-8ecb-c414a89c2854",//33
"/derrapsender?t302=2025-09-18_14-37-43&uuid=f72b407d-6484-436a-ae0f-5300937ae61b",//34
"/aac-128",//35
"/mellow-128",//36
"/rock-128",//37
"/global-128",//38
"/radio2050-128",//39
"/rmcradio_128.mp3",//40
"/bfmbusiness_128.mp3",//41
"/europe2.aac",//42
"/s/natio_aac_128k",//43
"/kissfm2-128.mp3",//44
"/jazzradio-high.mp3",//45
"/willy.mp3",//46
"/joe.mp3",//47
"/1064",//48



};  // Paths to the radio streams

int currentStation = 0;  // Index of the currently playing station
const int totalStations = sizeof(stationNames) / sizeof(stationNames[0]);  // Calculate the number of available stations

// VS1053 MP3 player object
VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);  // Create VS1053 object to control MP3 playback
WiFiClient client;  // WiFi client object to connect to the radio stream

//Variables for scrolling text on the OLED display
int textPosition = 128;  // Initial text position for scrolling  128
unsigned long previousMillis = 0;  // Store the last time the display was updated
const long interval = 50;  // Time interval for updating the display (50 ms)

void setup() {
    Serial.begin(115200);  // Start the serial monitor for debugging

    // Wait for VS1053 and PAM8403 amplifier to power up
    delay(1000);//was 2000

    u8g2.begin();  // Initialize the OLED display
    u8g2.setFlipMode(1);  // Flip the display 180 degrees
    u8g2.setFont(u8g2_font_ncenB08_tr);  // Set font for OLED display

    // Display startup messages on OLED
   u8g2.clearBuffer();
   u8g2.setFont(u8g2_font_profont17_mr);
   u8g2.drawStr(0, 16, "Start Radio...");  // Initial message
   u8g2.drawStr(20,30, "12/5/2026");
   u8g2.sendBuffer();
   delay(1000);//was 2000

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont17_mr);
    u8g2.drawStr(0, 16, "Start Engine...");  // Second message was 16
    u8g2.sendBuffer();
    delay(1000);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont17_mr);
    u8g2.drawStr(0, 16, "  WiFi... OK");  // WiFi connection message was 16
    u8g2.sendBuffer();

    Serial.println("\n\nSimple Radio Node WiFi Radio");  // Debug message in the serial monitor

    SPI.begin();  // Initialize SPI communication for VS1053

    player.begin();  // Start the VS1053 decoder
    if (player.getChipVersion() == 4) {  // Check for correct version of VS1053
        player.loadDefaultVs1053Patches();  // Load patches for MP3 decoding if needed
    }
    player.switchToMp3Mode();  // Switch VS1053 to MP3 decoding mode
    player.setVolume(90);  // Set the volume (range: 0-100)was 100 nu 50

    Serial.print("Connecting to SSID ");
    Serial.println(ssid);  // Debug message: attempting WiFi connection
    WiFi.begin(ssid, password);  // Start WiFi connection

    // Disable WiFi power saving mode for a more stable connection
    WiFi.setSleep(false);

    // Attempt to connect to WiFi with retries
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");  // Print dots to indicate connection progress
        attempts++;
    }

    // Check if WiFi connection is successful
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());  // Print the assigned IP address

        // Display success message on OLED
       u8g2.clearBuffer();
       u8g2.setFont(u8g2_font_profont17_mr);
        u8g2.drawStr(0,12, "Pauls WEBRADIO ");//was 16 paul 4.10.2025
        u8g2.drawStr(20,30, "48 stations");//was 16 paul
        u8g2.sendBuffer();
        delay(1000);//was 4000 nu 1000
    } else {
        // Display failure message on OLED if WiFi connection fails
        Serial.println("WiFi not connected");
        u8g2.clearBuffer();
        u8g2.drawStr(0, 16, "Not Connected");
        u8g2.sendBuffer();
        delay(1000);//was 2000 nu 1000
    }

    // Initialize button pins with pull-up resistors
    pinMode(BUTTON_NEXT, INPUT_PULLUP);
    pinMode(BUTTON_PREV, INPUT_PULLUP);

    // Set font for displaying station names
    u8g2.setFont(u8g2_font_profont22_mr);//was 17 22 29 kan ook

    displayStation();  // Display the initial station name on the OLED
    connectToHost();  // Connect to the radio station stream
}

void loop() {
    // Reconnect if the WiFi client is disconnected
    if (!client.connected()) {
        Serial.println("Reconnecting...");
        connectToHost();  // Attempt to reconnect to the stream
    }

    // Read data from the radio stream and send it to the VS1053 decoder for playback
    if (client.available() > 0) {
        uint8_t buffer[32];
        size_t bytesRead = client.readBytes(buffer, sizeof(buffer));  // Read data from stream
        player.playChunk(buffer, bytesRead);  // Play the received audio data
    }

    handleButtons();  // Check if buttons are pressed and switch stations accordingly
    scrollText();  // Scroll the station name on the OLED display
}

void connectToHost() {
    // Connect to the current radio station's server
    Serial.print("Connecting to ");
    Serial.println(stationHosts[currentStation]);

    if (!client.connect(stationHosts[currentStation], 80)) {
        Serial.println("Connection failed");  // Display error if connection fails
        return;
    }

    // Send HTTP request to the server to get the radio stream
    Serial.print("Requesting stream: ");
    Serial.println(stationPaths[currentStation]);

    client.print(String("GET ") + stationPaths[currentStation] + " HTTP/1.1\r\n" +
                 "Host: " + stationHosts[currentStation] + "\r\n" +
                 "Connection: close\r\n\r\n");

    // Skip the HTTP headers in the response
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
            break;  // End of headers
        }
    }

    Serial.println("Headers received");  // Debug message: headers successfully received
}

void handleButtons() {
    static bool lastButtonNextState = HIGH;  // Track the previous state of the 'next' button
    static bool lastButtonPrevState = HIGH;  // Track the previous state of the 'previous' button

    bool currentButtonNextState = digitalRead(BUTTON_NEXT);  // Read current state of 'next' button
    bool currentButtonPrevState = digitalRead(BUTTON_PREV);  // Read current state of 'previous' button

    // If 'next' button is pressed (LOW), switch to the next station
    if (lastButtonNextState == HIGH && currentButtonNextState == LOW) {
        nextStation();
    }
    // If 'previous' button is pressed (LOW), switch to the previous station
    if (lastButtonPrevState == HIGH && currentButtonPrevState == LOW) {
        previousStation();
    }

    lastButtonNextState = currentButtonNextState;  // Update the last state for the 'next' button
    lastButtonPrevState = currentButtonPrevState;  // Update the last state for the 'previous' button
}

//void nextStation() {
//    currentStation = (currentStation + 1) % totalStations;  // Move to the next station (wrap around)
//   displayStation();  // Update the OLED display with the new station name
//    connectToHost();  // Connect to the new station
//}


void nextStation() {
    currentStation = (currentStation + 1) % totalStations;
    displayStation();
    connectToHost();
}

void previousStation() {
    currentStation = (currentStation - 1 + totalStations) % totalStations;
    displayStation();
    connectToHost();
}

void displayStation() {
    textPosition = 128;  // Reset text position to start from the right
    u8g2.clearBuffer();
   //u8g2.drawLine(0, 0, 127, 0);
   u8g2.drawLine(0, 31, 127, 31);
    u8g2.setCursor(textPosition, 22);
    u8g2.print(stationNames[currentStation]);
    u8g2.sendBuffer();
}

void scrollText() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        textPosition--;  // Move text to the left
        if (textPosition < -u8g2.getUTF8Width(stationNames[currentStation])) {
            textPosition = 128;  // Reset position to start from the right again
        }

        u8g2.clearBuffer();
        //u8g2.drawLine(0, 0, 127, 0);
       u8g2.drawLine(0, 31, 127, 31);
        u8g2.setCursor(textPosition, 22);
        u8g2.print(stationNames[currentStation]);
        u8g2.sendBuffer();
    }
}
