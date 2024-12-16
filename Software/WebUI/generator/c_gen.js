export const cGenerator = new Blockly.Generator('C');

const Order = {
    ATOMIC: 0,
}

const definitions = {
    fcpu:       0,
};

const includes = {
    i2c_master: 0,
    spi_master: 1,
    scd41:      2,
    sevseg:     3,
    delay:      4,
};

const inits = {
    i2c_master: 0,
    spi_master: 1,
    sevseg:     2,
};

export const includesText = [
    "#include \"i2c_master.h\"\n",
    "#include \"spi_master.h\"\n",
    "#include \"SCD41.h\"\n",
    "#include \"sevseg.h\"\n",
    "#include <util/delay.h>\n",
];

export const initText = [
    "//init i2c\ni2c_master_init(200000);\n\n",
    "//init spi\nspi_master_init(8);\n\n",
    "//init 7-Segment-Display\nsevseg_init();\n\n",
]

export const defineText = [
    "#define F_CPU 12000000UL",
]

export var useIncludes = Array(Object.keys(includes).length);
useIncludes.fill(false);

export var useInits = Array(Object.keys(inits).length);
useInits.fill(false);

export var useDefines = Array(Object.keys(definitions).length);
useDefines.fill(false);


cGenerator.forBlock['events_start'] = function(block){
    return '// start\n'
};

cGenerator.forBlock['events_loop'] = function(block){
    return '// loop\n'
};

//scd41

cGenerator.forBlock['scd41_start_measure_single'] = function(block){
    useIncludes[includes.scd41] = true;
    useInits[inits.i2c_master] = true;

    return 'measure_single_shot(0x01);'
};

cGenerator.forBlock['scd41_retrieve_values'] = function(block){
    useIncludes[includes.scd41] = true;
    useInits[inits.i2c_master] = true;

    return 'read_measurement();'
};

cGenerator.forBlock['scd41_get_value'] = function(block){
    useIncludes[includes.scd41] = true;
    useInits[inits.i2c_master] = true;

    var code;
    switch (block.getFieldValue('value'))
    {
        case "opt_co2":
            code = 'scd41_get_co2()'
        break;
        case "opt_temp":
            code = 'scd41_get_temperature()'
        break;
        case "opt_rh":
            code = 'scd41_get_humidity()'
        break;
    }
    return [code, Order.ATOMIC];
};

cGenerator.forBlock['scd41_get_value_co2'] = function(block){
    useIncludes[includes.scd41] = true;
    useInits[inits.i2c_master] = true;
    return ['scd41_get_co2()', Order.ATOMIC];
};

cGenerator.forBlock['scd41_get_value_temp'] = function(block){
    useIncludes[includes.scd41] = true;
    useInits[inits.i2c_master] = true;
    return ['scd41_get_temperature()', Order.ATOMIC];
};

cGenerator.forBlock['scd41_get_value_rh'] = function(block){
    useIncludes[includes.scd41] = true;
    useInits[inits.i2c_master] = true;
    return ['scd41_get_humidity()', Order.ATOMIC];
};

//sevseg
cGenerator.forBlock['sevseg_print_number'] = function(block, generator){
    useIncludes[includes.sevseg] = true;
    useInits[inits.i2c_master] = true;
    useInits[inits.sevseg] = true;

    const number = generator.valueToCode(block, 'number', Order.ATOMIC) || "0";
    const code = `sevseg_print_number(${number});`;
    return code;
};

cGenerator.forBlock['sevseg_print_float'] = function(block, generator){
    useIncludes[includes.sevseg] = true;
    useInits[inits.i2c_master] = true;
    useInits[inits.sevseg] = true;

    const number = generator.valueToCode(block, 'number', Order.ATOMIC) || "0";
    const code = `sevseg_print_float(${number}, 2);`;
    return code;
};

//utils
cGenerator.forBlock['utils_delay'] = function(block, generator){
    useIncludes[includes.delay] = true;
    useDefines[definitions.fcpu] = true;

    const num_time = block.getFieldValue('num_time');
    
    switch (block.getFieldValue('set_time'))
    {
        case "opt_us":
            return `_delay_us(${num_time});`
        break;
        case "opt_ms":
            return `_delay_ms(${num_time});`
        break;
        case "opt_s":
            return `_delay_ms(${num_time * 1000});`
        break;
    }

    return '';
};
cGenerator.forBlock['input_string'] = function(block, generator){
    return[ "\"" + block.getFieldValue('Test') + "\"", Order.NONE];

}

