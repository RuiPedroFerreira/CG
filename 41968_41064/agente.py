"""
agente.py

criar aqui as funções que respondem às perguntas
e quaisquer outras que achem necessário criar

colocar aqui os nomes e número de aluno:
41968, Carlos Martins
41064, Rui Ferreira

"""
import time

#-------------------------------------------------------------------------------------------------------

# Variaveis
list_people = ['', '']

battery_remaining = 0

battery_aux = 0

start_time = time.time()

times_list = []

room_name = ['']

items_list = []

current_room = -1

previous_item = ''

position_aux = [0, 0]

# Lista com as coordenadas de cada divisao e respetivo numero
room_number = [
					((1, 85, 30, 565, 135)),
					((2, 30, 90, 85, 330)),
					((3, 565, 30, 635, 330)),
					((4, 30, 330, 770, 410)),
					((5, 85, 135, 235, 330)),
					((6, 280, 135, 385, 330)),
					((7, 430, 135, 565, 330)),
					((8, 635, 30, 770, 85)),
					((9, 635, 130, 770, 185)),
					((10, 635, 230, 770, 330)),
					((11, 30, 410, 235, 570)),
					((12, 280, 410, 385, 570)),
					((13, 430, 410, 570, 570)),
					((14, 615, 410, 770, 570))]




# funcao que determina a tempo restante de bateria
def time_remaining(bateria, time_used):
	
	global times_list
	
	#times_list guarda o tempo que demorou os ultimos 3 pontos percentuais
	times_list.append(time_used)
	len_times_list = len(times_list)
	if len_times_list > 3:
		times_list.pop(0)
	
	#media desses mesmos tempos
	average_times = sum(times_list) / len_times_list
	#Multiplica-se essa media com a capacidade restante da bateria
	result = average_times * (101 - bateria + 1)
	
	return result	



#Funcao que verifica se o item ja foi encontrado pelo robo
def find_item(item_name):
	for i in range(len(items_list)):
		if (item_name == items_list[i][2]):
			return True
			
	return False


#Adiciona ha lista items_list todos os objetos que o robo encontra
def add_new_object(objetos):
	global current_room
	
	for item in range(len(objetos)):
		#divide a string para obter a categoria e nome do objeto
		item_cate, item_name = tuple(objetos[item].split("_", 1))
		if find_item(item_name) == False:
			items_list.append((current_room, item_cate, item_name))



#calcula e retorna a posiçao atual do robo usando a lista room_number caso esteja dentro de algum limite previamente definido
def current_position(x,y):
	for room in room_number:
		if x >= room[1] and x<= room[3] and y >= room[2] and y <= room[4]:
			return room[0]
	return -1


#Funcao que define o tipo de divisao onde o robo se encontra
def room_type(room):
	
	list_aux = []
	if room in [1, 2, 3 ,4]:
		return "Corredor"
	
	#Caso o item esteja na divisao pretendida adiciona-se a list_aux
	for i in items_list:
		if i[0] == room:
			list_aux.append(i)
		
	mesa = 0
	cadeira = 0
	for i in list_aux:
		#caso haja uma cama na lista de itens dessa divisao
		if i[1] == "cama":
			list_aux.clear()
			return "Quarto"
		if i[1] == "mesa":
			mesa += 1
		if i[1] == "cadeira":
			cadeira += 1
	
	#caso exista no minimo uma cadeira e uma mesa
	if mesa >= 1 and cadeira >= 1:
		list_aux.clear()
		return "Sala Enfermeiro"
	elif cadeira >= 2:
		list_aux.clear()
		return "Sala de espera"
	else:
		#caso nao haja dados suficientes para classificar a divisao
		list_aux.clear()
		return "Tipo de divisao desconhecida"

#---------------------------------------------------------------------


