def parse_instruction(instruction):
    # TODO: Implement instruction parsing logic
    pass

def parse_operand(operand):
    # TODO: Implement operand parsing logic
    pass

def convert_to_binary(instruction, operands):
    # TODO: Implement binary conversion logic
    pass

def assemble_file(asm_file, bin_file):
    with open(asm_file, 'r') as f_asm, open(bin_file, 'wb') as f_bin:
        for line in f_asm:
            instruction, operands = parse_instruction(line)
            binary = convert_to_binary(instruction, operands)
            f_bin.write(binary)