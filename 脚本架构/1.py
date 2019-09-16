from interpreter import *
com_interpreter = COM_INTERPRETER('工作簿1.txt')

line_count = com_interpreter.line_count()

for i in range(line_count):
    print(com_interpreter.ins(i))
