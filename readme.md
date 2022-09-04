# Basic IoT Project - Control RGB LED from Webpage

Based on the example: https://aws.amazon.com/blogs/compute/building-an-aws-iot-core-device-using-aws-serverless-and-an-esp32/

## Prerequisites

Arduino, Access to AWS(All you need is a free account)

## Setting up the Arduino

NOTE: I reccommend using platformIO through VS code as it compiles faster than the Arduino IDE

Add main.ino + secrets.h

Add the additional required libraries including:

    - https://github.com/256dpi/arduino-mqtt search: mqtt in library manager
    (NOTE: unable to find this using arduino's built in library manager, platform io had no issue)
    - Arduino.json search: json in library manager

## AWS Setup

INCLUDE DIAGRAM OF ARCITECTURE HERE

### IoT Core

IoT core facilitates the connection between the arduino and AWS
Our main goal here is to be able to send and recieve MQTT messages

Create a thing
ADD IMAGE OF THIS

Add created certificates to secrets.h

Add

### S3

S3(Simple Storage Service) Allows you to store files and even host a bsaic website

Setup
Create a new bucket

### Cloudfront

### Lambda

Add trigger

## Extra

### Route 53

if you want to get fancy you can use Route 53 to point a domain at the cloudfront distribution
