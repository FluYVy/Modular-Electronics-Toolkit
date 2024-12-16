//Open windows
clear();

var test_frequencies = [80000, 100000, 300000, 400000];
var tests = [];

//Test frequency
test_frequencies.forEach(function(frequency) {
    Logic.single();
    while(Logic.State.running());
    const measured_frequency = Logic.Channels.I2C.measure("Frequency");
    const passed = (frequency*0.9 < measured_frequency && measured_frequency < frequency * 1.1);
    tests.push(passed);
    print("Passed: " + passed);
    print("Measured Frequency: " + measured_frequency + "\n");
});

//Test write
const returnValues = [0xf0, 0x0f];
Protocol.I2C.SlaveConfig(-1, 2, returnValues, true);
Protocol.I2C.Receiver();

var returning = 0;
var stopWhile = false;
while(!stopWhile){
    var rg = Protocol.I2C.Receive();
    if(!rg.length) continue;
    rg.forEach(function (v){
        if(v>>1 == 200)
        {
            returning = 1;
        }else if (returning>0)
        {
            tests.push(v>>1 == returnValues[returning - 1]);
            returning = returning + 1;
            if (returning == 3)
            {
                stopWhile = true;
            }
        }        
    });
}

print("Summary");
print("-----------------------");

for (var i = 0; i < test_frequencies.length; i++){
    const answer = tests[i] ? "Passed" : "Failed";
    print("Frequency " + test_frequencies[i] + ": " + answer);
}

const passed = tests[tests.length-2] && tests[tests.length-3];
const answer = passed ? "Passed" : "Failed";
print("Read/Write: " + answer);