

for i in range(1, 100):
	if(len(str(i)) % 2 == 0):
		print(str(i) + "  " + str(i)[0:int(len(str(i))/2)] + " " + str(i)[int(len(str(i))/2): len(str(i))])
	else:
		print(i * 2024)