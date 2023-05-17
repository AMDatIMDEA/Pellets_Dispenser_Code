#!/usr/bin/env python
# coding: utf-8

# In[2]:


import serial
import matplotlib.pyplot as plt
import time


# In[3]:


try :
    arduino=serial.Serial("COM3", 9600) # Serial port
except:
    print("Vérifier le port série") # Verification message


# In[4]:


# Initializing variables
rawdata=[]
compt=0
weights = []
limit=14


# In[5]:


# Creation of a function who send the speed of the motor selected to the Arduino Board
def setMotorSpeed(motor, speed, nb):
    # Set the speed of a certain motor.
    # motor: a string with the letter of the selected motor.
    # speed: an int between 0 and 640
    # nb: an int representing the number of pellets t obe sent
    message2 = motor + str(speed).zfill(3) + str(nb).zfill(4)
    arduino.write(message2.encode("utf-8"))


# In[9]:


setMotorSpeed('a',150,20)
debut_prise_poids = time.time()


# In[7]:


setMotorSpeed('b',300,50)


# In[ ]:


while compt < 1000:
   data_read = arduino.readline().decode()[:-2]  # Recovery of time when the sensor is broken
   rawdata.append(data_read)  # add items to the end of rawdata list
   if int(rawdata[-1]) < limit:  # if the time is under the limit, it means than 1 pellets go in the hole
       compt += 1
   if int(rawdata[-1]) >= limit:  # if the time is above the limit, it means than 2 pellets go in the hole
       compt += 2
   print(compt, rawdata[-1], sep=" - ")  # Display of time and number of pellets detected at that time


# In[ ]:


setMotorSpeed('a',0) 


# In[ ]:


time.sleep (3) # Wait 3 seconds


# In[ ]:


# Creating the time plot
rawdata.sort()
plt.plot(rawdata)


# In[ ]:


temps_prise_poids = time.time() - debut_prise_poids
print("Temps de prise de poids total : ", temps_prise_poids, "secondes")


# In[ ]:





# In[ ]:




