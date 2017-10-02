import visa

rm=visa.ResourceManager()
# print(rm.list_resources() #list including GPIB port
# my_instrument = rm.open_resource('GPIB0::15::INSTR')
# print(my_instrument.query('*IDN?'))
ser5=rm.open_resource('COM6')
ser5.timeout=5000

str=ser5.query_ascii_values('CURV?')
#print(str)
print((ser5.read_raw()))
