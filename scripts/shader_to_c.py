# Usage:
# scripts/shader_to_c.py <shaders> <out_file.h>
# <shaders>: Shader directories
# <out_file.h>: Last arg is always your output file

import sys
import os
import re

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

                with open(file_full_path) as file:
                    file_name_clean = re.sub(r"[-.]", "_", file_name)

                    output = f"static const char* {file_name_clean}_shader_source = \\"
                    content_lines = [line.strip() for line in file.readlines()]

                    for line in content_lines:
                        if line == "":
                            continue

                        output += f"\n\t\"{line}\\n\""
                    output += ";\n"
                    output_file_contents += f"{output}"

    with open(sys.argv[out_file], "w") as file:
        file.write(output_file_contents)
