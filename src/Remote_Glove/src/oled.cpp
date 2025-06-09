#include "oled.h"
#include "esp_system.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* Initialize oled */
void oledSetup() 
{
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
        delay(1000);
        esp_restart();
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Starting...");
    display.display();
}

/* Display gyro values with transmission status and blinking */
void oledShowInclination(int rollPct, int pitchPct, TxStatus txStatus, bool blinkOn) 
{
    /* Display inclination agles */
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Inclination:");
    display.print("Roll : "); display.print(rollPct); display.println(" %");
    display.print("Pitch: "); display.print(pitchPct); display.println(" %");
    display.setCursor(0, 40);  

    /* Display communication status */
    if (txStatus == TX_OK) 
    {
        if (blinkOn) 
        {
            display.println("Com status: OK");  
        } 
        else 
        {
            display.println("       ");  
        }
    } 
    else if (txStatus == TX_FAIL) 
    {
        if (blinkOn) 
        {
            display.println("Com status: FAIL");
        } 
        else 
        {
            display.println("       ");
        }
    }
    else 
    {
        display.println("Com status: UNKNOWN");
    }

    display.display();
}

