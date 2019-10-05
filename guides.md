# Guide e appunti

## Music Maker

* [Documentazione ufficiale](https://learn.adafruit.com/adafruit-music-maker-shield-vs1053-mp3-wav-wave-ogg-vorbis-player)



## Infrared
#### Receiver
* [Doc ufficiale](https://www.adafruit.com/product/157)
* [Datasheet](https://cdn-shop.adafruit.com/datasheets/tsop382.pdf)
* [Tutorial](https://www.hackster.io/techmirtz/finding-the-ir-codes-of-any-ir-remote-using-arduino-c7a852)

#### Transmitter
* [Osram SFH 4841 Datasheet](https://pdf1.alldatasheet.com/datasheet-pdf/view/1015969/OSRAM/SFH4841.html) (anodo + dove c'è la linguetta)
* [Transistor 2n5088 ed equivalenti](https://components101.com/transistors/2n5088-pinout-equivalent-datasheet)
* Tutorial
    * [Adafruit](https://learn.adafruit.com/using-an-infrared-library/sending-ir-codes)
    * [Arduino Project Hub](https://create.arduino.cc/projecthub/tatco/how-to-hack-ir-remotes-2d8b18)


## PIR
* [Parallax PIR datasheet](https://www.parallax.com/sites/default/files/downloads/555-28027-PIR-Sensor-Product-Guide-v2.3.pdf)
* Open issues:
    * rileviamo il movimento, come costruiamo un'esperienza piacevole basata sul movimento?
    * c'è un po' di ritardo (~5/6 secondi) da quando il movimento finisce a quando lo stato si aggiorna
    * idea: far partire samples che si sovrappongono ogni volta che rileviamo un movimento
