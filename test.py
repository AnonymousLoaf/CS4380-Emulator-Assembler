# Define a dictionary to map mnemonics to binary opcodes
mnemonic_to_opcode = {
    'LDB': '0001',
    'TRP': '0010',
    'JMP': '0011',
}

# Create a dictionary to store label addresses
label_addresses = {}

# Open the input assembly file for reading
with open('HelloWorld.asm', 'r') as asm_file:
    lines = asm_file.readlines()

# Function to remove comments from a line
def remove_comments(line):
    return line.split(';', 1)[0].strip()

# Open the output binary file for writing
with open('Htest.bin', 'wb') as bin_file:
    address_counter = 0  # Initialize an address counter
    for line in lines:
        # Remove comments from the line
        line = remove_comments(line)
        
        # Skip blank lines and comment lines
        if not line:
            continue

        # Split the line into words and remove whitespace
        words = line.strip().split()

        # Check if the line defines a label
        if line.endswith(':'):
            label = words[0].strip(':')
            label_addresses[label] = address_counter
            continue

        # Check if the line contains an instruction or directive
        if words[0][0].isalnum():  # Check if the first character is alphanumeric (label or instruction)
            # Instruction line
            if words[0] in mnemonic_to_opcode:
                opcode = mnemonic_to_opcode[words[0]]  # Get the opcode
                binary_instruction = opcode

                # Process the operands and immediate values
                for i in range(1, len(words)):
                    operand = words[i]
                    if operand.startswith('#'):
                        # Remove the '#' and convert the immediate value to binary
                        immediate = int(operand[1:], 10)
                        if immediate >= 0 and immediate <= 255:
                            binary_instruction += format(immediate, '08b')
                        else:
                            print(f"Error: Immediate value {immediate} is out of range (0-255).")
                            # You may want to handle this error condition appropriately.
                    else:
                        # Check if the operand is a single character or a label
                        if len(operand) == 1:
                            # Convert the ASCII character to binary
                            binary_instruction += format(ord(operand), '08b')
                        elif operand in label_addresses:
                            # Replace the label with its address
                            label_address = label_addresses[operand]
                            binary_instruction += format(label_address, '08b')
                        else:
                            print(f"Error: Operand '{operand}' is not a single character or a valid label.")
                            # You may want to handle this error condition appropriately.

                # Ensure the binary instruction is 32 bits
                while len(binary_instruction) < 32:
                    binary_instruction = '0' + binary_instruction

                # Convert the binary instruction to bytes and write to the binary file
                for i in range(0, 32, 8):
                    bin_file.write(bytes([int(binary_instruction[i:i+8], 2)]))
                address_counter += 4  # Increment the address counter for each instruction (assuming 32-bit instructions)
            else:
                print(f"Error: Invalid mnemonic '{words[0]}' on line: {line}")
                # You may want to handle this error condition appropriately.
        else:
            # Directive line (you can handle directives as needed)
            # For now, skip directive lines
            continue

print("Assembly file translated to binary file successfully.")
