# -*- coding: utf-8 -*-
from cocos.actions import *
from atlas import *
import common
import game_controller

spriteLifes = {}
lifeLayer = None

def createLifeLayer(gameLayer):
    global lifeLayer
    lifeLayer = gameLayer
    temp = createAtlasSprite("life")
    temp.position = atlas["prop"]["width"]/2, atlas["prop"]["height"]/2
    lifeLayer.add(temp, z=50)
    setSpriteLifes(0)


def setSpriteLifes(life):
    global lifeLayer
    for k in spriteLifes:
        try:
            lifeLayer.remove(spriteLifes[k])
            spriteLifes[k] = None
        except:
            pass

    lifeStr = str(life)
    i = 0
    for d in lifeStr:
        s = createAtlasSprite("number_context_0"+d)
        s.position = atlas["prop"]["width"]+14*i+atlas["number_context_00"]["width"]/2, atlas["number_context_00"]["height"]/2
        lifeLayer.add(s, z=50)
        spriteLifes[i] = s
        i = i + 1