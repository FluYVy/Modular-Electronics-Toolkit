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