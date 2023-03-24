# tcp-remote-control-app
This repo holds files relative to a TCP remote control app: the rempte control app's code in QML/C++, and a C++ sample code for a ESP32-driven small-scale car.  
This project was originally started as an end-of-year project in engineering school, and later on progressively refined as a personal project. Improvements are still going on ;)  
It is intended to allow remote controlling any IP-enabled mobile vehicle through a TCP connection, with image frames streaming support if the vehicle is equipped with a camera. It is contructed around a client-server idea where the vehicle starts a TCP server on a specific port where the client App can connect to, knowing its IP address and the port number.  
The remote application is entirely written under the Qt framework with a mix of QML for the front-end, and C++ for the back-end. 

## The Remote control app
It is a simple 2 "interfaces" application, a connection-setup interface and a stream-and-control interface
### Connection-setup
The connection is done by simply specifying the remote server's IP address and port number. This interface can be accessed from the stream-and-control one using the button in the top left corner. It allows connecting to but also disconnecting from a server.
### Stream-and-control
The image frames transmitted by the vehicle's camera, if there is one, are displaying one after another resulting in a video stream. This interface is provided with four buttons to control the vehicle motion: Up, down, left and right buttons which can serve for a car as well as a UAV. The interface also has a connection status indicator, which is red when the connection is off, and green when the connection is established. It automatically turns red when the connection breaks, or if voluntarily disconnected from the client. This allows an easy monitoring of the connection status.
