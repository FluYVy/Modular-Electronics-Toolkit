const toolbox = {
    "kind": "categoryToolbox",
    "contents": [
        {
            "kind": "category",
            "name": "datatypes",
            "categorystyle": "datatypes",
            "contents": [
                {
                    "kind": "block",
                    "type": "input_string"
                },
                {
                    "kind": "block",
                    "type": "input_char"
                },
                {
                    "kind": "block",
                    "type": "input_bool"
                },
                {
                    "kind": "block",
                    "type": "input_int"
                }
            ]
        },
        {
            "kind": "category",
            "name": "logic",
            "categorystyle": "logic",
            "contents": [
                {
                    "kind": "block",
                    "type": "logic_if"
                },
                {
                    "kind": "block",
                    "type": "logic_math"
                },
                {
                    "kind": "block",
                    "type": "logic_when"
                },
                {
                    "kind": "block",
                    "type": "logic_when2"
                }
            ]
        },
        {
            "kind": "category",
            "name": "math",
            "categorystyle": "math",
            "contents": [
                {
                    "kind": "block",
                    "type": "math_basic"
                },
                {
                    "kind": "block",
                    "type": "operators"
                }
            ] // Fehler: Es darf hier kein Komma sein, korrigiert.
        },
        {
            "kind": "category",
            "name": "Air Quality",
            "categorystyle": "scd41",
            "contents": [
                {
                    "kind": "block",
                    "type": "scd41_start_measure_single"
                },
                {
                    "kind": "block",
                    "type": "scd41_retrieve_values"
                },
                {
                    "kind": "block",
                    "type": "scd41_get_value_co2"
                },
                {
                    "kind": "block",
                    "type": "scd41_get_value_temp"
                },
                {
                    "kind": "block",
                    "type": "scd41_get_value_rh"
                }
            ]
        },
        {
            "kind": "category",
            "name": "7 Segment Display",
            "categorystyle": "sevseg",
            "contents": [
                {
                    "kind": "block",
                    "type": "sevseg_print_number"
                },
                {
                    "kind": "block",
                    "type": "sevseg_print_float"
                }
            ]
        },
        {
            "kind": "category",
            "name": "Utils",
            "categorystyle": "utils",
            "contents": [
                {
                    "kind": "block",
                    "type": "utils_delay"
                }
            ]
        },
        {
            "kind": "category",
            "name": "Pixelmatrix",
            "categorystyle": "pixelmatrix",
            "contents": [
                {
                    "kind": "block",
                    "type": "pixelmatrix"
                },
            ]
        },
       
    ]
};


        

const theme = Blockly.Theme.defineTheme('Theme', {
    'base': Blockly.Themes.Classic, // Optional, kann man entfernen, falls gewünscht.
    'blockStyles': {
        'scd41': {
            'colourPrimary': '#007fff'
        },
        'event': {
            'colourPrimary': '#00d800'
        },
        'sevseg': {
            'colourPrimary': '#7f00ff'
        },
        'utils': {
            'colourPrimary': '#aa0000'
        },
        'datatypes': {
            'colourPrimary': '#875F9A'
        },
        'logic': {
            'colourPrimary': '#195e83'
        },
        'pixelmatrix': {
            'colourPrimary': '#FF0000'
        },
        'math': {
            'colourPrimary': '#69bdd2'
        }
    },
    'categoryStyles': {
        'scd41': {
            'colour': '#007fff'
        },
        'sevseg': {
            'colour': '#7f00ff'
        },
        'utils': {
            'colour': '#aa0000'
        },
        'datatypes': {
            'colour': '#875F9A'
        },
        'logic': {
            'colour': '#195e83'
        },
        'pixelmatrix': {
            'colour': '#FF0000'
        },
        'math': {
            'colour': '#69bdd2'
        }
    }
});


const workspace = Blockly.inject('blocklyDiv', {theme: theme, toolbox: toolbox});
