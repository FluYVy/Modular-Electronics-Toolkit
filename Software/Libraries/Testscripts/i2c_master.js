//Open windows
clear();

print("I2C-Master-Test");
print("------------------------------------");

var test_frequencies = [80000, 100000, 300000, 400000];
var tests = [];

// Iterate through test frequencies.
test_frequencies.forEach(function(frequency) {

    // Start single-shot measurement.
    Logic.single();

    // Wait for measurement completion.
    while(Logic.State.running());

    // Measure frequency on I2C channel.
    const measured_frequency = Logic.Channels.I2C.measure("Frequency");

    // Check if measured frequency is within 10% of target.
    const passed = (frequency*0.9 < measured_frequency && measured_frequency < frequency * 1.1);

    // Store test result.
    tests.push(passed);

    // Print test result.
    print("Passed: " + passed);

    // Print measured frequency.
    print("Measured Frequency: " + measured_frequency + "\n");
});

// Test write
const returnValues = [0xf0, 0x0f]; // Values to be returned by the I2C slave
Protocol.I2C.SlaveConfig(-1, 2, returnValues, true); // Configure I2C slave
Protocol.I2C.Receiver(); // Start I2C receiver

var returning = 0; // Tracks the current return value being checked
var stopWhile = false; // Flag to stop the loop

while(!stopWhile){
    var rg = Protocol.I2C.Receive(); // Receive data from I2C
    if(!rg.length) continue; // Skip if no data received

    rg.forEach(function (v){
        if(v>>1 == 200) // Check for a specific start condition (0xC8 shifted right by 1)
        {
            returning = 1; // Start checking return values
        }else if (returning>0) // If start condition received
        {
            tests.push(v>>1 == returnValues[returning - 1]); // Compare received data with expected return value
            returning = returning + 1; // Increment return value counter
            if (returning == 3) // All return values checked
            {
                stopWhile = true; // Stop the loop
            }
        }        
    });
}

print("Summary");
print("------------------------------------");

for (var i = 0; i < test_frequencies.length; i++){
    const answer = tests[i] ? "Passed" : "Failed";
    print("Frequency " + test_frequencies[i] + ": " + answer);
}

const passed = tests[tests.length-2] && tests[tests.length-3];
const answer = passed ? "Passed" : "Failed";
print("Read/Write: " + answer);