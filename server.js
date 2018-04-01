var SerialPort = require("serialport");
var Readline = SerialPort.parsers.Readline
var axios = require('axios');

var apiURL = "https://syde362fireplansystem.azurewebsites.net"

var sensorMapping = {
    "US1": {
        type: "Distance",
        cells: [788,789,790,791,792],
        value: function(value) {
            let threshold = 6;

            if (value > threshold) {
                return 1
            } else {
                return 0
            }
        }
    },"US2": {
        type: "Distance",
        cells: [400,440,480],
        value: function(value) {
            let threshold = 6;

            if (value > threshold) {
                return 1
            } else {
                return 0
            }
        }
    },"US3": {
        type: "Distance",
        cells: [22,23,24,25,26,27,28,29,30,31],
        value: function(value) {
            let threshold = 5;

            if (threshold < value) {
                return 1
            } else {
                return 0
            }
        }
    },"US4": {
        type: "Distance",
        cells: [769,770,771,772,773],
        value: function(value) {
            let threshold = 6;

            if (threshold < value) {
                return 1
            } else {
                return 0
            }
        }
    },"US5": {
        type: "Distance",
        cells: [681, 641, 601],
        value: function(value) {
            let threshold = 7;

            if (threshold < value) {
                return 1
            } else {
                return 0
            }
        }
    },"Temp1": {
        type: "Temperature",
        cells: [440,480,520,560,600,479,519,559,518],
        value: function(value) {
            let threshold = 40;

            if (threshold < value) {
                return 1
            } else {
                return 0
            }
        }
    },"Temp2": {
        type: "Temperature",
        cells: [26,27,28,29,30,67,68,69,70],
        value: function(value) {
            let threshold = 40;

            if (threshold < value) {
                return 1
            } else {
                return 0
            }
        }
    },"Temp3": {
        type: "Temperature",
        cells: [778,779,780,781,782,739,740,741,742],
        value: function(value) {
            let threshold = 40;

            if (threshold < value) {
                return 1
            } else {
                return 0
            }
        }
    },"Temp4": {
        type: "Temperature",
        cells: [521,561,601,641,681,562,602,642,603],
        value: function(value) {
            let threshold = 40;

            if (threshold < value) {
                return 1
            } else {
                return 0
            }
        }
    },"GAS4": {
        type: "Gas",
        cells: [384,385,386,387,388,389,390, 424,425,426,427,428,429,430, 464,465,466,467,468,469,470],
        value: function(value) {
            let threshold = 70;

            if (threshold > value) {
                return 1
            } else {
                return 0
            }
        }
    },"GAS2": {
        type: "Gas",
        cells: [373,374,375,376,377,378,379,413,414,415,416,417,418,419,453,454,455,456,457,458,459],
        value: function(value) {
            let threshold = 50;

            if (threshold > value) {
                return 1
            } else {
                return 0
            }
        }
    },"GAS3": {
        type: "Gas",
        cells: [780,781,782,740,741,742],
        value: function(value) {
            let threshold = 300;

            if (threshold < value) {
                return 1
            } else {
                return 0
            }
        }
    },"GAS1": {
        type: "Gas",
        cells: [ 1,
            2,
            3,
            4,
            5,
            6,
            7,
            8,
            9,
            10,
            41,
            42,
            43,
            44,
            45,
            46,
            47,
            48,
            49,
            50,
            81,
            82,
            83,
            84,
            85,
            86,
            87,
            88,
            89,
            90,
            121,
            122,
            123,
            124,
            125,
            126,
            127,
            128,
            129,
            130,
            161,
            162,
            163,
            164,
            165,
            166,
            167,
            168,
            169,
            170,
            201,
            202,
            203,
            204,
            205,
            206,
            207,
            208,
            209,
            210,
            241,
            242,
            243,
            244,
            245,
            246,
            247,
            248,
            249,
            250,
            281,
            282,
            283,
            284,
            285,
            286,
            287,
            288,
            289,
            290 ],
            value: function(value) {
                let threshold = 140;
    
                if (threshold > value) {
                    return 1
                } else {
                    return 0
                }
            }
    }
}

var serialPort1 = new SerialPort("/dev/cu.usbmodemFD121");
var serialPort2 = new SerialPort("/dev/cu.usbmodemFA131");

var parser1 = new Readline()
serialPort1.pipe(parser1)

var parser2 = new Readline()
serialPort2.pipe(parser2)

function sendStuff(data) {
    var outputArray = []
    let elements = data.replace(/\r/g, '').split(",")
    for (var index in elements) {
        let element = elements[index];
        if (!element) {
            continue
        }

        let parsedElement = element.split(" ");
        let sensorID = parsedElement[0];
        let value;
        // console.log(parsedElement)
        if (parsedElement.length == 2) {
            value = parsedElement[1];
        } else {
            value = [parsedElement[1], parsedElement[2]];
        }

        let sensorMappingValue = sensorMapping[sensorID]

        if (!sensorMappingValue) {
            continue
        }

        let json = {
            type: sensorMappingValue.type,
            locations: sensorMappingValue.cells,
            value: value
        }

        outputArray.push(json);
    }

    let body = {
        updateData: outputArray
    }

    // console.log(apiURL + '/graphInformation/updateGraph/0')

    axios({
        method: 'post',
        url: apiURL + '/graphInformation/updateGraph/0',
        data: body
      }).then((success) => {
        //   console.log("This actually worked!")
      }).catch((error) => {
        //   console.log("This didn't work :( " + error);
      }) 
}

serialPort1.on('open', function(){
  console.log('Serial Port Opened');
  parser1.on('data', function (data) {
    console.log(data);
        if (data != 'Ready') {
            sendStuff(data)
        }
  });
});

serialPort2.on('open', function(){
    console.log('Serial Port Opened');
    parser2.on('data', function (data) {
      console.log(data);
      if (data != 'Ready') {
        sendStuff(data)
      }
      
    });
  });