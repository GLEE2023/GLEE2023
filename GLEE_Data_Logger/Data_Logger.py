# GLEE Data Logger is a python script which produces a csv file containing a given number of samples. Built for generalized recording of LunaSat samples.
import serial
import datetime

# Logger Configuration
port = "COM4"       # Will change with respect to usb port, should match port used during uploading or indicated by arduino serial monitor
baud = 9600         # Default LunaSat Samplerate = 9600
nSamples = 60       # Default of 60 samples @ 1Hz for 1min of datapoints 

# Initialize Serial Port
s = serial.Serial(port, baud)
print("Began Serial Communications with Port: " + port)

# Initialize Log File

# Construct File name 
baseFileName = "GLEE_Data_Logger\GLEE_Data.csv"             # TODO: Allow for configuration
suffix = datetime.datetime.now().strftime("%y%m%d_%H%M%S")  # Create timestamp patern string
fileName = "_".join([baseFileName, suffix])                 # Joins base file name and date time suffix into a single string representing file name

# Open File
file = open(fileName, "w")                  # Writes new file each time
print("Created New Log File: " + fileName)

# Main Line Reading Loop
print("Beginning Sampling: ")
for i in range(nSamples):
    
    # Read and filter data
    rawData=str(s.readline())
    data = rawData[2:][:-6]
    
    # Write and display data
    print(data)
    file.write(data + "\n")

file.close()
