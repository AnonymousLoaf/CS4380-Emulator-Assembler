import sys

INSTRUCTION_MAP = {
    'JMP': { 
        'opcode': (1).to_bytes(4, "little", signed=False),
        'operands': 1 },
    'JMR': {
        'opcode': (2).to_bytes(4, "little", signed=False),
        'operands': 1 },
    'BNZ': {
        'opcode': (3).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'BGT': {
        'opcode': (4).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'BLT': {
        'opcode': (5).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'BRZ': {
        'opcode': (6).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'MOV': { 
        'opcode': (7).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'LDA': {
        'opcode': (8).to_bytes(4, "little", signed=False),
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
    'ADDI': {
        'opcode': (14).to_bytes(4, "little", signed=False),
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
    'AND': {
        'opcode': (18).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'OR': {
        'opcode': (19).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'CMP': {
        'opcode': (20).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'TRP': { 
        'opcode': (21).to_bytes(4, "little", signed=False),
        'operands': 1 },
    'ISTR': {
        'opcode': (22).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'ILDR': {
        'opcode': (23).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'ISTB': {
        'opcode': (24).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'ILDB': {
        'opcode': (25).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'MOVI': {
        'opcode': (31).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'CMPI': {
        'opcode': (32).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'MULI': {
        'opcode': (33).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'DIVI': {
        'opcode': (34).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'ALCI': {
        'opcode': (35).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'ALLC': {
        'opcode': (36).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'IALLC': {
        'opcode': (37).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'SDIV': {
        'opcode': (38).to_bytes(4, "little", signed=False),
        'operands': 2 },
    'PSHR': {
        'opcode': (40).to_bytes(4, "little", signed=False),
        'operands': 1 },
    'PSHB': {
        'opcode': (41).to_bytes(4, "little", signed=False),
        'operands': 1 },
    'POPR': {
        'opcode': (42).to_bytes(4, "little", signed=False),
        'operands': 1 },
    'POPB': {
        'opcode': (43).to_bytes(4, "little", signed=False),
        'operands': 1 },
    'CALL': {
        'opcode': (44).to_bytes(4, "little", signed=False),
        'operands': 1 },
    'RET': {
        'opcode': (45).to_bytes(4, "little", signed=False),
        'operands': 0 },
}

DIRECTIVE_MAP = ['.INT', '.BYT', '.BTS', '.STR']

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
            if '.STR' in line.upper():
                # Extract the string from the line
                string_start = line.find('"')
                string_end = line.rfind('"')
                if string_start != -1 and string_end != -1:
                    temp_string = line[string_start:string_end+1]
                    line = line.replace(temp_string, 'temp_string')
            line = remove_comments(line)
            line_elements = line.strip().split()
            #Put the space and ; back in
            for idx in range(len(line_elements)):
                if line_elements[idx] == 'temp_space':
                    line_elements[idx] = "' '"
                elif line_elements[idx] == 'temp_semicolon':
                    line_elements[idx] = "';'"
                elif line_elements[idx] == 'temp_string':
                    line_elements[idx] = temp_string
            if len(line_elements) > 0:
                check_upper = line_elements[0].upper()
            else:
                check_upper = ''
            #Handle directive with no label
            if check_upper in DIRECTIVE_MAP:
                if check_upper == '.INT':
                    num_bytes = 4
                elif check_upper == '.BYT':
                    num_bytes = 1
                elif check_upper == '.BTS':
                    num_bytes = int(line_elements[1].replace('#', ''))
                elif check_upper == '.STR':
                    if line_elements[1][0] != '"' or line_elements[1][-1] != '"':
                        num_bytes = int(line_elements[1].replace('#', '')) + 2
                    else:
                        num_bytes = len(line_elements[1]) + 2
                num_data_bytes += num_bytes
                bytes_from_start += num_bytes
                instructions.append(line_elements)
            #Handle directive with label
            elif (len(line_elements) > 0) and check_upper not in DIRECTIVE_MAP and check_upper not in INSTRUCTION_MAP:
                label = line_elements[0]
                if label[0] == '_':
                    print(f"Assembler error encountered on line {line_num}!")
                    exit(2)
                line_elements = line_elements[1:]
                check_upper = line_elements[0].upper()
                if check_upper in DIRECTIVE_MAP:
                    if check_upper == '.INT':
                        num_bytes = 4
                    elif check_upper == '.BYT':
                        num_bytes = 1
                    elif check_upper == '.BTS':
                        num_bytes = int(line_elements[1].replace('#', ''))
                    elif check_upper == '.STR':
                        if line_elements[1][0] != '"' or line_elements[1][-1] != '"':
                            num_bytes = len(line_elements[1].replace('#', '')) + 2
                        else:
                            # Handle escape sequences
                            escaped_string = line_elements[1][1:-1].encode().decode('unicode-escape')
                            num_bytes = len(escaped_string) + 2
                    data_labels[label] = bytes_from_start
                    num_data_bytes += num_bytes
                    bytes_from_start += num_bytes
                if check_upper in INSTRUCTION_MAP:
                    code_labels[label] = bytes_from_start
                    num_bytes = 12
                    bytes_from_start += num_bytes
                instructions.append(line_elements)
            elif check_upper in INSTRUCTION_MAP:
                num_bytes = 12
                bytes_from_start += num_bytes
                instructions.append(line_elements)
            else:
                pass
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
            data = ord(data).to_bytes(1, "little", signed=False)
            write_to_file(data)
    elif data_type == '.BTS':
        num_bytes = int(instruction[1].replace('#', ''))
        data = (0).to_bytes(num_bytes, "little", signed=False)
        write_to_file(data)

    elif data_type == '.STR':
        if data[0] != '"' or data[-1] != '"':
            print(data[0])
            num_bytes = int(instruction[1].replace('#', ''))
            data = int(num_bytes).to_bytes(1, "little", signed=True)
            write_to_file(data)
            data = (0).to_bytes(num_bytes + 1, "little", signed=False)
            write_to_file(data)
        else:
            first_byte = (len(data) - 1).to_bytes(1, "little", signed=True)
            write_to_file(first_byte)
            middle_bytes = data[1:-1]
            i = 0
            while i < len(middle_bytes):
                byte = middle_bytes[i]
                if byte == '\\':
                    if i + 1 < len(middle_bytes):
                        next_byte = middle_bytes[i + 1]
                        if next_byte in ['n', 't', 'r', "'", '\\', '"']:
                            # Convert escape sequences to their ASCII values
                            escape_mapping = {'n': '\n', 't': '\t', 'r': '\r', "'": "'", '\\': '\\', '"': '"'}
                            byte = escape_mapping.get(next_byte, next_byte)
                            i += 1  # Skip the next character as it has been processed
                data_byte = byte.encode('utf-8')
                write_to_file(data_byte)
                i += 1

            null_byte = (0).to_bytes(1, "little", signed=False)
            write_to_file(null_byte)

    else:
        print(f"Assembler error encountered on line {line_num}!")
        exit(2)

def process_code_instruction(instruction, line_num):
    instruction[0] = instruction[0].upper()
    if instruction[0] not in INSTRUCTION_MAP:
        print(f"Assembler error encountered on line {line_num + 1}!")
        #print(f'Invalid instruction')
        exit(2)
    else:
        operator = instruction.pop(0)
        num_operands = INSTRUCTION_MAP[operator]['operands']
        operands = ('').join(instruction).split(',')
        if len(instruction) < num_operands:
            print(f"Assembler error encountered on line {line_num + 1}!")
            print(f'Not enough operands for {operator}')
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
            elif operator == 'JMR':
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += REGISTER_MAP[operands[0]].to_bytes(4, "little", signed=False)
                bytecode += filler
                write_to_file(bytecode)
            # Branch instructions
            elif operator in ['BNZ', 'BGT', 'BLT', 'BRZ']:
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += REGISTER_MAP[operands[0].upper()].to_bytes(4, "little", signed=False)
                if operands[1] in code_labels:
                    bytecode += code_labels[operands[1]].to_bytes(4, "little", signed=False)
                else:
                    print(f'Error. Invalid label {operands[1]}')
                    exit(1)
                write_to_file(bytecode)

            # Instruction where Operand_2 is a label (Address)
            elif operator == 'LDA':
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += REGISTER_MAP[operands[0]].to_bytes(4, "little", signed=False)
                if operands[1] in code_labels:
                    bytecode += code_labels[operands[1]].to_bytes(4, "little", signed=False)
                elif operands[1] in data_labels:
                    bytecode += data_labels[operands[1]].to_bytes(4, "little", signed=False)
                else:
                    print(f'Error. Invalid label {operands[1]}')
                    exit(1)
                write_to_file(bytecode)
            
            # Instructions where Operand 1 is a register and Operand 2 is a register or a label
            elif operator in ['MOV', 'STR', 'LDR', 'STB', 'LDB', 'ADD', 'SUB', 'MUL', 'DIV']:
                operand1 = operands[0].strip().split()
                operand2 = operands[1].strip().split()
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += REGISTER_MAP[operand1[0].upper()].to_bytes(4, "little", signed=False)
                if operand2[0] in code_labels:
                    bytecode += code_labels[operand2[0]].to_bytes(4, "little", signed=False)
                elif operand2[0] in data_labels:
                    bytecode += data_labels[operand2[0]].to_bytes(4, "little", signed=False)
                elif operand2[0].upper() in REGISTER_MAP:
                    bytecode += REGISTER_MAP[operand2[0].upper()].to_bytes(4, "little", signed=False)
                else:
                    print(f"Assembler error encountered on line {line_num + 2}!")
                    #print(f'Invalid operand {operand2[0]}')
                    exit(2)
                write_to_file(bytecode)
            
            # CMP Instruction
            elif operator == 'CMP':
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += REGISTER_MAP[operands[0]].to_bytes(4, "little", signed=False)
                bytecode += REGISTER_MAP[operands[1]].to_bytes(4, "little", signed=False)
                write_to_file(bytecode)

            # TRP Instruction
            elif operator == 'TRP':
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += int(operands[0].replace('#', '')).to_bytes(4, "little", signed=False)
                bytecode += filler
                write_to_file(bytecode)

            # I instructions
            elif operator in ['ISTR', 'ILDR', 'ISTB', 'ILDB']:
                operand1 = operands[0].strip().split()
                operand2 = operands[1].strip().split()
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                if operand1[0].upper() not in REGISTER_MAP or operand2[0].upper() not in REGISTER_MAP:
                    print(f'Error. Invalid register {operand1[0]}')
                    exit(1)
                else:
                    bytecode += REGISTER_MAP[operand1[0].upper()].to_bytes(4, "little", signed=False)
                    bytecode += REGISTER_MAP[operand2[0].upper()].to_bytes(4, "little", signed=False)
                write_to_file(bytecode)

            # Immediate Instruction
            elif operator in ['MOVI', 'CMPI', 'ADDI', 'MULI', 'DIVI', 'ALCI']:
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += REGISTER_MAP[operands[0].replace('r', 'R')].to_bytes(4, "little", signed=False)
                bytecode += int(operands[1].replace('#', '')).to_bytes(4, "little", signed=True)
                write_to_file(bytecode)

            # ALLC Instruction
            elif operator == 'ALLC':
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += REGISTER_MAP[operands[0].replace('r', 'R')].to_bytes(4, "little", signed=False)
                bytecode += int(operands[1].replace('#', '')).to_bytes(4, "little", signed=False)
                write_to_file(bytecode)
            
            # IALLC Instruction
            elif operator == 'IALLC':
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += REGISTER_MAP[operands[0].replace('r', 'R')].to_bytes(4, "little", signed=False)
                bytecode += REGISTER_MAP[operands[1].replace('r', 'R')].to_bytes(4, "little", signed=False)
                write_to_file(bytecode)
            
            # SDIV Instruction
            elif operator == 'SDIV':
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += REGISTER_MAP[operands[0].replace('r', 'R')].to_bytes(4, "little", signed=False)
                bytecode += REGISTER_MAP[operands[1].replace('r', 'R')].to_bytes(4, "little", signed=False)
                write_to_file(bytecode)

            # Stack Instruction
            elif operator in ['PSHR', 'PSHB', 'POPR', 'POPB']:
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += REGISTER_MAP[operands[0].replace('r', 'R')].to_bytes(4, "little", signed=False)
                bytecode += filler
                write_to_file(bytecode)

            # CALL Instruction
            elif operator == 'CALL':
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                if operands[0] in code_labels:
                    bytecode += code_labels[operands[0]].to_bytes(4, "little", signed=False)
                else:
                    print(f'Error. Invalid label {operands[0]}')
                    exit(1)
                write_to_file(bytecode)
            
            # RET Instruction
            elif operator == 'RET':
                bytecode = INSTRUCTION_MAP[operator]['opcode']
                bytecode += filler
                bytecode += filler
                write_to_file(bytecode)
            else:
                print(f"Assembler error encountered on line {line_num + 1}!")
                #print(f'Invalid instruction')
                exit(2)


def write_to_file(binary_instruction: bytes):
    global bin_file
    with open(bin_file, 'ab') as f:
        f.write(binary_instruction)

def main():
    global asm_file, bin_file
    asm_file = sys.argv[1]
    #asm_file = "program_a.asm"
    if not asm_file.endswith('.asm'):
        print("USAGE: python asm4380.py inputFile.asm")
        exit(1)
    else:
        bin_file = asm_file.replace('.asm', '.bin')

    with open(bin_file, 'wb') as f:
        pass
    
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