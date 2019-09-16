import pygame

pygame.init()
clock = pygame.time.Clock()
pygame.joystick.init()

# Get count of joysticks
joystick_count = pygame.joystick.get_count()
print("Number of joysticks: {}\n".format(joystick_count))

joystick = pygame.joystick.Joystick(0)
joystick.init()

# Get the name from the OS for the controller/joystick
name = joystick.get_name()
print("Joystick name: {}\n".format(name))

# Usually axis run in pairs, up/down for one, and left/right for
# the other.
axes = joystick.get_numaxes()
print("Number of axes: {}".format(axes) )

done = False
while done==False:
    # EVENT PROCESSING STEP
    for event in pygame.event.get(): # User did something
        if event.type == pygame.QUIT: # If user clicked close
            done=True # Flag that we are done so we exit this loop
    
    for i in range( axes ):
            axis = joystick.get_axis( i )
            print("Axis {} value: {:>6.3f}".format(i, axis) )

