import sys

INSTRUCTION_MAP = {
    'JMP': { 
        'opcode': (1).to_bytes(4, "little", signed=False),
        'operands': 1 },
    'MOV': { 
        'opcode': (7).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'STR': { 
        'opcode': (9).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'LDR': { 
        'opcode': (10).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'STB': { 
        'opcode': (11).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'LDB': { 
        'opcode': (12).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'ADD': { 
        'opcode': (13).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'SUB': { 
        'opcode': (15).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'MUL': { 
        'opcode': (16).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'DIV': { 
        'opcode': (17).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'TRP': { 
        'opcode': (21).to_bytes(4, "little", signed=False),
        'operands': 1 },
}

DIRECTIVE_MAP = {
    '.INT': (0).to_bytes(4, "little", signed=True),
    '.BYT': (0).to_bytes(1, "little", signed=False),
}

REGISTER_MAP = {
    'R0': 0, 'R1': 1, 'R2': 2, 'R3': 3, 'R4': 4, 'R5': 5,
    'R6': 6, 'R7': 7, 'R8': 8, 'R9': 9, 'R10': 10, 'R11': 11,
    'R12': 12, 'R13': 13, 'R14': 14, 'R15': 15, 'PC': 16, 'SL': 17,
    'SB': 18, 'SP': 19, 'FP': 20, 'HP': 21
}

filler = (0).to_bytes(4, "little", signed=False)

asm_file = ""
bin_file = ""
code_labels = {}
data_labels = {}
instructions = []
num_data_bytes = 0
bytes_from_start = 4

def remove_comments(line):
    """
    Removes comments from the line
    :param line: The line
    :return: The line without comments
    """
    # Remove comments from the line
    if ";" in line:
        line = line.split(";", 1)[0].strip()
    return line

def first_pass(asm_file):
    global num_data_bytes, bytes_from_start
    with open(asm_file, 'r') as f:
        line_num = 1

        for line in f:
            line = line.replace("' '", 'temp_space')
            line = line.replace("';'", 'temp_semicolon')
            line = remove_comments(line)
            line_elements = line.strip().split()
            #Put the space and ; back in
            for idx in range(len(line_elements)):
                if line_elements[idx] == 'temp_space':
                    line_elements[idx] = "' '"
                elif line_elements[idx] == 'temp_semicolon':
                    line_elements[idx] = "';'"
            if len(line_elements) > 0:
                check_upper = line_elements[0].upper()
            if line_elements and check_upper not in DIRECTIVE_MAP and check_upper not in INSTRUCTION_MAP:
                label = line_elements[0]
                if label[0] == '_':
                    print(f"Assembler error encountered on line {line_num}!")
                    exit(2)
                line_elements = line_elements[1:]
                check_upper = line_elements[0].upper()
                if check_upper in DIRECTIVE_MAP:
                    num_bytes = 4 if check_upper == '.INT' else 1
                    data_labels[label] = bytes_from_start
                    num_data_bytes += num_bytes
                    bytes_from_start += num_bytes
                if check_upper in INSTRUCTION_MAP:
                    code_labels[label] = bytes_from_start
                    num_bytes = 12
                    bytes_from_start += num_bytes
                instructions.append(line_elements)
            else:
                instructions.append(line_elements)
                if len(line_elements) > 0:
                    num_bytes = 12
                    bytes_from_start += num_bytes

            line_num += 1

    # Write location of first instruction to file
    num_data_bytes += 4
    data = int(num_data_bytes).to_bytes(4, "little", signed=False)
    write_to_file(data)

def second_pass(instructions):
    line_num = 1
    in_code_section = False
    for instruction in instructions:
        if not instruction:
            line_num += 1
            continue
        if instruction[0].upper() in DIRECTIVE_MAP and in_code_section:
            print(f"Assembler error encountered on line {line_num}!")
            #print(f'Data segment found after code segment')
            exit(2)
        elif instruction[0].upper() in DIRECTIVE_MAP and not in_code_section:
            process_data_instruction(instruction, line_num)
            line_num += 1
        elif instruction[0].upper() in INSTRUCTION_MAP:
            in_code_section = True
            process_code_instruction(instruction, line_num)
            line_num += 1
        else:
            print(f"Assembler error encountered on line {line_num}!")
            exit(2)

def process_data_instruction(instruction, line_num):
    data_type = instruction[0]
    data_type = data_type.upper()
    data = instruction[1] if len(instruction) > 1 else 0
    if data_type == '.INT':
        if isinstance(data, str):
            data = data.replace('#', '')
        data = int(data).to_bytes(4, "little", signed=True)
        write_to_file(data)

    elif data_type == '.BYT':
        if data == 0:
            data = data.to_bytes(1, "little", signed=False)
            write_to_file(data)
        elif "'" not in data:
            data = data.replace('#', '')
            if int(data) > 255 or int(data) < 0:
                print(f'Assembler error encountered on line {line_num}!')
                #print(f'Byte value out of range')
                exit(2)
            data = int(data).to_bytes(1, "little", signed=False)
            write_to_file(data)
        elif '\\n' in data:
            data = data.replace('\\n', '\n')
            data = ord(data[1]).to_bytes(1, "little", signed=False)
            write_to_file(data)
        else:
            data = data.replace("'", '')
            if '#' in data and len(data) > 1:
                data = data.replace('#', '')
                data = int(data)
                data = chr(data)
            data = ord(data).to_bytes(1, "little", signed=False)
            write_to_file(data)
    else:
        print(f"Assembler error encountered on line {line_num}!")
        exit(2)

def process_code_instruction(instruction, line_num):
    if instruction[0] not in INSTRUCTION_MAP:
        print(f"Assembler error encountered on line {line_num}!")
        #print(f'Invalid instruction')
        exit(2)
    else:
        operator = instruction.pop(0)
        num_operands = INSTRUCTION_MAP[operator]['operands']
        operands = ('').join(instruction).split(',')
        if len(instruction) < num_operands:
            print(f"Assembler error encountered on line {line_num}!")
            #print(f'Not enough operands for {operator}')
            exit(2)
        else:
            if operator == 'JMP':
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                if operands[0] in code_labels:
                    bytecode += code_labels[operands[0]].to_bytes(4, "little")
                else:
                    bytecode += int(operands[0].replace('#', '')).to_bytes(4, "little")
                bytecode += filler
                write_to_file(bytecode)
                
            if operator == 'TRP':
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += int(operands[0].replace('#', '')).to_bytes(4, "little", signed=False)
                bytecode += filler
                write_to_file(bytecode)

            elif operator in ['MOV', 'STR', 'LDR', 'STB', 'LDB', 'ADD', 'SUB', 'MUL', 'DIV']:
                operand1 = operands[0].strip().split()
                operand2 = operands[1].strip().split()
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += REGISTER_MAP[operand1[0]].to_bytes(4, "little", signed=False)
                if operand2[0] in code_labels:
                    bytecode += code_labels[operand2[0]].to_bytes(4, "little", signed=False)
                elif operand2[0] in data_labels:
                    bytecode += data_labels[operand2[0]].to_bytes(4, "little", signed=False)
                elif operand2[0] in REGISTER_MAP:
                    bytecode += REGISTER_MAP[operand2[0]].to_bytes(4, "little", signed=False)
                elif operand2[0].startswith('#'):
                    bytecode += int(operand2[0].replace('#', '')).to_bytes(4, "little", signed=True)
                else:
                    print(f"Assembler error encountered on line {line_num}!")
                    #print(f'Invalid operand {operand2[0]}')
                    exit(2)
                write_to_file(bytecode)

def write_to_file(binary_instruction: bytes):
    global bin_file
    with open(bin_file, 'ab') as f:
        f.write(binary_instruction)

def main():
    global asm_file, bin_file
    asm_file = sys.argv[1]
    if not asm_file.endswith('.asm'):
        print("USAGE: python asm4380.py inputFile.asm")
        exit(1)
    else:
        bin_file = asm_file.replace('.asm', '.bin')

    first_pass(asm_file)

    #print(f'Code labels: {code_labels}')
    #print(f'Data labels: {data_labels}')


    #Check if main exists
    if 'MAIN' not in code_labels:
        print("Error. MAIN label not found")
        exit(2)
    
    second_pass(instructions)
    exit(0)
if __name__ == "__main__":
    main()