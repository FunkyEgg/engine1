# Usage:
# scripts/texture_to_c.py <textures> <out_file.h>
# <textures>: Texture directories
# <out_file.h>: Last arg is output file

import sys
import re
import os

if __name__ == "__main__":
    if (len(sys.argv) < 3):
        print("Incorrect usage: scripts/shader_to_c_py: <shader_dirs> <outfile.h>")
        exit(-1)

    out_file = len(sys.argv) - 1

    output_file_contents = ""
    for i in range(1, out_file):
        for path, subdirs, files in os.walk(sys.argv[i]):
            for file_name in files:
                file_full_path = os.path.join(path, file_name)

                with open(file_full_path, "rb") as file:
                    file_name_clean = re.sub(r"[-.]", "_", file_name)
                    output = f"const uint8_t {file_name_clean}_texture_source[] = {{\n"

                    counter = 0
                    while True:
                        byte = file.read(1).hex()
                        if not byte:
                            break

                        if counter == 0:
                            output += f"\t0x{byte}"
                            counter += 1
                        elif counter == 16:
                            output += f",0x{byte},\n"
                            counter = 0
                        else:
                            output += f",0x{byte}"
                            counter += 1

                    if counter != 16:
                        output += "\n};\n"
                    else:
                        output += "};\n"

                    output_file_contents += output

    with open(sys.argv[out_file], "w") as file:
        safe_name = re.sub(r"[-.]", "_", os.path.basename(file.name)).upper()
        file.write((
            f"#ifndef TEXTUREGEN_{safe_name}\n"
            f"#define TEXTUREGEN_{safe_name}\n"
            f"#include <stdint.h>\n"
            f"{output_file_contents}"
            f"#endif"
        ))
