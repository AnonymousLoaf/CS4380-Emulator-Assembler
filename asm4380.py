def pass_one_parse(asm_file: str):
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
            raw_asm[i] = line.split()
    # remove all comments
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
            if ";" in j:
                # remove all elements including and after the ;
                raw_asm[i] = raw_asm[i][:raw_asm[i].index(j)]
                break
    return raw_asm

def main(asm_file):
    raw_asm = pass_one_parse(asm_file)
    # remove all comments
    clean_asm = clean_comments(raw_asm)
    print(clean_asm)

main('HelloWorld.asm')