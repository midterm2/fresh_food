# Sofeware
## Descript
   我們要做的智能型冰箱是打算用Microchip的dspic33F來作為整個系統的控制單元，而搭配的周邊和硬體設都可以在硬體架構單元內找尋到相關資料。而作為MCU的編譯程序打算用C 語言來開發，並在MPLAB IDE 下做編程，並且會連上[雲端私服器thingspeak]( https://thingspeak.com/apps)來做我們的資料儲      存，此私服器不但能幫使用者存取資料外，還可以為使用者建圖形表格，以方便使用者以圖形化的方式監控所存取的資料。
   最後作為人和機器的溝同，顯示介面為我們和機器的溝通橋樑，我們會打算用兩個顯示介面:     
   * 1.用MCU去控制HMI來顯示我們的選單，再用keypad的方式做輸入 
   * 2.則是用手機來顯示，我們寫一手機app來和機器做溝通。上述的概念就如圖(一)所示
   ![圖一](fresh_food/phtot1.png)
## Architecture diagram
