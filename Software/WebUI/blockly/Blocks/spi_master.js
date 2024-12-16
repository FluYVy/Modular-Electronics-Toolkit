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
}