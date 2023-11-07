# Example using pellet dispenser

This notebook contains an example of how to run the code for using the multimaterial dispenser. Most cells can be run without any modification if the dispenser has been built without design modifications. The lines that need modifications are marked like this:

`###############################################################################`


```python
# Import Libraries
import serial
import numpy as np
import time
import serial.tools.list_ports
import os
import sys
sys.path.append('src')
from DispenserSetup import SetupDispenser, Compositions
```

In this cell, we define the port name for the arduino and the balance. We do it by listing the connected ports and selecting the ones whose device serial is the corresponding to our device. You should find the serial of your devices and change it accordingly. You can do it by inspecting the devices detected by serial.tools.list_ports.comports(). In case of doubt, try disconnecting and connecting to see which ports are affected.


```python
ports = serial.tools.list_ports.comports()

###############################################################################

arduino_serial = "write_here_your_arduino_serial"
balance_serial = "write_here_your_balance_serial"

###############################################################################

for port in ports:
    if port.serial_number == arduino_serial:
        arduino_port = port.device
    elif port.serial_number == balance_serial:
        balance_port = port.device
        
### Serial initialization
## Arduino
arduino = serial.Serial(arduino_port, 9600)
## Balance
balance = serial.Serial(port=balance_port, baudrate=9600, bytesize=serial.SEVENBITS,
                       parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE)
```

In the next cell, we use the Arduino Command Line Interface (arduino-cli) to compile and upload the arduino script. If you prefer to do it using the Graphical User Interface, skip this cell. For using it, update the location of your arduino-cli


```python
###############################################################################################################

os.system("PATH/arduino-cli compile --fqbn arduino:avr:mega 8noIRnew_NOshaketimer_new/8noIRnew_NOshaketimer_new.ino")
os.system("PATH/arduino-cli upload -p "+arduino_port+" --fqbn arduino:avr:mega 8noIRnew_NOshaketimer_new/8noIRnew_NOshaketimer_new.ino")

###############################################################################################################
```

The next step is to define the materials and their concentrations in each of the dispensers. For that we will use the `SetupDispenser` function from `/src/DispenserSetup`. It will require a `.csv` file detailing the compositions, that can be built following the example available in the help of the function:


```python
help(SetupDispenser)
```


```python
###############################################################################
D, M, A = SetupDispenser("data/dispenser_setup.csv", "PLA")
###############################################################################
```

Next, we read the file that contains the compositions using the Compositions function. It's working is detailed in the documentation:


```python
help(Compositions)
```


```python
###############################################################################
compositions = Compositions("./data/compositions_to_dispense.csv", D, M, A, "PLA")
###############################################################################
print(compositions)
```

For the rest of the notebook we will use only the first of the compositions available in the .csv file. in your application you can easily loop over all of them, just remember to change the cup in the balance between compositions.


```python
C = compositions[0]
```

It is convenient now to establish a ratio between materials: in this way, instead of dispensing all the ammount of one material and then of another, we can alternate small quantities and thus have a better mixing. We do this by selecting the one with lower ammount and expressing the others as a ratio with respect to this one. In every step we will dispense 1 g of the smaller one and the corresponding grams of the rest.


```python
tol = 1e-3
minC = np.min([c for c in C if c>tol])
ratios = C/minC 
print('The ratios for pellet dispensing will be: ', ratios)
```

The next cell stablish commands to tare, calibrate and measure weight from the balance. You may need to edit them if your balance uses a different protocol.


```python
###############################################################################
def tare(balance):
    # Set the balance measurement to 0.0 g
    try:
        balance.write(b'T\r\n')
    except:
        balance = serial.Serial(port=balance_port, baudrate=9600, bytesize=serial.SEVENBITS,
                       parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE)
        balance.write(b'T\r\n')


def calib(balance):
    # Calibrate the balance automatically
    try:
        balance.write(b'C\r\n')
    except:
        balance = serial.Serial(port=balance_port, baudrate=9600, bytesize=serial.SEVENBITS,
                       parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE)
        balance.write(b'C\r\n')

def measure(balance):
    try:
        trash = balance.read_all()
        balance.write(b'B\r\n')
        reading = balance.readline()
        weight = float(reading[:10])
    except:
        balance = serial.Serial(port=balance_port, baudrate=9600, bytesize=serial.SEVENBITS,
                       parity=serial.PARITY_EVEN, stopbits=serial.STOPBITS_ONE)
        trash = balance.read_all()
        balance.write(b'B\r\n')
        reading = balance.readline()
        weight = float(reading[:10])
    return weight
###############################################################################
```

Select the total ammount of material that you want to dispense


```python
###############################################################################
mass_target = 100 # g
###############################################################################
```

Now, we define some variables and arrays that we will need in the dispensing loop


```python
measureAll = True
massAll = 0
mass = 0
masses = [[0] for _ in range(8)]
i = 0
times = [[0] for _ in range(8)]


dispenser_switch = {0 : b'0', 1 : b'1', 2 : b'2', 3 : b'3', 
                    4 : b'4', 5 : b'5', 6 : b'6', 7 : b'7', 'stop' : b'8'}
```

The next cell runs the dispensers until the desired quantity is reached. As pointed above, it will alternate dispensers keeping the ratios between materials.


```python
start = time.time()

while measureAll == True:
    for j in range(len(C)):
        if C[j] > tol:
            if masses[j][i] < ratios[j]*(i+1):
                measureNow = True
                tare(balance)
                time.sleep(2)
                arduino.write(dispenser_switch[j])
                while measureNow == True:
                    mass = measure(balance)
                    if mass + masses[j][i] >= ratios[j]*(i+1):
                        arduino.write(dispenser_switch['stop'])
                        time.sleep(4)
                        mass = measure(balance)
                        measureNow = False
                massAll = massAll + mass
                masses[j].append(masses[j][i]+mass)
                times[j].append(time.time()-start)
                time.sleep(0.1)
            elif masses[j][i] >= ratios[j]*(i+1):
                arduino.write(dispenser_switch['stop'])
            print("Dispenser ", j, "| iteration: ", i+1, "| t = ", times[j][i+1], "s | ", "mass : ", masses[j][i+1])                  
    if massAll > mass_target:
        measureAll = False
        arduino.write(dispenser_switch['stop'])
    i = i + 1
    
arduino.write(dispenser_switch['stop'])
```

We can now plot the dispensing progress, comparing the amount that was actually dispensed and the one asked by the code


```python
import matplotlib.pyplot as plt

colors = ["C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7"] 

for key in D.keys():
    plt.plot(times[D[key]-1], masses[D[key]-1],label=key, c=colors[D[key]-1])
    plt.plot(times[D[key]-1], np.arange(len(times[D[key]-1]))*ratios[D[key]-1], '--',c=colors[D[key]-1])
plt.plot(0,0,'--',c = 'gray',label = "Expected amounts")
plt.title("Progression of Sample")
plt.xlabel("Time [s]")
plt.ylabel("Mass [g]")
plt.legend()
plt.show()
```

Finally, we present a measure for accuracy consistent of the total error commited divided by the total amount dispensed:


```python
final_masses = [mass[-1] for mass in masses[:]]
accuracy = (1-np.sum(abs(final_masses-C*mass_target))/np.sum(final_masses))*100
print("Accuracy: ", accuracy, "%")
```


```python

```
