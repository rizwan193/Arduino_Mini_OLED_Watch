#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#define OLED_reset LED_BUILTIN
#define screen_width 128
#define screen_height 32

Adafruit_SSD1306 display (screen_width, screen_height, &Wire, OLED_reset);
RTC_DS1307 rtc;

#define button1 11
#define button2 12
#define powerRead A1
#define batteryRead A2
float powerVoltage = 0.0;
float batteryVoltage = 0.0;
int yearupg, monthupg, dayupg, hourupg, minuteupg, secondupg;
int menu = 0;
char DAY [7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thrusday", "Friday", "Saturday"};

const unsigned char battery [] PROGMEM = {
  0x1e, 0x00, 0x1e, 0x00, 0x7f, 0x80, 0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0xf7, 0xc0, 0xf7, 0xc0, 
  0xe1, 0xc0, 0xe1, 0xc0, 0xfb, 0xc0, 0xfb, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0x7f, 0x80
}; // Battery Icon

const unsigned char no_battery [] PROGMEM = {
  0x1e, 0x00, 0x12, 0x00, 0x3f, 0x40, 0x20, 0x80, 0x21, 0x00, 0x23, 0x00, 0x23, 0x00, 0x25, 0x00, 
  0x29, 0x00, 0x11, 0x00, 0x21, 0x00, 0x21, 0x00, 0x61, 0x00, 0xa1, 0x00, 0x21, 0x00, 0x3f, 0x00
};   //No battery indicator

const unsigned char chargedBattery [] PROGMEM = {
  0x7f, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xfe, 0x00, 0xdc, 0xe7, 0x72, 0x00, 0xdc, 0xef, 0x7b, 0x00, 
  0xdc, 0xef, 0x7b, 0x80, 0xdc, 0xef, 0x7b, 0x80, 0xdc, 0xef, 0x7b, 0x00, 0xdc, 0xe7, 0x72, 0x00, 
  0xe3, 0x00, 0x96, 0x00, 0x7f, 0xff, 0xfe, 0x00
};    //Chanrged battery indicator


const unsigned char batteryChargeIcon [] PROGMEM = {
  0x7f, 0xff, 0xf0, 0x00, 0xc0, 0x00, 0x0c, 0x00, 0xc0, 0x00, 0x0c, 0x00, 0xc0, 0x60, 0x0f, 0x80, 
  0xc3, 0xf0, 0x0d, 0x80, 0xc0, 0x3f, 0x0d, 0x80, 0xc0, 0x38, 0x0f, 0x80, 0xc0, 0x00, 0x0c, 0x00, 
  0xc0, 0x00, 0x0c, 0x00, 0x7f, 0xff, 0xf0, 0x00
};  //Battery charge Icon

void setup()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    Wire.begin();
    rtc.begin();
    display.clearDisplay();
    display.display();

    if(!rtc.begin())
    {
        display.setCursor(0, 0);
        display.print("RTC Error");
        display.display();
    }
    else if (!rtc.isrunning())
    {
        display.setCursor(0, 0);
        display.print("RTC is not working");
        display.display();
    }
    else
    {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Initializing");
        for (int i = 12; i < 17; i++)
        {
            delay(200);
            display.print(".");
            display.display();
        }
        delay(2000);
        display.clearDisplay();
        display.display();
    }
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    menu = 0;
}

void DisplayDateTime()
{
    DateTime now = rtc.now();
    display.setTextSize(2);
    display.setCursor(17, 15);

    if (now.hour() <= 9)
    {
        display.print("0");
    }
    display.print(now.hour(), DEC);
    hourupg = now.hour();
    display.print(":");

    if (now.minute() <= 9)
    {
        display.print("0");
    }
    display.print(now.minute(), DEC);
    minuteupg = now.minute();
    display.print(":");

    if (now.second() <= 9)
    {
        display.print("0");
    }
    display.print(now.second(), DEC);
    secondupg = now.second();
    
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print(DAY[now.dayOfTheWeek()]);
    
    display.setCursor(65, 0);

    if (now.day() <= 9)
    {
        display.print("0");
    }
    display.print(now.day(), DEC);
    dayupg = now.day();
    display.print("/");

    if (now.month() <= 9)
    {
        display.print("0");
    }
    display.print(now.month(), DEC);
    monthupg = now.month();
    display.print("/");

    display.print(now.year(), DEC);
    delayMicroseconds(5);
    display.clearDisplay();
}


void DisplaySetHour()
{
    display.clearDisplay();


    if (digitalRead(button2) == LOW)
    {
        if (hourupg == 23)
        {
            hourupg = 0;
        }
        else
        {
            hourupg = hourupg + 1;
        }
    }
    display.setCursor(0, 0);
    display.print("Set Hour: ");
    display.setCursor(40, 20);
    display.print(hourupg, DEC);
    display.display();
    delay(100);
}

