import tkinter as tk
import math
import serial
import time

#창의 가로 크기
WIDTH = 640
#창의 세로 크기
HEIGHT = 480
# 현재 레이더 방향
angle = 0
#레이더 회전 방향
direction = 0
#송신된 각도
sendAngle = 0
#각도, 거리
objects = [[0,0], [10, 0], [20, 0], [30, 0],[ 40, 0], [50, 0], [60, 0], [70, 0]
           , [80, 0], [90, 0], [100, 0], [110, 0], [120, 0], [130, 0], [140, 0],
           [150, 0], [160, 0], [170, 0], [180, 0]]

def draw_object(angle, distance):
    radius = WIDTH / 2
    x = radius + math.cos(angle * math.pi / 180) * distance
    y = radius - math.sin(angle * math.pi / 180) * distance
    canvas.create_oval(x-5, y-5, x+5, y+5, fill = 'green')
    
ser = serial.Serial("COM3", 115200)
window = tk.Tk()
window.title('초음파 레이더')
canvas = tk.Canvas(window, width=WIDTH, height=HEIGHT, bg = 'black')
canvas.pack()

def update_screen():
    global angle
    global direction
    global objects
    global sendAngle
    receivedDistance = 0;
    #각도전송
    if angle % 10 == 0:
        sendAngle = angle
        ser.write(f'A{sendAngle:03d}\n'.encode())
    #거리수신
    if ser.in_waiting > 0:
        data = ser.readline().decode()
        print(data)
        if data[0] == 'D':
            #데이터 파싱
            receivedDistance = int(data[1:])
            #데이터 업데이트
            for obj in objects:
                if obj[0] == sendAngle:
                    obj[1] = receivedDistance
    #레이더 선 그리기
    canvas.delete('all')
    radius = WIDTH / 2
    x = radius + math.cos(angle * math.pi / 180) * radius
    y = radius - math.sin(angle * math.pi / 180) * radius
    canvas.create_line(x, y, radius, radius, fill = 'green', width = 3)
    #물체 그리기
    for obj in objects:
        draw_object(obj[0], obj[1])
    #각도 업데이트
    if direction == 0:
        angle += 1
        if angle == 181:
            direction = 1
    else:
        angle -= 1
        if angle == -1:
            direction = 0
    # 재귀 호출
    canvas.after(50, update_screen)
    
update_screen()
window.mainloop()
