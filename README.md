# ESP32にST7789ディスプレイをつなぐ
aliで買った"<b>ESP32-S3 supermini</b>"にSPIディスプレイST7789をつなぐテスト<br>
<br>

# esp32-s3-fh4r2について
ボート名はPlatformIOには登録されていない。<br>
<br>
https://community.platformio.org/t/esp32-s3-zero-does-not-work-on-platformio/40297/10 <br>
上記の情報で自前で作成登録した。詳細はリンク先参照。<br>

# ディスプレイ PIN

* GND          GND
* VCC          3.3V
* SCL (SCLK) → GPIO13
* SDA (MOSI）→ GPIO12
* RES (RST)  → GPIO11
* DC        → GPIO10
* CS        → GPIO9
* BLK       → GPIO8

ピンは3.3vピンを基準に適当に配置。結構好きに変更可能。<br>
lovyan03/LovyanGFX@^1.2.7 を使用

BLをGPI08に繋いでいるが、LovyanGFXだとBLの指定ができなかったので、classのコンストラクタに

```
pinMode(8, OUTPUT);
digitalWrite(8, HIGH);

```
を入れて無理やり使ってる。これが嫌なら3.3Vピンに指せばいい。ただし、バックライトの制御はできなくなる。

## Dependency
Visual studio 2022 C#

## License
This software is released under the MIT License, see LICENSE

## Authors

bry-ful(Hiroshi Furuhashi)<br>
twitter:[bryful](https://twitter.com/bryful)<br>
Mail: bryful@gmail.com<br>

