import asm4380

def test_process_code_instruction():
    # Test valid instruction
    assert asm4380.process_code_instruction(['MOV', 'R1', 'R2']) == None
    
    # Test invalid instruction
    assert asm4380.process_code_instruction(['INVALID', 'R1', 'R2']) == -1
    
    # Test invalid number of operands
    assert asm4380.process_code_instruction(['MOV', 'R1']) == None # should print error message
    
    # Test JMP instruction with label
    assert asm4380.process_code_instruction(['JMP', 'LABEL']) == None
    
    # Test JMP instruction with immediate value
    assert asm4380.process_code_instruction(['JMP', '#10']) == None
    
    # Test TRP instruction
    assert asm4380.process_code_instruction(['TRP', '#1']) == None
    
    # Test LDR instruction with register and label
    assert asm4380.process_code_instruction(['LDR', 'R1', 'LABEL']) == None
    
    # Test LDR instruction with register and data label
    assert asm4380.process_code_instruction(['LDR', 'R1', 'DATA_LABEL']) == None
    
    # Test LDR instruction with register and immediate value
    assert asm4380.process_code_instruction(['LDR', 'R1', '#10']) == None