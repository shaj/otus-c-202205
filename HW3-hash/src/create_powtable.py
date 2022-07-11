
POW_SIZE = 256
line_size = 7
pow_p = 1
p = 31

with open("powtable.h", "w") as powfile:
    powfile.write(f"#pragma once\n\n#define POW_SIZE {POW_SIZE}\n")
    powfile.write("\nstatic const unsigned long long p_pow[POW_SIZE] = {\n")
    for i in range(POW_SIZE // line_size):
        powfile.write("    ")
        for j in range(line_size):
            powfile.write(f'{f"0x{pow_p:x}, ":>13}')
            pow_p = pow_p * p
            pow_p = pow_p & 0xffffffff
        powfile.write("\n")
    powfile.write("    ")
    for i in range((POW_SIZE % line_size) - 1):
        powfile.write(f'{f"0x{pow_p:x}, ":>13}')
        pow_p = pow_p * p
        pow_p = pow_p & 0xffffffff
    powfile.write(f'{f"0x{pow_p:x}":>11}')
    powfile.write("\n};\n")
