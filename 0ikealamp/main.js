/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

/*
A simple node.js application intended to read data from Digital pins on the Intel based development boards such as the Intel(R) Galileo and Edison with Arduino breakout board.

MRAA - Low Level Skeleton Library for Communication on GNU/Linux platforms
Library in C/C++ to interface with Galileo & other Intel platforms, in a structured and sane API with port nanmes/numbering that match boards & with bindings to javascript & python.

Steps for installing MRAA & UPM Library on Intel IoT Platform with IoTDevKit Linux* image
Using a ssh client: 
1. echo "src maa-upm http://iotdk.intel.com/repos/1.1/intelgalactic" > /etc/opkg/intel-iotdk.conf
2. opkg update
3. opkg upgrade

Article: https://software.intel.com/en-us/html5/articles/intel-xdk-iot-edition-nodejs-templates
*/

var mraa = require('mraa'); //require mraa
console.log('MRAA Version: ' + mraa.getVersion()); //write the mraa version to the console

var myDigitalPinR = new mraa.Gpio(6); //setup digital read on Digital pin #6 (D6)
var myDigitalPinW = new mraa.Gpio(13); //setup digital read on Digital pin #5 (D5)
myDigitalPinR.dir(mraa.DIR_IN); //set the gpio direction to input
myDigitalPinW.dir(mraa.DIR_OUT); //set the gpio direction to output

periodicActivity(); //call the periodicActivity function

var on = 0;
var flag = 0;

function periodicActivity() //
{
  var click = 1 - myDigitalPinR.read(); //read the digital value of the pin
    console.log('Gpio is ' + click);
    if (!click) 
        {
            flag = 0;
        }
    if (!flag)
    {
      if (click)
          {
              flag = 1;
              console.log('Gpio is ' + click);
              if (on)
                  {
                  on = 0;
                  myDigitalPinW.write(0);
                  }
              else
                  {
                  on = 1;
                  myDigitalPinW.write(1);
                  }
          }
    
  //console.log('Gpio is ' + Value); //write the read value out to the console
 // myDigitalPinW.write(Value); //set the digital pin to high (1)
  
    }
setTimeout(periodicActivity,100); //call the indicated function after 1 second (1000 milliseconds)
}


