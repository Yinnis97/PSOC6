<h1 align="center">PSOC6-cy8cproto-062-4343w</h1>
<h3 align="center">Trying Stuff Out</h3>

**Materiaal**
- KY-012 - Buzzer
- 5VDC Motor
- L298N Motor driver module
- CY8CPROTO-062-4343W
- TTP224 - Capacitief keypad 1x4 met 4 vlakken 
- KY-013 - Analoge Temperatuur Sensor
- Push Button
- UCNL 4010 - Proximity Sensor


**Introductie**

*De bedoeling is om een kleine DC-motor te kunnen laten werken met de PSOC6 en gebruik te maken van meerdere componenten.
De motor kan zowel links als rechts draaien door middel van 2 knoppen en de snelheid kan worden aangepast
door ook weer 2 knoppen. 
Ook is er gebruik gemaakt van een Proximity sensor die er voor zorgt dat de motor stilvalt indien men te kort bij komt.
Dan word de temperatuur gemeten door middel van een temperatuur sensor die kan worden uitgezet door middel van een knop.
Wanneer de start/stop knop wordt ingeduwd zal de motor starten indien hij al bezig is stopt hij en gaat de PSOC in sleep mode.
Ten slotte zal de buzzer een geluidssignaal produceren wanneer er op eender welke knop wordt gedrukt als er niet op de startknop is gedrukt.*


**Gebruikte technologieën**
- I2C
- Interrupts
- ADC
- PWM

  
**Belangrijk**

***Het is van belang om de juiste poorten op het PSOC6 bordje te gebruiken.
Ook zal je in de Device Configurator poort P10_5 moeten veranderen zodat deze vrij is om te gebruiken.***

