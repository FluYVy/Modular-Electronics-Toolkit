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