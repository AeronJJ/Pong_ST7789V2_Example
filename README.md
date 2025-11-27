### How to run
`git clone https://github.com/AeronJJ/Pong_ST7789V2_Example.git`
`git submodule init`
`git submodule update`

Connect ST7789V2 LCD to the following pins on Nucleo L467RG board:
| LCD | MCU |
|-----|-----|
| VCC | VDD |
| GND | GND |
| DIN | B15 |
| CLK | B13 |
| CS  | B12 |
| DC  | B11 |
| RST | B2  |
| BL  | B1  |


Open VSCode and install STM32Cube extension

Go to Terminal -> Run Task -> Build + Flash