# Coding Style Guidelines 
This document is intended for everyone who contributes to the GLEE2023 code. The basic convnetions for naming, indentation, and comments are explained here. These guidelines will uniformize the overall look of the code and will aim to increase organization, readability and as a result, faster comprehension of the code. 

## Naming Convention
* We follow the standard C++ naming conventions
* Variables: nouns, camel case with the first word in small case and the first letter of each consecutive word capitalized
* Registers: nouns, all capitalized with an underscore between each word
* Functions: verbs, camel case with the first word in small case and the first letter of each consecutive word capitalized
* Classes: nouns, first letter of each word capitalized
* Folders: nouns, first letter of each word capitalized with an underscore between each word

## Indentation 
* One tab = four spaces
* Nested blocks of code should be properly indented with each new block adding an additional tab

Example: 

[[https://github.com/GLEE_2023/GLEE_2023/blob/main/IndentationExample.png|alt=EX]]

## Comments
* Block comments: There should be a block comment immediately before each function that lists the parameters, what this function returns, and the purpose of this function.
* One-line comments: When needed, one to two-line comments should be used to explain the function and purpose of this line of code when it is more complex or not self-explanatory. This type of comment should be in its own line immediately before the line of code it is describing.
