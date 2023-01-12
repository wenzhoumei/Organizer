#!/usr/bin/env python
import random
import string

# Number of lines to generate
n = 1000

# Set of characters to use for the strings
characters = string.ascii_lowercase + string.digits + '-'

# Open the file in write mode
with open("root.space", "w") as file:
    for _ in range(n):
        # Generate the length of each string
        string_length = random.randint(1, 10)
        # Generate the two strings
        str1 = "".join(random.choices(characters, k=string_length))
        str2 = "".join(random.choices(characters, k=string_length))
        # Write the strings to the file, separated by a period
        file.write(f"{str1}.{str2}\n")
