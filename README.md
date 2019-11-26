# Filament scale project

## Building Software

```
$ mkdir build
$ cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/project-toolchain-config.cmake= ..
```

## Parts

- SSD1306 128x32px I2C OLED display [[datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)]
- HX711 breakout board [[datasheet](https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf)]
- 2x 5KG load cell [[link](https://www.robotshop.com/en/micro-load-cell-5-kg.html)]
- AtTiny85
- 3.3v voltage regulator with associated hardware
  - I bought a few LM2937ET-3.3 liniar regulators on DigiKey as they can take the 24v input from my 3D printer power supply.