# fresh_food
Prevent expired
軟體架構:
1.我們是打算用手機或HMI做顯示介面，做多個介面，一個介面唯一參數，如此一來就可以判斷，食物是冰冷凍或只是冷藏、是肉類或是奶製品誘惑只是蔬菜類等等等，
  軟體會在MPLAB IDE下做編成，並用DSPIC33作為我們的控制器，再搭配微控制器的周邊來連動HMI WI-FI keypad RTC 。
2.在運網路上的資源作為雲端印碟來儲存我的資料，寫出手機APP來做第二個介面。
<硬體架構>
MCU、Battery、keypad、uart_hmi、app、smart phone、real time clock、refrigerator