  Blockly.Blocks['input_string'] = {
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('String')
      .appendField(new Blockly.FieldTextInput('Test'), 'value');
    this.setOutput(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setStyle("datatypes");
    }
  };
  Blockly.Blocks['input_char'] = {
    init: function() {
      this.appendDummyInput('NAME')
        .appendField('char')
        .appendField(new Blockly.FieldNumber(0, 0, 255), 'value');
      this.setOutput(true, null);
      this.setTooltip('');
      this.setHelpUrl('');
      this.setStyle("datatypes");
    }
  }; 
  Blockly.Blocks['input_bool'] = {
    init: function() {
      this.appendDummyInput('NAME')
        .appendField('bool')
        .appendField(new Blockly.FieldDropdown([
            ['true', 'value_true'],
            ['false', 'value_false']
          ]), 'value');
      this.setOutput(true, null);
      this.setTooltip('');
      this.setHelpUrl('');
      this.setStyle("datatypes");
    }
  };
  Blockly.Blocks['input_int'] = {
    init: function() {
      this.appendDummyInput('NAME')
        .appendField('int')
        .appendField(new Blockly.FieldNumber(0, -32768, 32767), 'value');
      this.setOutput(true, null);
      this.setTooltip('');
      this.setHelpUrl('');
      this.setStyle("datatypes");
    }
  };
  Blockly.Blocks['logic_if'] = {
    init: function() {
      this.appendValueInput('condition')
        .appendField('If');
      this.appendStatementInput('command');
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setTooltip('');
      this.setHelpUrl('');
      this.setStyle("logic");
    }
  };
  Blockly.Blocks['logic_math'] = {
    init: function() {
      this.appendValueInput('value1');
      this.appendValueInput('value2')
        .appendField(new Blockly.FieldDropdown([
            ['==', 'equal'],
            ['>=', 'greater_equal'],
            ['>', 'greater'],
            ['<=', 'smaller_equal'],
            ['<', 'smaller'],
            ['!=', 'not_equal']
          ]), 'equation');
      this.setInputsInline(true)
      this.setOutput(true, null);
      this.setTooltip('');
      this.setHelpUrl('');
      this.setStyle("logic");
    }
  };
  Blockly.Blocks['operators'] = 
{
    init: function() {
      this.appendValueInput('NAME');
      this.appendValueInput('NAME')
        .appendField(new Blockly.FieldDropdown([
            ['LEFT', 'opt_LEFT'],
            ['RIGHT', 'opt_RIGHT'],
            ['AND', 'opt_AND'],
            ['OR', 'opt_OR'],
            ['XOR', 'opt_XOR']
          ]), 'Operators');
      this.setInputsInline(true)
      this.setOutput(true, null);
      this.setTooltip('');
      this.setHelpUrl('');
    }
  };
                    
  Blockly.Blocks['math_basic'] =  {
    init: function() {
      this.appendValueInput('value1');
      this.appendValueInput('value2')
        .appendField(new Blockly.FieldDropdown([
            ['+', 'add'],
            ['-', 'sub'],
            ['*', 'mul'],
            ['/', 'div'],
            ['%', 'mod']
          ]), 'equation');
      this.setInputsInline(true)
      this.setOutput(true, null);
      this.setTooltip('');
      this.setHelpUrl('');
      this.setStyle("math");
    }
  };
  Blockly.Blocks['logic_when'] = {
    init: function() {
      this.appendValueInput('logic1')
        .appendField('When');
      this.appendStatementInput('NAME')
        .appendField('then');
      this.appendValueInput('logic2')
        .appendField('else when');
      this.appendStatementInput('NAME')
        .appendField('then');
      this.setInputsInline(true)
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setTooltip('');
      this.setHelpUrl('');
      this.setStyle("logic");
    }
  };
  Blockly.Blocks['logic_when2'] = {
    init: function() {
    this.appendValueInput('logic1')
      .appendField('When');
    this.appendStatementInput('statement1')
      .appendField('then');
    this.appendDummyInput('NAME')
      .appendField('else');
    this.appendStatementInput('statement2')
      .appendField('then');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setStyle("logic");
  }
};
Blockly.Blocks['scd41_start_measure_single'] = 
{
    init: function() {
        this.appendDummyInput('NAME')
          .appendField('Start new air quality measurement (single)');
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setTooltip('');
        this.setHelpUrl('');
        this.setStyle("scd41");
    }
};

Blockly.Blocks['scd41_retrieve_values'] = 
{
    init: function() {
        this.appendDummyInput('NAME')
          .appendField('Retrieve values from air quality sensor');
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setTooltip('');
        this.setHelpUrl('');
        this.setStyle("scd41");
      }
};

