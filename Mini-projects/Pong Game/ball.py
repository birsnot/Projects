from turtle import Turtle
from random import random, randint, choice
from paddle import Paddle

WIDTH = 15
PACE = 15


class Ball(Turtle):
    def __init__(self, screen_height):
        super().__init__()
        self.screen_height = screen_height
        self.shape("circle")
        self.speed("fastest")
        self.turtlesize(WIDTH/20, WIDTH/20)
        self.color("white")
        self.pu()
        self.reset_ball()

    def reset_ball(self):
        self.ball_speed = 0.05
        new_y = self.screen_height*(random()-0.5)
        self.goto(0, new_y)
        self.x_slope = choice([-1, 1])
        self.y_slope = choice([-1, 1])

    def move(self, player1, player2):
        '''Controls the movement of the ball'''
        new_x = self.xcor() + PACE*self.x_slope
        new_y = self.ycor() + PACE*self.y_slope
        self.goto(new_x, new_y)

        if abs(self.ycor()) >= self.screen_height/2-WIDTH:
            self.y_slope *= -1
        if self.xcor() >= player1.x-20 and self.xcor() <= player1.x+5:
            if abs(self.ycor() - player1.ycor()) < 50 + WIDTH:
                self.x_slope = -1
                self.ball_speed *= 0.9
        elif self.xcor() <= player2.x+20 and self.xcor() >= player2.x-5:
            if abs(self.ycor()-player2.ycor()) < 50 + WIDTH:
                self.x_slope = 1
                self.ball_speed *= 0.9
        elif self.xcor() >= 1.25*player1.x:
            return 2
        elif self.xcor() <= 1.25*player2.x:
            return 1
        return 0