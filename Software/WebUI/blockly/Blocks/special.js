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