# ControlPanelLite

Control Panel Lite by MileStorm
made in 2019 - 2020

Contains four games:
 - `Red button:` Push the lit button (Easy version)
 - `Yellow button:` Push the lit button (Timed version) (not ready yet)
 - `Green button:` Simon says
 - `Blue button:` Sound board
    - Long press `Red button`: Retro sounds
    - Long press `Green button`: Wolf 3D sounds
    - Long press `Yellow button`: (not ready yet)
    - Long press `Blue button`: Two tones arp mellodies

### IDE Setup
If using CLion and unable to build, run `Tools > PlatformIO > Re-Init` this will generate `./cmake-build-nanoatmega328` folder and then you can upload from IDE with `^R`. 

### BOM
1x MAX7219 8x8 led matrix with driver [AliExpress Link](https://www.aliexpress.com/item/4000329917094.html)
4x 45mm Push Button Arcade Button [AliExpress Link](https://www.aliexpress.com/item/1005005621268503.html)
1x Arduino Pro Micro [AliExpress Link](https://www.aliexpress.com/item/32846843498.html)
1x Li-Pol Battery (Ideally salvage some from vape pens, they are 550 mAh)
1x Battery charging module with protection [AliExpress Link](https://www.aliexpress.com/item/1005005468881238.html)
1x Step Up converter 3.7V to 5V [AliExpress Link](https://www.aliexpress.com/item/1005002858417858.html)
1x Passive Buzzer or Speaker [AliExpress Link for Buzzer](https://www.aliexpress.com/item/1005003490603698.html)
1x Rocker Switch [AliExpress Link](https://www.aliexpress.com/item/1005004533833914.html)

### Pinout Definition
For pinout definition see `/include/config.h`
