import {cGenerator, useIncludes, includesText, useInits, initText, defineText, useDefines} from './c_gen.js';

const supportedEvents = new Set([
  Blockly.Events.BLOCK_CHANGE,
  Blockly.Events.BLOCK_CREATE,
  Blockly.Events.BLOCK_DELETE,
  Blockly.Events.BLOCK_MOVE,
]);

cGenerator.scrub_ = function(block, code, thisOnly) {
    const nextBlock =
      block.nextConnection && block.nextConnection.targetBlock();
    if (nextBlock && !thisOnly) {
      return code + cGenerator.blockToCode(nextBlock);
    }
    return code;
};

var workspace = Blockly.getMainWorkspace();

function updateCode(event) {
    if (workspace.isDragging()) return; // Don't update while changes are happening.
    if (!supportedEvents.has(event.type)) return;
    useIncludes.fill(false);
    useDefines.fill(false);
    useInits.fill(false);
  
    var code = cGenerator.workspaceToCode(workspace);

    var code_start;
    var code_loop;

    if(code.indexOf('// start\n') > code.indexOf('// loop\n'))
    {
        code_start = code.split('// start\n')[1];
        code_loop = code.replace(code_start, '');
    }
    else
    {
        code_loop = code.split('// loop\n')[1];
        code_start = code.replace(code_loop, '');
    }

    const fullCode = addDefinitions() + '\n' + addInclude() + 'int main(void)\n{\n' + addInits() + code_start.replace('// loop\n', '').replace('// start\n', '') + '\nwhile (1)\n{\n' + code_loop.replace('// loop\n', '').replace('// start\n', '') + '}\n}';

    const complete_code = js_beautify(fullCode, {indent_size: 4, indent_with_tabs: false})

    document.getElementById('codeDiv').innerText = complete_code;
    document.getElementById('codeSaver').innerText = complete_code;
  };

  workspace.addChangeListener(updateCode);
  workspace.addChangeListener(Blockly.Events.disableOrphans);
  Blockly.serialization.blocks.append({'type': 'events_start'}, workspace).moveBy(20, 30);
  Blockly.serialization.blocks.append({'type': 'events_loop'}, workspace).moveBy(320, 30);


function addInclude()
{
  var includesStr = "";
  var someIncluded = false;

  for (let i = 0; i < useIncludes.length; i++)
  {
    if(useIncludes[i]){
      someIncluded = true;
      includesStr += includesText[i];
    }
  }

  return includesStr.replaceAll('\n\n', '\n') + (someIncluded ? "\n" : "");
};

function addInits()
{
    var initStr = "";
    var someInits = false;

  for (let i = 0; i < useInits.length; i++)
  {
    if(useInits[i]){
      someInits = true;
      initStr += initText[i];
    }
  }

  return initStr + (someInits ? "\n" : "");
}

function addDefinitions()
{
    var defStr = "";
    var someDef = false;

  for (let i = 0; i < useDefines.length; i++)
  {
    if(useDefines[i]){
      someDef = true;
      defStr += defineText[i];
    }
  }

  return defStr + (someDef ? "\n" : "");
}