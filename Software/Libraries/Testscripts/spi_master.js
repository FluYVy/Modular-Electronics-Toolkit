clear();

const F_CPU = 12000000;
var test_PS = [2, 4, 8, 16, 32, 64, 128];
var tests = [];

//Test frequencies
test_PS.forEach(function(PS) {
    const frequency = F_CPU/PS;
    Logic.single();
    while(Logic.State.running());
    const measured_frequency = Logic.Channels.SPIMOSI1.measure("Frequency");
    const passed = (frequency*0.9 < measured_frequency && measured_frequency < frequency * 1.1);
    tests.push(passed);
    print("Passed: " + passed);
    print("Measured Frequency: " + measured_frequency + "\n");
});


//Test transfer
//Not implemented Yet (I do not know how to respond)
print("Summary");
print("-----------------------");

for (var i = 0; i < test_PS.length; i++){
    const answer = tests[i] ? "Passed" : "Failed";
    print("Prescaler " + test_PS[i] + ": " + answer);
}
