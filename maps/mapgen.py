# Generates 50x50 world map.
from random import randrange
import numpy as np

UPPERQUAD = 1
LOWERQUAD = 2
FULL = 3

def Fill(Map, row, col, char):
	if row<len(Map) and col<len(Map[0]) and row >= 0 and col >= 0:
		if not (Map[row][col] == '.' or Map[row][col] == 'w' ):
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
		if x > y and s1 < capacity and (Map[y][x] == '.' or Map[y][x] == 'w' ) and not U.__contains__([x,y]):
			U.append([x,y])
			s1 += 1
		elif x < y and s2 < capacity  and (Map[y][x] == '.' or Map[y][x] == 'w' ) and not L.__contains__([x,y]):
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
		s += str(i[0])+ " "+ str(i[1])+ " "
	return s	

def DrawMargin(Map, offset):
	i_max = len(Map)
	j_max = len(Map[0])
	
	interval = 5
	
	i = 0
	for j in range(offset, j_max):
		if j % interval != 0:
			Map[i][j] = 'W'
		else:
			Map[i][j] = 'E'	
		i += 1
	
	for k in range(1, offset+1):
		i = 0
		for j in range(offset-k, j_max):
			Map[i][j] = 'w'
			i += 1	
	
	for k in range(1, offset+1):
		j = 0
		for i in range(offset-k, i_max):
			Map[i][j] = 'w'
			j += 1
		
	j = 0
	for i in range(offset, i_max):
		if i % interval != 0:
			Map[i][j] = 'W'
		else:
			Map[i][j] = 'E'
		j += 1	
	

rows = 75
cols = 75

Map = [['.' for i in range(cols)] for j in range(rows)]


#Print Rows and cols:
print "ROWS: ", rows, " "
print "COLS: ", cols, " "
#Draw Fight Margin
print "WAR_BORDER_OFFSET: ", rows/6, " "
DrawMargin(Map, rows/6)

# Draw Territories
Coord_list = DrawCircle(-1, 0, rows/8, UPPERQUAD, Map, 'A')
print "TEAM_A_SPAWN_BORDER: ", len(Coord_list), print_Coord_list(Coord_list)
Fill(Map, rows-1, 0, 'A')

Coord_list = DrawCircle(0, 1, rows/8, LOWERQUAD, Map, 'B')
print "TEAM_B_SPAWN_BORDER: ", len(Coord_list), print_Coord_list(Coord_list)
Fill(Map, 0, cols-1, 'B')

Coord_list = DrawCircle(cols/2-cols/6, rows/2+rows/6, int(rows*0.065), FULL, Map, 'a')
print "TEAM_A_TEMPLE_BORDER: ", len(Coord_list), print_Coord_list(Coord_list)

Coord_list = DrawCircle(cols/2+cols/6, rows/2-rows/6, int(rows*0.065), FULL, Map, 'b')
print "TEAM_B_TEMPLE_BORDER: ", len(Coord_list), print_Coord_list(Coord_list)

Fill(Map, rows/2-rows/6, cols/2-cols/6, 'a')
print "A_TEAM_TEMPLE_CENTER: ", str(rows/2-rows/6), str(cols/2-cols/6), " "
Fill(Map, rows/2+rows/6, cols/2+cols/6, 'b')
print "B_TEAM_TEMPLE_CENTER: ", str(rows/2+rows/6), str(cols/2+cols/6), " "

# Draw Jungle
L, U = GenCoordList(rows*cols*0.025, Map)
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