void DisplaySetMinute()
{
    if (digitalRead(button2) == LOW)
    {
        if (minuteupg == 59)
        {
            minuteupg = 0;
        }
        else
        {
            minuteupg = minuteupg + 1;
        }
    }
    display.setCursor(0, 0);
    display.print("Set Minute: ");
    display.setCursor(40, 20);
    display.print(minuteupg, DEC);
    display.display();
    delay(100);
}

void DisplaySetDay()
{
    if (digitalRead(button2) == LOW)
    {
        if (dayupg == 31)
        {
            dayupg = 1;
        }
        else
        {
            dayupg = dayupg + 1;
        }
    }
    display.setCursor(0, 0);
    display.print("Set Day: ");
    display.setCursor(40, 20);
    display.print(dayupg, DEC);
    display.display();
    delay(100);
}

void DisplaySetMonth()
{
    if (digitalRead(button2) == LOW)
    {
        if (monthupg == 12)
        {
            monthupg = 1;
        }
        else
        {
            monthupg = monthupg + 1;
        }
    }
    display.setCursor(0, 0);
    display.print("Set Month: ");
    display.setCursor(40, 20);
    display.print(monthupg, DEC);
    display.display();
    delay(100);
}

void DisplaySetYear()
{
    if (digitalRead(button2) == LOW)
    {
        if (yearupg == 2099)
        {
            yearupg = 2000;
        }
        else
        {
            yearupg = yearupg + 1;
        }
    }
    display.setCursor(0, 0);
    display.print("Set Year: ");
    display.setCursor(40, 20);
    display.print(yearupg, DEC);
    display.display();
    delay(100);
}

void Storing()
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Saving in progress");
    rtc.adjust(DateTime(yearupg, monthupg, dayupg, hourupg, minuteupg, secondupg));
    display.display();
    delay(200);
}

void BatteryDisplay()
{
    int volts;
    batteryVoltage = analogRead(batteryRead);
    volts = (batteryVoltage/1023) * 100;

    if (volts == 100)
    {
        display.clearDisplay();
        display.drawBitmap(40, 10, chargedBattery, 25, 16, WHITE);
        display.setCursor(30, 20);
        display.print("Charged: ");
        display.print(volts);
        display.print("%");
        display.display();
        delay(100);
    }
    else if(volts < 100)
    {
        display.clearDisplay();
        display.drawBitmap(40, 10, chargedBattery, 25, 16, WHITE);
        display.setCursor(30, 20);
        display.print("Remaining: ");
        display.print(volts);
        display.print("%");
        display.display();
        delay(100);
    }
}

void BatteryCharging()
{
    int pvolts;
    powerVoltage = analogRead(powerRead);
    pvolts = (powerVoltage/1023) * 100;

    if (pvolts == 100)
    {
        display.clearDisplay();
        display.drawBitmap(40, 10, batteryChargeIcon, 25, 10, WHITE);
        display.setCursor(30, 20);
        display.print("Charged: ");
        display.print(pvolts);
        display.print("%");
        display.display();
        delay(100);
    }
    else if (pvolts < 100)
    {
        display.clearDisplay();
        display.drawBitmap(40, 10, batteryChargeIcon, 25, 10, WHITE);
        display.setCursor(30, 20);
        display.print("Charging: ");
        display.print(pvolts);
        display.print("%");
        display.display();
        delay(100);
    }
    else
    {
        display.clearDisplay();
        display.drawBitmap(40, 10, batteryChargeIcon, 25, 10, WHITE);
        display.setCursor(0, 20);
        display.print("Connect to charger/battery low");
        display.display();
        delay(100);
    }
}


void loop()
{
    if (digitalRead(button1) == LOW)
    {
        menu = menu + 1;
    }
    if (menu == 0)
    {
        DisplayDateTime();
    }
    else if (menu == 1)
    {
        DisplaySetMinute();
    }
    else if (menu == 2)
    {
        DisplaySetHour();
    }
    else if (menu == 3)
    {
        DisplaySetDay();
    }
    else if (menu == 4)
    {
        DisplaySetMonth();
    }
    else if (menu == 5)
    {
        DisplaySetYear();
    }
    else if (menu == 6)
    {
        if (batteryVoltage  >= 5.0)
        {
            BatteryDisplay();
        }
        else
        {
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("Battery is not");
            display.setCursor(0, 12);
            display.print("connected");
            display.display();
        }
    }
    else if (menu == 7)
    {
        if (powerVoltage >= 3.7)
        {
            BatteryCharging();
        }
        else
        {
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("Please connect");
            display.setCursor(0, 12);
            display.print("battery");
            display.display();
        }
    }
    else if (menu == 8)
    {
        Storing();
        delay(500);
        menu = 0;
    }
    delay(200);

    powerVoltage = analogRead(powerRead) * (5.0/1023.0);
    if (powerVoltage >= 4.69)
    {
        display.drawBitmap(0, 13, battery, 10, 16, WHITE);
    }
    batteryVoltage = analogRead(batteryRead) * (5.0/1023.0);
    if (batteryVoltage >= 0.00)
    {
        display.drawBitmap(0, 13, no_battery, 10, 16, WHITE);
    }
}