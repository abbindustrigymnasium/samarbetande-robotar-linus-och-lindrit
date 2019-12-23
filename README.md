# Samarbetande robotar Linus Kasper och Lindrit Koxha

I det här projektet har Linus Kasper och Lindrit Koxha försökt att få två robotar att åka med samma hastighet. För att lösa den uppgiften så använde vi oss av ett reglersystem. Programmvaran är skriven i språket **C++** och **Java Script**. Där vi har har ett skript till reglerloopen och ett till hemsidan där vi viusaliserar hastigheterna i realtid. För att skicka data i realtid använde vi oss av **MQTT** där vi hostade en **broker** på **maqiatto.com**.

## Bilder på robotarna
* Lindrits Robot
    * lindritsRobot1.jpg - Lindrits robot framifrån
    * lindritsRobot2.jpg - Lindrits robot bakifrån
    * lindritsRobot3.jpg - Lindrits robot från sidan

* Linus Robot
    * linusRobot1.jpg - Linus robot framifrån
    * linusRobot2.jpg - Linus robot bakifrån
    * linusRobot3.jpg - Linus robot från sidan

## Dokumentation 
* Dokumentation.pdf - dokumentationen för robotarna

## [Hemsida](http://robotbilsida.s3-website-us-east-1.amazonaws.com)
* Bilder på hemsidan
    * 1.png - startsidan
    * 2.png - när man klickar connect
    * 3.png - när man är connectad till brokern
    * 4.png - när man skickar ett börvärde
    * 5.png - när bilarnas data loggas och visualiserats
    * 6.png - när man klickar disconnect
* index.html - hemsidan
* style.css - stylen till hemsidan

## Koppling och flödesschema
* Flödesschema.vsdx - Flödesschemat för robotarna
* RobotKoppling.ms14 - Kopplingsschemat för robotarna

## Reglerkoder
* reglerKodLindrit
    * reglerKodLindrit.ino - reglerloopen till robot 1

* reglerKodLinus
    * reglerKodLinus.ino - reglerloopen till robot 2