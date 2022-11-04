from time import sleep
from turtle import Screen
from paddle import Paddle
from scoreboard import Scoreboard
from ball import Ball

SCREEN_RATIO = float(0.9)
WIN_SCORE = 3

screen = Screen()
screen.tracer(0)
screen.bgcolor("black")
screen.setup(SCREEN_RATIO, SCREEN_RATIO)
screen_height = screen.window_height()
screen_width = screen.window_width()
player1_x = 0.45*screen_width
player2_x = -1*player1_x
player1 = Paddle(player1_x, screen_height)
player2 = Paddle(player2_x, screen_height)
score = Scoreboard(screen_height)
ball = Ball(screen_height)

screen.listen()
screen.onkeypress(player1.up, "Up")
screen.onkeypress(player1.down, "Down")
screen.onkeypress(player2.up, "w")
screen.onkeypress(player2.down, "s")

game_on = True
ball_status = 0
while score.score1 < WIN_SCORE and score.score2 < WIN_SCORE:
    if not game_on:
        ball.reset_ball()
        screen.update()
        game_on = True
        sleep(1)
    else:
        ball_status = ball.move(player1, player2)
        if ball_status == 1:
            game_on = False
            score.increase_score1()
        elif ball_status == 2:
            game_on = False
            score.increase_score2()
    screen.update()
    sleep(ball.ball_speed)
score.winner()
screen.exitonclick()
