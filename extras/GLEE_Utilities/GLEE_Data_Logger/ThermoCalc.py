M = 172
PTAT25 = 13498
U0 = 64617
UOut1 = 70200
TObj1 = 40
K = ((UOut1 - U0) / ((pow((TObj1 + 273.15), 3.8)) - pow(25.0 + 273.15, 3.8)))


#Using readlines()
file1 = open("/Users/akhiltadiparthi/Documents/Arduino/libraries/GLEE2023/extras/GLEE_Utilities/sampleThermoData.txt", "r")
Lines = file1.readlines()
  
count = 0
# Strips the newline character
for line in Lines:
    count += 1
    #amb temp
    if (count % 2) != 0:
        TPamb = int(line.strip())
        Tamb = (298.15 + (TPamb - PTAT25) * (1.0/M)) 
        FinalTamb = Tamb - 273.15
        print("Ambient temp: " + str(FinalTamb))
    #obj temp
    else:
        TPobj = int(line.strip())
        f1 = pow(Tamb, 3.8)
        f2 = ((TPobj) - (U0)) / K
        Tobj = pow((f1 + f2), 0.2631578947) - 273.15
        print("Object temp: " + str(Tobj))