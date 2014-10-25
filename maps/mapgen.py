# Generates 50x50 world map.
from random import randrange
import numpy as np

UPPERQUAD = 1
LOWERQUAD = 2
FULL = 3

def Fill(Map, row, col, char):
	if row<len(Map) and col<len(Map[0]) and row >= 0 and col >= 0:
		if not Map[row][col] == '.':
			return
		Map[row][col] = char
		Fill(Map, row-1, col, char)
		Fill(Map, row+1, col, char)
		Fill(Map, row, col-1, char)	
		Fill(Map, row, col+1, char)		
	else:
		return

def DrawCircle(x0, y0, radius, code, Map, char):
	Coord_list = set()
	tmp = []
	#Mid-point circle Algorithm
	y0 = -y0;
	x = radius
	y = 0
	radiusError = 1-x
	
	while x >= y:
		if code == LOWERQUAD:
			Map[x + x0][-y + y0] = char
			Map[y + x0][-x + y0] = char
			tmp.append([ x + x0, -y + y0, y + x0, -x + y0])
		if code == UPPERQUAD:	
			Map[-x + x0][y + y0] = char
			Map[-y + x0][x + y0] = char
			tmp.append([-x + x0, y + y0, -y + x0, x + y0])
		if code == FULL:
			Map[x + x0][y + y0] = char
			Map[y + x0][x + y0] = char
			Map[-x + x0][y + y0] = char
			Map[-y + x0][x + y0] = char
			Map[-x + x0][-y + y0] = char
			Map[-y + x0][-x + y0] = char
			Map[x + x0][-y + y0] = char
			Map[y + x0][-x + y0] = char
			tmp.append([x + x0, y + y0, y + x0, x + y0, -x + x0, y + y0, -y + x0, x + y0, -x + x0, -y + y0, -y + x0, -x + y0, -y + x0, -x + y0, y + x0, -x + y0])
		y += 1
		if radiusError < 0:
			radiusError += 2 * y + 1
		else:
			x -= 1
			radiusError += 2 * (y - x + 1)
	
	for i in tmp:
		j = 0
		while j < len(i):
			Coord_list.add((i[j] % len(Map[0]), i[j+1] % len(Map)))	
			j += 2
	return Coord_list		

def GenCoordList(capacity, Map):
	s1 = 0
	U = []
	s2 = 0
	L = []

	while s1 < capacity or s2 < capacity :
		x = randrange(len(Map[0])-1)
		y = randrange(len(Map)-1)
		if x > y and s1 < capacity and Map[y][x] == '.' and not U.__contains__([x,y]):
			U.append([x,y])
			s1 += 1
		elif x < y and s2 < capacity  and Map[y][x] == '.' and not L.__contains__([x,y]):
			L.append([x,y])
			s2 += 1	
	return L, U

def Print(Map):
	print "MAP:"
	for i in Map:
		s = ""
		for j in i:
			s += j+' '
		print s

def print_Coord_list(Coord_list):
	s = ""
	for i in Coord_list:
		s += str(i) + " "
	return s	

rows = 50
cols = 50

Map = [['.' for i in range(cols)] for j in range(rows)]

# Draw Territories
Coord_list = DrawCircle(-1, 0, rows/8, UPPERQUAD, Map, 'A')
print "TEAM_A_SPAWN_BORDER: ", print_Coord_list(Coord_list)
Fill(Map, rows-1, 0, 'A')

Coord_list = DrawCircle(0, 1, rows/8, LOWERQUAD, Map, 'B')
print "TEAM_B_SPAWN_BORDER: ", print_Coord_list(Coord_list)
Fill(Map, 0, cols-1, 'B')

Coord_list = DrawCircle(cols/2-cols/6, rows/2+rows/6, rows/12, FULL, Map, 'a')
print "TEAM_A_TEMPLE_BORDER: ", print_Coord_list(Coord_list)

Coord_list = DrawCircle(cols/2+cols/6, rows/2-rows/6, rows/12, FULL, Map, 'b')
print "TEAM_B_TEMPLE_BORDER: ", print_Coord_list(Coord_list)

Fill(Map, rows/2-rows/6, cols/2-cols/6, 'a')
print "A_TEAM_TEMPLE_CENTER: ", str((rows/2-rows/6, cols/2-cols/6))
Fill(Map, rows/2+rows/6, cols/2+cols/6, 'b')
print "B_TEAM_TEMPLE_CENTER: ", str((rows/2+rows/6, cols/2+cols/6))

# Draw Jungle
L, U = GenCoordList(rows*cols/25, Map)
for j in [L,U]:
	for i in j:
		Map[i[1]][i[0]] = 'J'


# Draw Collectible Items
items = list(np.random.random_integers(1, 9, size=(1,rows*cols/50))[0])
items = [str(i) for i in items]
L, U = GenCoordList(rows*cols/100, Map)
for j in [L,U]:
	for i in range(len(j)):
		Map[j[i][1]][j[i][0]] = items[i]

Print(Map)
