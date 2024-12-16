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