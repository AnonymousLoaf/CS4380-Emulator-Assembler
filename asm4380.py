INSTRUCTION_MAP = {
    'JMP':  (1).to_bytes(4, "little", signed=True),
    'MOV':  (7).to_bytes(4, "little", signed=True),
    'STR':  (9).to_bytes(4, "little", signed=True),
    'LDR':  (10).to_bytes(4, "little", signed=True),
    'STB':  (11).to_bytes(4, "little", signed=True),
    'LDB':  (12).to_bytes(4, "little", signed=True),
    'ADD':  (13).to_bytes(4, "little", signed=True),
    'SUB':  (15).to_bytes(4, "little", signed=True),
    'MUL':  (16).to_bytes(4, "little", signed=True),
    'DIV':  (17).to_bytes(4, "little", signed=True),
    'TRP':  (21).to_bytes(4, "little", signed=True),
    }

DIRECTIVE_MAP = {
    '.INT': (0).to_bytes(4, "little", signed=True),
    '.BYT': (1).to_bytes(4, "little", signed=True),
}

def _parse(asm_file: str):
    """
    This function will parse the assembly file and return a dictionary with the
    line number as the key and the list of elements as the value.
    :param asm_file: the file to be parsed
    :return: a dictionary with the line number as the key and the list of elements as the value
    """
    with open(asm_file, 'r') as f:
        # open the file and parse on the whitspace
        raw_asm = {}
        i = 0
        for line in f:
            i += 1
            raw_asm[i] = line.strip().split()
    return raw_asm

def clean_comments(raw_asm: dict):
    """
    This function will remove all comments from the dictionary
    :param master: the dictionary to be cleaned
    :return: the cleaned dictionary
    """
    # delete any elements after a ; in the list
    for i in raw_asm:
        for j in raw_asm[i]:
            if j == "'":
                j.replace("'", " ")
            elif ";" in j:
                # remove all elements including and after the ;
                raw_asm[i] = raw_asm[i][:raw_asm[i].index(j)]
                break
            print(j)
    return raw_asm

def directive_to_bin(label: str, type: str, value: str):
    """
    This function will convert the directive to binary
    :param label: the label of the directive
    :param type: the type of the directive
    :param value: the value of the directive
    :return: the binary representation of the directive
    """
    print(value)
    if type not in DIRECTIVE_MAP:
        raise ValueError(f"Invalid directive: {type}")
    elif type == ".INT":
        # convert the value to a 32 bit signed integer
        return (int(value)).to_bytes(4, "little", signed=True)
    elif type == ".BYT":
        # convert the value to a 8 bit memory location and initialize with optional decimal or ascii code
        #return (ord(value[1])).to_bytes(1, "little", signed=True)
        ...

def instruction_to_bin(command: str):
    """
    This function will convert the instruction to binary
    :param command: the command to be converted
    :return: the binary representation of the command
    """
    if command not in INSTRUCTION_MAP:
        raise ValueError(f"Invalid command: {command}")
    else:
        return INSTRUCTION_MAP[command]

def convert(clean_asm: dict):
    for key, value in clean_asm.items():
        if len(value) > 1:
            if value[1] == ".INT" or value[1] == ".BYT":
                directive_to_bin(value[0], value[1], value[2])
            ...


def main():
    #asm_file = input("Please enter the name of the assembly file: ")
    asm_file = "HelloWorld.asm"
    raw_asm = _parse(asm_file)
    clean_asm = clean_comments(raw_asm)
    #convert(clean_asm)
    '''with open('TEST.bin', 'wb') as f:
        f.write(instruction_to_bin("JMP"))'''

if __name__ == "__main__":
    main()