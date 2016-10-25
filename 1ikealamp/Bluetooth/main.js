/*jslint node:true,vars:true,bitwise:true,unparam:true */

/*jshint unused:true */

/*
The BLE - Peripheral Node.js sample application distributed within Intel® XDK IoT Edition under the IoT with Node.js Projects project creation option showcases how to advertise it's presence, read and write data via it's service and corresponding characteristic for Bluetooth Low Energy (BLE) communication.

MRAA - Low Level Skeleton Library for Communication on GNU/Linux platforms
Library in C/C++ to interface with Galileo & other Intel platforms, in a structured and sane API with port nanmes/numbering that match boards & with bindings to javascript & python.

Steps for installing/updating MRAA & UPM Library on Intel IoT Platforms with IoTDevKit Linux* image
Using a ssh client: 
1. echo "src maa-upm http://iotdk.intel.com/repos/1.1/intelgalactic" > /etc/opkg/intel-iotdk.conf
2. opkg update
3. opkg upgrade

OR
In Intel XDK IoT Edition under the Develop Tab (for Internet of Things Embedded Application)
Develop Tab
1. Connect to board via the IoT Device Drop down (Add Manual Connection or pick device in list)
2. Press the "Settings" button
3. Click the "Update libraries on board" option

Review README.md file for more information about enabling bluetooth and completing the desired configurations.

Article: https://software.intel.com/en-us/creating-a-bluetooth-low-energy-app/ 
*/

var led_characteristics = null;
var connected = 0;
var high = 'h';
var low = 'l';
var auto = 'a';

function send_to_genuino(state) {
    if (!led_characteristics)
        console.log('ERROR: Invoking send with no characteristic discovered.');
    else
    	if (connected)
        	led_characteristics.write(new Buffer(state), withoutResponse = false);
        else
        	console.log('Not connected, can\'t proceed');
}


function setup_bt() {
    var noble = require('noble');

    console.log('Starting application.');

    var peripheralUUID = '984fee0fbf77';
    var characteristicsUUIDs = ['19b10111e8f2537e4f6cd104768a1214'];
    var serviceUUIDs = ['19b10110e8f2537e4f6cd104768a1214']; // default: [] => all
    var allowDuplicates = false; // default: false


    
    noble.on('stateChange', function(state) {
            console.log('State changed to: ' + state);

            if (state === 'poweredOn') {
                console.log('Scanning for devices...');
                noble.startScanning(); // particular UUID's
            }   
            else {
                noble.stopScanning();
                process.exit();
            }
    });

    noble.on('discover', function(peripheral) {
            console.log('Discovered device, id = ' + peripheral.id.toString());
            explore(peripheral);
    });


    function explore(peripheral) {
        if (peripheral.id == peripheralUUID) {
            console.log('ID matches Genuino ID.');
            noble.stopScanning();

            peripheral.on('disconnect', function() {
                    console.log('Disconnected. Attemping to reconnect in 200 ms...');
                    connected = 0;
                    setTimeout(function() {peripheral.connect()}, 200);
            });

            
            peripheral.on('connect', function() {connected = 1});

            getcharacteristics(peripheral);
        }
    }



    function getcharacteristics(peripheral) {
            peripheral.connect(function(error) {
                    console.log('-----------------------------------------------------------------------');
                    console.log('Connection established!');

                    peripheral.discoverServices(serviceUUIDs, function(error, services) {
                            services[0].discoverCharacteristics(characteristicsUUIDs, function(error, characteristics) {
                                    console.log('Found LED characteristic.');
                                    led_characteristics = characteristics[0];
                            });
                    });
            });
    }

}

exports.setup_bt = setup_bt;
exports.send = send_to_genuino;
exports.low = low;
exports.high = high;
exports.auto = auto;
