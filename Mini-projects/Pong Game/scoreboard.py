from turtle import Turtle

FONT = "Comic Sans MS"

class Centerline(Turtle):
    def __init__(self, screen_height):
        super().__init__()
        self.screen_height = screen_height
        self.hideturtle()
        self.shape("square")
        self.pu()
        self.turtlesize(0.25,0.75)
        self.color("white")
        self.draw_center()
        
    def draw_center(self):
        center_top = self.screen_height/2-self.screen_height/20
        self.goto(0, center_top)
        self.setheading(270)
        while self.ycor() >= -1*center_top:
            self.stamp()
            self.forward(30)

class Scoreboard(Turtle):
    def __init__(self, screen_height):
        super().__init__()
        Centerline(screen_height)
        self.hideturtle()
        self.color("white")
        self.pu()
        self.screen_height = screen_height
        self.score1 = 0
        self.score2 = 0
        self.update_score()

    def increase_score1(self):
        self.score1 += 1
        self.update_score()
    
    def increase_score2(self):
        self.score2 += 1
        self.update_score()

    def update_score(self):
        self.clear()
        padding = self.screen_height/5
        self.goto(padding/2, self.screen_height/2-padding)
        self.write(self.score1, align = "left", font=(FONT,int(self.screen_height/10),"normal"))
        self.goto(padding/-2, self.screen_height/2-padding)
        self.write(self.score2, align = "right", font=(FONT,int(self.screen_height/10),"normal"))
    
    def winner(self):
        self.home()
        self.color("cyan")
        if self.score1 > self.score2:
            self.write("Player1👉 has won!", align="center",font=(FONT,int(self.screen_height/20),"normal"))
        else:
            self.write("Player2👈 has won!", align="center",font=(FONT,int(self.screen_height/20),"normal"))

