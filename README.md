# Aquae
Sanbox for Raoul Gilioli's Aquae artwork


**This repository is meant to updload Arduino sketches and other draft codes for sensors and technology to be tested for Aquae artwokrs**

## How to use this repository:
* clone the repository to your local machine with `git clone https://github.com/Omnivors/aquae.git`
* please add all of your code in separated folders (better if [kebab-case](https://it.wikipedia.org/wiki/Kebab_case)) _e.g. x-band-tests, ir-codes or mirror-draft..._
* stage `git add *`, commit `git commit` and push to master branch `git push`

# Guide e appunti

## Music Maker
* [Documentazione ufficiale](https://learn.adafruit.com/adafruit-music-maker-shield-vs1053-mp3-wav-wave-ogg-vorbis-player)
* necessaria un'accurata gestione degli interrupt per consentire la lettura dei sensori durante la riproduzione del file
* per i sensori che restituiscono 0/1 è necessario gesitre il caso in cui il movimento non finisce per un periodo lungo. I sensori rilevano sempre 1 e quindi bisogna implementare un sistema che controlla se lo stato rimane HIGH per molto tempo di fila (potrebbe essere
sensato impostare i samples = ~ la durata dello sleep time del sensore, e controllare ogni
volta che finisce il sample, oppure scegliere un sample molto lungo e lasciarlo ripdodurre
ogni volta interamente, stoppandolo solamente quando finsice il movimento)
per il sensore X-BAND si potrebbe sfruttare la frequenza come dato per capire la
situazione "movimento" e decidere come gestire la traccia. non sono stati fatti test

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
* Libreria [IRemote](https://github.com/z3t0/Arduino-IRremote)


## PIR
* [Datasheet](https://www.parallax.com/sites/default/files/downloads/555-28027-PIR-Sensor-Product-Guide-v2.3.pdf) dal sito Parallax
* restituisce un segnale HIGH / LOW (*movimento* / *non movimento*)
* robusto il rilevamento
* necessita di ~40 secondi di calibrazione all'avvio per funzionare bene. Durante la calibrazione non ci devono essere movimenti
* **Open issues**:
    * rileviamo il movimento, come costruiamo un'esperienza piacevole basata sul movimento?
    * c'è un po' di ritardo (~5/6 secondi) da quando il movimento finisce a quando lo stato si aggiorna a LOW
    * idea: far partire samples che si sovrappongono ogni volta che rileviamo un movimento


## X-Band Motion Detector
* [Datasheet](https://www.parallax.com/sites/default/files/downloads/32213-X-BandMotionDetector-v1.1_0.pdf) dal sito Parallax
* _operational frequency = 10.525 GHz_
* output del sensore al pin OUT è un impulso la cui frequenza è proporzionale alla velocità degli oggetti davanti all'antenna
* la frequenza può essere stimata su Arduino tramite la liberia [FreqMeasure](https://www.arduinolibraries.info/libraries/freq-measure)
(qui un esempio di [sketch](https://gist.github.com/BenjaminFraser/0c36b14bd85bf9ff45055a016a608856#file-doppler_frequency_sensing-cpp))
* montaggio incredibilmente semplice
* ottimo rilevamento nella direzione di massima potenza, molto sensibile se non ci sono
ostacoli od attraverso porte, muri e specchi
* **Open issues**:
    * soffre attraverso il vetro, ma i vetri di casa sono doppi e belli spessi. Andrebbe testato per le singole opere
    * qualsiasi movimento viene rilevato allo stesso modo, quindi per usarlo dietro i vetri che girano bisogna gestire dei tempi di _timeout_ per non entrare in loop infiniti
    * come possiamo sfruttare il dato sulla frequenza (che è proporzionale alla velocità del movimento rilevato) per gestire la traccia audio?



## RCWL 0516
* [Documento](https://github.com/jdesbonnet/RCWL-0516/) ben strutturato con test e dettagli
* _operational frequency = 3.181 GHz_
* restituisce HIGH / LOW
* robusto il rilevamento
* funziona bene attraverso il legno (~3cm)
* tempo di inattività 2/3 sec
