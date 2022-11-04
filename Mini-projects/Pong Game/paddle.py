from turtle import Turtle

WIDTH = 20
HEIGHT = 100
SPEED = WIDTH*5

class Paddle(Turtle):
    def __init__(self, x, screen_height):
        super().__init__()
        self.x = x
        self.screen_height = screen_height
        self.shape("square")
        self.turtlesize(HEIGHT/20, WIDTH/20)
        self.color("white")
        self.pu()
        self.goto(self.x, 0)
    
    def up(self):
        new_y = self.ycor() + SPEED
        self.goto(self.x, new_y)
        if new_y < self.screen_height/2 - HEIGHT/2:
            self.goto(self.x, new_y)
        else:
            self.goto(self.x, self.screen_height/2 - HEIGHT/2)

    def down(self):
        new_y = self.ycor() - SPEED
        if new_y > self.screen_height/-2 + HEIGHT/2:
            self.goto(self.x, new_y)
        else:
            self.goto(self.x, self.screen_height/-2 + HEIGHT/2)