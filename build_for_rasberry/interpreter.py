#coding=utf-8
# Written by leuk0cyte, 2019
# 一个简单的命令解释器，利用动态类型进行封装，使用时需要先进行实例化。
# 脚本格式：指令 参数1 参数2 时间参数（换行符）
# 支持从excel导出的txt文件
# 脚本末尾必须以换行符结尾，否则不能读取到正确的指令行数。
'''
脚本编写示例：
abc 02 02 03
abs 33 22 11
sss 22 11 11
aaa 33 44 22
asd 02 33 11
左转 03 11 33

（最后一行必须是空行）
'''
# 实例化时，输入脚本文件存放的地址。
class COM_INTERPRETER:
    def __init__(self,command_file_path):
        #打开命令文件，写入内存
        with open(command_file_path,"r",encoding='UTF-8') as command_file:
            self.commands = command_file.readlines()

    #计数文件中的指令
    def line_count(self):
        line_counter = 0
        #遍历脚本字符，以换行符个数计算行数
        for line in self.commands:
            for char in line:
                if char == '\n':
                    line_counter += 1
        print('we have %i commands'%line_counter)
        return line_counter

    # 输入行数，返回包含该行命令中所有参数的列表
    # P.S.: 行数从0开始计算
    def ins(self,line_no):
        command = self.commands[line_no]
        len_of_command = len(command)

        #利用制表符，换行符或空格作为分隔符，分割参数
        position_of_separator = []
        for i in range(len_of_command):
            if command[i] == '\t' or command[i] == '\n' or command[i] == ' ':
                position_of_separator.append(i)
        
        instruction = command[              0                 : position_of_separator[0] ]
        parm1       = command[ (position_of_separator[0] + 1) : position_of_separator[1] ]
        parm2       = command[ (position_of_separator[1] + 1) : position_of_separator[2] ]
        time        = command[ (position_of_separator[2] + 1) : position_of_separator[3] ]

        ins_list = [instruction,parm1,parm2,time]
        return ins_list
