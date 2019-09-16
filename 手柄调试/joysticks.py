import pygame

class JOYSTICKS:
    def __init__(self):
        pygame.init()
        pygame.joystick.init()
        
        # 获取手柄数量，打印
        joystick_count = pygame.joystick.get_count()
        print("Number of joysticks: {}".format(joystick_count))
        
        if joystick_count<1:
            print('Error: it do not connect with a joystick')
        else:
            # 初始化0号手柄
            self.joystick = pygame.joystick.Joystick(0)
            self.joystick.init()

            # 获取手柄名称，打印
            name = self.joystick.get_name()
            print("Joystick name: {}".format(name))

            # 获取轴数，打印
            axes = self.joystick.get_numaxes()
            print("Number of axes: {}".format(axes) )

    
    #输入要获取的轴的编号，返回读数
    def axis(self,axis_no):
        pygame.event.get()
        axis_value = self.joystick.get_axis(axis_no)
        print(axis_no,axis_value)
        return axis_value