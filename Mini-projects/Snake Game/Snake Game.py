###
# Author: Nardos Wehabe
###

from random import randint
from turtle import Turtle, Screen
import time

screen = Screen()
screen.tracer(0)
SCREEN_SIZE = 700  # Multiple of 20 is recommended
HALF_DOTS = int(SCREEN_SIZE/40) - 1
screen.setup(SCREEN_SIZE + 20, SCREEN_SIZE + 20)
screen.bgcolor("slate gray")
screen.title("Snake")

moves = [(-40, 0), (-20, 0), (0, 0)]
new_move = ()
score = 0

head = Turtle(shape="circle")
head.speed(0)
head.hideturtle()
head.pu()
head.color("medium spring green")
move_speed = 0.3

food = Turtle(shape="circle")
food.hideturtle()
food.pu()
food.shapesize(0.5, 0.5, 1)
food.color("yellow")

cursor = Turtle()
cursor.hideturtle()
cursor.pu()
cursor.color("orange")

setup = Turtle()
setup.hideturtle()
setup.pu()
setup.color("orange")
setup.setposition(0, SCREEN_SIZE/2 - 50)
my_font = 'Comic Sans MS'
setup.write(f"Score: ", True, align="center",
            font=(my_font, 16, 'normal'))
cursor.setposition(setup.position())
cursor.write(score, False, align="left",
             font=(my_font, 16, 'normal'))
setup.goto(SCREEN_SIZE/2 - 15, SCREEN_SIZE/2 - 15)
setup.pd()
setup.pensize(10)
for _ in range(4):
    setup.right(90)
    setup.forward(SCREEN_SIZE-30)

suspend = False
suspend_angle = 0.0
suspend_x = 0.0
suspend_y = 0.0


def update_screen():
    time.sleep(move_speed)
    screen.update()


def move_foward():
    head.forward(20)
    global the_food, score, new_move
    x = round(head.xcor())
    y = round(head.ycor())
    new_move = (x, y)
    time.sleep(move_speed)
    head.stamp()
    screen.update()
    if new_move == the_food:
        moves.append(new_move)
        score += 1
        update_score()
        the_food = make_food()
    elif (abs(x) >= SCREEN_SIZE/2-20) or (abs(y) >= SCREEN_SIZE/2-20) or (new_move in moves):
        head.clearstamps(1)
        screen.update()
        moves.pop(0)
        return False
    else:
        head.clearstamps(1)
        screen.update()
        moves.pop(0)
        moves.append(new_move)
    return True


def turn(angle, x_change, y_change):
    x = new_move[0] + x_change
    y = new_move[1] + y_change
    next_move = (x, y)
    global suspend, suspend_angle, suspend_x, suspend_y
    if next_move != moves[-2] and next_move != moves[-1]:
        head.seth(angle)
        suspend = False
    else:
        suspend = True
        suspend_angle = angle
        suspend_x = x_change
        suspend_y = y_change


def down_arrow():
    turn(270, 0, -20)


def up_arrow():
    turn(90, 0, 20)


def left_arrow():
    turn(180, -20, 0)


def right_arrow():
    turn(0, 20, 0)


def make_food():
    food.clearstamps()
    while True:
        x = 20 * randint(-1*HALF_DOTS, HALF_DOTS)
        y = 20 * randint(-1*HALF_DOTS, HALF_DOTS)
        new_food = (x, y)
        if new_food not in moves:
            food.setpos(new_food)
            food.stamp()
            screen.update()
            return new_food


def update_score():
    cursor.clear()
    cursor.write(score, False, align="left",
                 font=(my_font, 16, 'normal'))
    screen.update()


for p in moves:
    head.setposition(p)
    head.stamp()

the_food = make_food()
screen.update()

difficulty = screen.textinput(
    title="Choose Your Level", prompt="What difficulty do you want to play?\nEasy, Normal, Hard, Hardest, Inferno or Impossible:").lower()
if difficulty == "impossible":
    move_speed = 0.01
elif difficulty == 'inferno':
    move_speed = 0.02
elif difficulty == 'hardest':
    move_speed = 0.03
elif difficulty == 'hard':
    move_speed = 0.05
elif difficulty == 'normal':
    move_speed = 0.1
else:
    move_speed = 0.2
head.speed(move_speed)

screen.listen()
screen.onkeypress(down_arrow, "Down")
screen.onkeypress(up_arrow, "Up")
screen.onkeypress(left_arrow, "Left")
screen.onkeypress(right_arrow, "Right")

game_on = True
while game_on:
    game_on = move_foward()
    if suspend:
        turn(suspend_angle, suspend_x, suspend_y)
        suspend = False

cursor.home()
cursor.color("red")
cursor.write("Game Over.", False, align="center",
             font=(my_font, 16, 'normal'))

for _ in range(3):
    head.color("red")
    head.stamp()
    time.sleep(0.1)
    screen.update()
    head.color("medium spring green")
    head.stamp()
    time.sleep(0.1)
    screen.update()
screen.exitonclick()
