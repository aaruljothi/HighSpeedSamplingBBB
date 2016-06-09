import sys
import math

def tobin():
	result = []
	fnametxt = input('Enter Read File Name: ') + '.txt'
	print('Starting file converion')
	with open(fnametxt) as f:
		for line in f:
			t = ''
			t = t+ ((32-len("{0:b}".format(int(line))))*'0') + "{0:b}".format(int(line)) + '\n'
			result.append(t)

	#writeFile.close()
	f.close()
	print('Binary Conversion finished')
	return result
	#print('Error could not open file!!!!!')

def binrip(inpute = []):
	result = []
	temp = []
	temp2 = ''
	print('Riping Binary Char')
	count = 0
	for line in inpute:
		if count % 2 != 0:
			i = [line[31-16], line[31-17], line[31-12], line[31-15], line[31-28]]
			#result.append(temp[0] + temp[1] + i[0] + temp[2] + temp[3] + temp[4] + i[1] + temp[5] + temp[6] + temp[7] + i[2] + temp[8] + i[3] + i[4] + temp[9] + temp[10])
			result.append(temp[10] + temp[9] + i[4] + i[3] + temp[8] + i[2] + temp[7] + temp[6] + temp[5] + i[1] + temp[4] + temp[3] +  temp[2] + i[0] + temp[1] + temp[0])
		else:
			temp = [line[31-30], line[31-31], line[31-5], line[31-26], line[31-3], line[31-12], line[31-7], line[31-13], line[31-20], line[31-4], line[31-27]]
		count = count + 1
	return result




def bintoint(inpute = []):
	result = []
	for e in inpute:
		result.append(str(int(e, 2)))
	return result

def writeit(inpute =[]):
	print('Writing out')
	writeFile = input('Enter write file name: ') + '.txt'
	#try:
	out = open(writeFile, 'w')
	for e in inpute:
		out.write(e + '\n')
	return 'Conversion Complete'
	#except:
	#	return 'failed'

def fixnums(inpute = []):
	# set to round up
	print("Maping values")
	one_volt = 48463
	zero_volt = 31776
	one_to_zero = one_volt - zero_volt
	result  = [] 
	for e in inpute:
		result.append(str((float(e) - zero_volt)/one_to_zero))
	return result





print(writeit(fixnums(bintoint(binrip(tobin())))))