def work(posicao, bateria, objetos):
    
    global battery_remaining
    global battery_aux
    global start_time
    global list_aux
    global current_room
    global previous_item
    global position_aux
    
    
    position_aux[0] = posicao[0]
    position_aux[1] = posicao[1]
    aux_room = current_position(posicao[0], posicao[1])
    if aux_room != current_room and aux_room != -1:
        current_room = aux_room
    
    #Sempre que o robo encontra um objeto diferente chamamos a funcao add_new_object para adicionar a lista de itens conhecidos
    if (objetos != previous_item and objetos != ""):
        add_new_object(objetos)
        previous_item = objetos
    
    #percorremos a lista de objetos para determinar se existe alguma pessoa
    for i in range(len(objetos)):
		#dividimos a lista para obter apenas o tipo de pessoa, caso seja registado varios tipos de objetos ao mesmo tempo
        aux_objetos = objetos[i].split(",")
        #caso a string aux_objetos contenha a alguma destas palavras podemos assumir que se trata de uma pessoa
        if any(s for s in aux_objetos if "enfermeiro" in s or "doente" in s or "medico" in s):
            if list_people[0] != aux_objetos:
                list_people[1] = list_people[0]
                list_people[0] = aux_objetos
    
    #sempre que a percentagem da bateria muda
    if int(battery_aux) != int(bateria):
		#calcular o tempo que o ponto percentual demorou
        time_pass = time.time() - start_time
        #Quantos pontos percentais ja foram gastos
        battery_lost = 100 - int(bateria)
        battery_remaining = time_remaining(battery_lost, time_pass)
        battery_aux = bateria
        #comeca a contar o tempo para o novo ponto percentual
        start_time = time.time()


#---------------------------------------------------------------------
def resp1():
	print(list_people[1])

def resp2():
	global position_aux
	#determina e guarda o tipo de divisao 
	room_name[0] = room_type(current_position(position_aux[0], position_aux[1]))
	print(room_name[0])
	
def resp3():
    pass

def resp4():
    pass

def resp5():
    pass

def resp6():
    print("Faltam %.2fs para a bateria acabar." % battery_remaining)

def resp7():
    room_with_chair = 0
    room_with_chair_and_book = 0
    rooms_visited = 0
    
    #percorre todas as salas consideradas como divisoes
    for room in range(5, 15):
        aux_visited = False
        aux_chair = 0
        aux_book = 0
		
		#percorre lista de items econtrados pelo robo
        for item in items_list:
			#caso a divisao onde o item foi encontrada seja a mesma da divisao pretendida
            if item[0] == room:
                if item[1] == "cadeira":
                    aux_chair += 1
                if item[1] == "livro":
                    aux_book += 1
                aux_visited = True
		
		#caso exista algum item encontrado na divisao pretendida
        if aux_visited == True:
            rooms_visited += 1	
        if aux_chair >= 1:
            room_with_chair += 1
        #caso a divisao em questao tenha pelo menos uma cadeia e um livro
        if aux_chair >= 1 and aux_book >=1:
            room_with_chair_and_book += 1
    
    
    if(rooms_visited == 0 or room_with_chair == 0):
        print("Nenhuma divisao com cadeira encontrada")
    else:
		#probabilidade de haver cadeiras nas divisoes
        pb_chair = room_with_chair / rooms_visited
        #probabilidade de haver cadeiras e livros nas divisoes
        pb_chair_and_book = room_with_chair_and_book / rooms_visited
        res = pb_chair_and_book / pb_chair
        print("A probabilidade é %.2f." % res)
	
	
def resp8():
    room_with_enf = 0
    room_with_enf_and_doente = 0
    rooms_visited = 0
    
    for room in range(5, 15):
        aux_visited = False
        aux_enf = 0
        aux_doente = 0
		
		#percorre lista de items econtrados pelo robo
        for item in items_list:
			#caso a divisao onde o item foi encontrada seja a mesma da divisao pretendida
            if item[0] == room:
                if item[1] == "enfermeiro":
                    aux_enf += 1
                if item[1] == "doente":
                    aux_doente += 1
                aux_visited = True
		
        if aux_visited == True:
            rooms_visited += 1	
        if aux_enf >= 1:
            room_with_enf += 1
        if aux_enf >= 1 and aux_doente >=1:
            room_with_enf_and_doente += 1
    
    if(rooms_visited == 0 or room_with_enf == 0):
        print("Nenhuma divisao com enfermeiro encontrada")
    else:
        pb_enf = room_with_enf / rooms_visited
        pb_enf_and_doente = room_with_enf_and_doente / rooms_visited
        res = pb_enf_and_doente / pb_enf
        print("A probabilidade é %.2f." % res)