Blockly.Blocks['scd41_get_value_co2'] = 
{
    init: function() {
        this.appendDummyInput('NAME')
          .appendField('Air quality: CO2');
        this.setOutput(true, 'uint16');
        this.setTooltip('');
        this.setHelpUrl('');
        this.setStyle("scd41");
    }
};

Blockly.Blocks['scd41_get_value_temp'] = 
{
    init: function() {
        this.appendDummyInput('NAME')
          .appendField('Air quality: temperature');
        this.setOutput(true, 'float');
        this.setTooltip('');
        this.setHelpUrl('');
        this.setStyle("scd41");
    }
};

Blockly.Blocks['scd41_get_value_rh'] = 
{
    init: function() {
        this.appendDummyInput('NAME')
          .appendField('Air quality: Humidity');
        this.setOutput(true, 'float');
        this.setTooltip('');
        this.setHelpUrl('');
        this.setStyle("scd41");
    }
};

Blockly.Blocks['scd41_get_value'] = 
{
    init: function() {
        this.appendDummyInput('NAME')
          .appendField('Air quality:')
          .appendField(new Blockly.FieldDropdown([
              ['CO2', 'opt_co2'],
              ['Temperature', 'opt_temp'],
              ['Relative Humidity', 'opt_rh']
            ]), 'value');
        this.setOutput(true, 'uint16');
        this.setTooltip('');
        this.setHelpUrl('');
        this.setStyle("scd41");
    }
};
Blockly.Blocks['sevseg_print_number'] = 
{
    init: function() {
        this.appendValueInput('number')
        .setCheck('uint16')
          .appendField('Print');
        this.appendEndRowInput('NAME')
          .appendField('on 7 segment display');
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setTooltip('');
        this.setHelpUrl('');
        this.setStyle('sevseg');
      }    
};

Blockly.Blocks['sevseg_clear'] = 
{
  init: function() {
    this.appendDummyInput('NAME')
      .appendField('Clear 7 segment display');
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setStyle('sevseg');
  }
};

Blockly.Blocks['sevseg_print_digit'] = 
{
  init: function() {
    this.appendValueInput('number')
      .appendField('Print number');
    this.appendValueInput('digit')
      .appendField('on digit');
    this.appendEndRowInput('NAME')
      .appendField('of 7 segment display');
    this.setInputsInline(true)
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip('');
    this.setHelpUrl('');
    this.setStyle('sevseg');
  }
};

Blockly.Blocks['sevseg_print_float'] = 
{
    init: function() {
        this.appendValueInput('number')
        .setCheck('float')
          .appendField('Print float');
        this.appendEndRowInput('NAME')
          .appendField('on 7 segment display');
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setTooltip('');
        this.setHelpUrl('');
        this.setStyle('sevseg');
      }    
};
Blockly.Blocks['events_start'] = {
  init: function(){
    this.appendDummyInput()
      .appendField("Start");
    this.setNextStatement(true, null);
    this.setTooltip("Start");
    this.setHelpUrl("");
    this.setStyle("event");
    this.setDeletable(false);
    this.setEditable(false);
  }
};

Blockly.Blocks['events_loop'] = {
  init: function(){
    this.appendDummyInput()
      .appendField("Loop");
    this.setNextStatement(true, null);
    this.setTooltip("Loop");
    this.setHelpUrl("");
    this.setStyle("event");
    this.setDeletable(false);
    this.setEditable(false);
  }
};
Blockly.Blocks['spi_Device'] = 
{
    init: function() {
        this.appendDummyInput('NAME')
          .appendField('Create SPI-Device with ID')
          .appendField(new Blockly.FieldDropdown([
              ['1', 'opt_PC7'],
              ['2', 'opt_PC6'],
              ['3', 'opt_PC5'],
              ['4', 'opt_PC4'],
              ['5', 'opt_PC3'],
              ['6', 'opt_PC2']
            ]), 'Pin');
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setTooltip('');
        this.setHelpUrl('');
        this.setStyle('spi');
      }    
};
Blockly.Blocks['utils_delay'] = 
{
    init: function() {
        this.appendDummyInput('NAME')
          .appendField('Wait for')
          .appendField(new Blockly.FieldNumber(0, 1, Infinity, 1), 'num_time')
          .appendField(new Blockly.FieldDropdown([
              ['µs', 'opt_us'],
              ['ms', 'opt_ms'],
              ['s', 'opt_s']
            ]), 'set_time');
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setTooltip('');
        this.setHelpUrl('');
        this.setStyle('utils');
      }
      
};
                    
                    

                      

                      

                      

                      
 
                      
                      

                                        
                    