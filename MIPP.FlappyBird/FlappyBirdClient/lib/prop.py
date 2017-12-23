# -*- coding: utf-8 -*-
from cocos.actions import *
from cocos.cocosnode import *
from cocos.collision_model import *
from cocos.actions import *
import random
from atlas import *
import game_controller
import common
import random
import bird
from life import *

# constants
propCount = 0
propHeight = 40
propWidth = 40
propRange = 3
gameSceneForAi = None
propLayer = None
spriteBird_prop = None

prop_type = [
    "life",
    "rush"
]
coldtime = None
life = 0
crossMode = False
crossCount = 0
reborn_ready = False
prop_index = [0, 1, 2]
props = {}
newProps = []
propsIndex = [0, 0, 0]
propType = {}   # 道具类型

def createProps(layer, gameScene, spriteBird, pipes):
    global spriteBird_prop, gameSceneForAi, movePropFunc, makePropFunc, propCount, newProps, propLayer, props, propsIndex, life, prop_index, crossMode, reborn_ready, crossCount
    propLayer = layer
    gameSceneForAi = gameScene
    spriteBird_prop = spriteBird
    coldtime = random.randint(1, propRange + 1)

    props = {}
    propsIndex = [None, None, None]
    prop_index = [0, 1, 2]
    newProps = []
    life = 0
    crossMode = False
    reborn_ready = False
    crossCount = 0

    def newProp(xPosition):
        global newProps
        heightOffset = random.randint(atlas["land"]["height"] + propHeight / 2, common.visibleSize["height"] - propHeight / 2)
        typeindex = random.randint(0, len(prop_type) - 1)
        try:
            propindex = prop_index.pop(0)
        except:
            return
        prop = CollidableRectSprite(prop_type[typeindex], xPosition, heightOffset, propWidth / 2, propHeight / 2)
        propsIndex[propindex] = prop
        props[propindex] = prop
        layer.add(prop, z=11)
        newProps.append(prop)

    def moveProp(dt):
        moveDistance = common.visibleSize["width"] / (2 * 60) * game_controller.difficulty_x  # 移动速度和land一致
        for i in range(3):
            if i not in prop_index:
                props[i].position = props[i].position[0] - moveDistance, props[i].position[1]
                if props[i].position[0] < -propWidth / 2:
                    removeProp(props[i])

    def makeProp(dt):
        global coldtime
        if coldtime <= 0:
            for i in pipes:
                if pipes[i].position[0] > common.visibleSize["width"]:
                    newProp(pipes[i].position[0] + atlas["pipe_up"]["width"] + propWidth)
                    coldtime = random.randint(1, propRange + 1)
                    break

    movePropFunc = moveProp
    makePropFunc = makeProp
    gameScene.schedule(moveProp)
    gameScene.schedule(makeProp)

    return props


def removeMovePropFunc(gameScene):
    global movePropFunc
    if movePropFunc:
        gameScene.unschedule(movePropFunc)

def removeMakePropFunc(gameScene):
    global makePropFunc
    if makePropFunc:
        gameScene.unschedule(makePropFunc)

def reduceColdtime():
    global coldtime
    coldtime -= 1
    return

def removeProp(prop):
    global propLayer, prop_index
    propLayer.remove(prop)
    index = propsIndex.index(prop)
    prop_index.append(index)
    props.pop(index)
    propsIndex[index] = None

def getLife():
    return life

def reduceLife():
    global life
    life -= 1
    setSpriteLifes(life)

def addLife():
    global life
    life += 1
    setSpriteLifes(life)

def getProps():
    return props

def getNewProps():
    return newProps

def addCrossCount():
    global crossCount, crossMode, gameSceneForAi, spriteBird_prop
    crossCount += 1
    if crossCount == 10:
        game_controller.difficulty_x = game_controller.difficulty_y
        crossMode = False
        spriteBird_prop.gravity = bird.gravity
        bird.addTouchHandler(gameSceneForAi, True, spriteBird_prop)
        action = ScaleTo(1, 0)
        spriteBird_prop.do(action)

def crossing():
    return crossMode

def crossModeOn():
    global crossCount, crossMode, gameSceneForAi, spriteBird_prop
    crossCount = 0
    if crossMode:
        return

    crossMode = True
    game_controller.difficulty_x = 20
    spriteBird_prop.position = spriteBird_prop.position[0], common.visibleSize["height"]/2
    spriteBird_prop.gravity = 0
    spriteBird_prop.velocity = (0, 0)
    bird.removeBirdTouchHandler(gameSceneForAi)
    action = ScaleTo(2, 0)
    spriteBird_prop.do(action)

def rebornReady():
    return reborn_ready

def rebornReadyOff():
    global reborn_ready, crossMode, gameSceneForAi, spriteBird_prop
    crossMode = False
    reborn_ready = False
    spriteBird_prop.gravity = bird.gravity
    bird.addTouchHandler(gameSceneForAi, True, spriteBird_prop)

def reborn():
    global reborn_ready, gameSceneForAi, spriteBird_prop
    reborn_ready = True
    spriteBird_prop.position = spriteBird_prop.position[0], common.visibleSize["height"]/2
    spriteBird_prop.gravity = 0
    spriteBird_prop.velocity = (0, 0)
    bird.removeBirdTouchHandler(gameSceneForAi)

    action = Blink(3, 1)
    spriteBird_prop.do(action)
