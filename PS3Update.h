#include <PS3BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB usb;
BTD btd(&usb);
PS3BT PS3(&btd, 0x00, 0x1A, 0x7D, 0xDA, 0x71, 0x13); // new

class PS3Comm {
public:
    //digital buttons
    bool up;
    bool down;
    bool left;
    bool right;
    bool L1;
    bool L3;
    bool R1;
    bool R3;
    bool square;
    bool circle;
    bool cross;
    bool tri;
    bool start;
    bool select;
    bool ps;

    //analog buttons
    uint8_t L2 = -1;
    uint8_t R2 = -1;
    uint8_t LeftHatX = -1;
    uint8_t LeftHatY = -1;
    uint8_t RightHatX = -1;
    uint8_t RightHatY = -1;
};

PS3Comm ps3;

void resetAnalog () {
    ps3.L2 = -1;
    ps3.R2 = -1;
    ps3.LeftHatX = -1;
    ps3.LeftHatY = -1;
    ps3.RightHatX = -1;
    ps3.RightHatY = -1;
}

void PS3Setup () {
    #if !defined(_MIPSEL_)
        while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
    #endif

	if (usb.Init() == -1) {
		Serial.println("OSC did not found");
		while (1); //halt
	}

	Serial.println("PS3 bluetooth library started");
	while (!PS3.PS3Connected) {
        Serial.print("\nPS3 connection status: ");
		Serial.print(PS3.PS3Connected);
        Serial.print('\n');
	    usb.Task();
    }
}

void getPS3Data () {

    usb.Task();

    //digital buttons
    ps3.up    = PS3.getButtonPress(UP);
    ps3.right = PS3.getButtonPress(RIGHT);
    ps3.down  = PS3.getButtonPress(DOWN);
    ps3.left  = PS3.getButtonPress(LEFT);
    ps3.select  = PS3.getButtonPress(SELECT);
    ps3.L3    = PS3.getButtonPress(L3);
    ps3.R3    = PS3.getButtonPress(R3);
    ps3.start = PS3.getButtonPress(START);
    ps3.L1    = PS3.getButtonPress(L1);
    ps3.R1    = PS3.getButtonPress(R1);
    ps3.tri   = PS3.getButtonPress(TRIANGLE);
    ps3.circle  = PS3.getButtonPress(CIRCLE);
    ps3.cross  = PS3.getButtonPress(CROSS);
    ps3.square = PS3.getButtonPress(SQUARE);
    ps3.ps = PS3.getButtonClick(PS);

    //analog buttons
    ps3.LeftHatX = PS3.getAnalogHat(LeftHatX);
    ps3.LeftHatY = PS3.getAnalogHat(LeftHatY);
    ps3.RightHatX = PS3.getAnalogHat(RightHatX);
    ps3.RightHatY = PS3.getAnalogHat(RightHatY);
    ps3.L2 = PS3.getAnalogButton(L2);
    ps3.R2 = PS3.getAnalogButton(R2);
}

void printKey (char * str) {
    Serial.print("pressed button: ");
    Serial.print(str);
    Serial.println();
}

void printAnalogPress (char * str, int8_t value) {
    Serial.print(str);
    Serial.print(value);
    if (str == "PS3.L2: " || str == "PS3.R2: ") {
        Serial.print("\n");
    } else {
        Serial.print("\t");
    }
    
}

void printPressedKey (PS3Comm & ps3) {
    if (ps3.up == true) {
        printKey("UP");
    } else if (ps3.down == true) {
        printKey("DOWN");
    } else if (ps3.left == true) {
        printKey("LEFT");
    } else if (ps3.right == true) {
        printKey("RIGHT");
    } else if (ps3.tri == true) {
        printKey("TRIANGLE");
    } else if (ps3.circle == true) {
        printKey("CIRCLE");
    } else if (ps3.square == true) {
        printKey("SQUARE");
    } else if (ps3.cross == true) {
        printKey("CROSS");
    } else if (ps3.select == true) {
        printKey("SELECT");
    } else if (ps3.start == true) {
        printKey("START");
    } else if (ps3.L1 == true) {
        printKey("L1");
    } else if (ps3.R1 == true) {
        printKey("R1");
    } else if (ps3.L3 == true) {
        printKey("L3");
    } else if (ps3.R3 == true) {
        printKey("R3");
    } else if (ps3.ps == true) {
        printKey("PS");
    } else if (ps3.L2) {
        printAnalogPress("PS3.L2: ", ps3.L2);
    } if (ps3.R2) {
        printAnalogPress("PS3.R2: ", ps3.R2);
    }

    // Navigation joystick
    printAnalogPress("LeftHatX: ", PS3.getAnalogHat(LeftHatX));
    printAnalogPress("LeftHatY: ", PS3.getAnalogHat(LeftHatY));
    printAnalogPress("RightHatX: ", PS3.getAnalogHat(RightHatX));
    printAnalogPress("RightHatY: ", PS3.getAnalogHat(RightHatY));
    Serial.print("\n");
}
