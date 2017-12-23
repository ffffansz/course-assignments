# -*- coding: utf-8 -*-
from cocos.actions import *
from atlas import *
import common
import game_controller

#两个相连接地面，不断循环向左移动一个屏宽，再回到原点
def createLand():
	landHeight = atlas["land"]["height"]/4

	#first moving land
	land_1 = createAtlasSprite("land")
	land_1.position = common.visibleSize["width"] / 2, landHeight

	move1 = MoveTo((- common.visibleSize["width"]/ 2, landHeight), 4)
	reset1 = Place((common.visibleSize["width"]/ 2, landHeight))
	land_1.do(Repeat(sequence(move1, reset1)))

	#second moving land
	land_2 = createAtlasSprite("land")
	land_2.position = common.visibleSize["width"] * 3 / 2, landHeight

	move2 = MoveTo((common.visibleSize["width"] / 2, landHeight), 4)
	reset2 = Place((common.visibleSize["width"] * 3 / 2, landHeight))

	land_2.do(Repeat(sequence(move2, reset2)))
	
	return land_1, land_2

def setLandSpeed(land_1, land_2):
	landHeight = atlas["land"]["height"]/4
	land_1.position = common.visibleSize["width"] / 2, landHeight
	move1 = MoveTo((- common.visibleSize["width"] / 2, landHeight), 4/game_controller.difficulty_x)
	reset1 = Place((common.visibleSize["width"] / 2, landHeight))
	land_1.do(Repeat(sequence(move1, reset1)))
	land_2.position = common.visibleSize["width"] * 3 / 2, landHeight
	move2 = MoveTo((common.visibleSize["width"] / 2, landHeight), 4/game_controller.difficulty_x)
	reset2 = Place((common.visibleSize["width"] * 3 / 2, landHeight))
	land_2.do(Repeat(sequence(move2, reset2)))
