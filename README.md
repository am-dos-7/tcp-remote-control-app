## tcp-remote-control-app
This repo holds files relative to a TCP remote control app: the rempte control app's code in QML/C++, and a C++ sample code for a ESP32-driven small-scale car.  
This project was originally started as an end-of-year project in engineering school, and later on progressively refined as a personal project. Improvements are still going on ;)  

It is intended to allow remote controlling any IP-enabled mobile vehicle through a TCP connection. It is contructed around a client-server idea where the vehicle starts a TCP server on a specific port where the client App can connect to, knowing its IP address and the port number.  
The remote application is entirely written under the Qt framework with a mix of QML for the front-end, and C++ for the back-end. 
