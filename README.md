# TCP remote control App
This repo holds files relative to a TCP remote control app: the rempte control app's code in QML/C++, and a C++ sample code for a ESP32-driven small-scale car.  
This project was originally started as an end-of-year project in engineering school, and later on progressively refined as a personal project. Improvements are still going on ;)  
It is intended to allow remote controlling any IP-enabled mobile vehicle through a TCP connection, with image frames streaming support if the vehicle is equipped with a camera. It is contructed around a client-server idea where the vehicle starts a TCP server on a specific port where the client App can connect to, knowing its IP address and the port number.  
The remote application is entirely written under the Qt framework with a mix of QML for the front-end, and C++ for the back-end. 

## The Remote control app
It is a simple 2 "interfaces" application, a connection-setup interface and a stream-and-control interface
### Connection-setup
The connection is done by simply specifying the remote server's IP address and port number. This interface can be accessed from the stream-and-control one using the button in the top left corner. It allows connecting to but also disconnecting from a server.
### Stream-and-control
The image frames transmitted by the vehicle's camera, if there is one, are displaying one after another resulting in a video stream. This interface is provided with four buttons to control the vehicle motion: Up, down, left and right buttons which can serve for a car as well as a UAV. The interface also has a connection status indicator, which is red when the connection is off, and green when the connection is established. It automatically turns red when the connection breaks, or if voluntarily disconnected from the client. This enables an easy monitoring of the connection status while driving.  

The pression on the buttons triggering the sending of fixed bytes of messages to the server, this application can be employed in a huge number of projects, as long the reception of such bytes are well-handled on the server part. And the most interesting part is, the Qt framework being cross-platform, the application has been successfully deployed on Android with little configuration steps: that was the purpose of choosing Qt ;)

## The server side code
Provided in this repo, is the code written in C++ under Platformio for an-ESP32 driven 4-wheeled robot. It only handles the communication with the server, as all the mechanisms related to motion control were taken care of by an Arduino Nano. All the bytes relative to motion control received by the ESP32 are forwarded to the Arduino Nano through an I2C connection. (Arduino code not included in the repo).  

In the actual state of the project, it quickly occurs that TCP is not really suitable for real-time video streaming purpose, as it requires great bandwidth and other resources. So, next time, implement the streaming component using UDP while maintaining the control side in TCP.  

Project demo link: https://youtu.be/ipp_EQQPbHI
