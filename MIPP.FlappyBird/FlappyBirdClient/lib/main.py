# -*- coding: utf-8 -*-
import cocos
from cocos.actions import *
from cocos.director import *
from cocos.scene import *
from game_controller import *
import common
import user
import pyglet
import sys

def on_close():
    try:
        user.user.logout()
    except:
        pass
    sys.exit(0)

def main():
    # initialize director 这是窗口宽、高、名，接口函数
    gameWindow = director.init(width=common.visibleSize["width"], height=common.visibleSize["height"], caption="Flappy Bird")
    gameWindow.on_close = on_close

    # turn off display FPS
    # director.show_FPS = True

    #run
    gameScene = Scene()  # 构造场景，接口函数
    game_start(gameScene)

    if director.scene:
        director.replace(gameScene)
    else:
        director.run(gameScene)

