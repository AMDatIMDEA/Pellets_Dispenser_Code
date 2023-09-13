import numpy as np
import pandas as pd
from collections import Counter

tol = 1e-3

def SetupDispenser(dispenser_setup_file, Polymer):
    """
    Funtion to read the setup file and create structures to define what material each dispenser has.
    
    Inputs:
    
        dispenser_setup_file: full route of a .csv file with the different materials and how they are arranged. Example:
            Dispenser, PLA, GNP, CLO, BN
            1, 0.0, 0.0, 0.0, 0.0 # Empty
            2, 0.0, 0.0, 0.0, 0.0 # Empty
            3, 0.0, 0.0, 0.0, 0.0 # Empty
            4, 1.0, 0.0, 0.0, 0.0 # Contains pure polymer
            5, 0.9, 0.1, 0.0, 0.0 # Polymer with 10% GNP
            6, 0.9, 0.0, 0.0, 0.1 # Polymer with 10% BN
            7, 0.0, 0.0, 0.0, 0.0 # Empty
            8, 0.0, 0.0, 0.0, 0.0 # Empty
            
            Notes:
            - Only one polymer matrix allowed.
            - All of the non-empty dispensers must contain the polymer matrix and at most one additive.
            - The composition for each dispenser has to sum 1.
            
        Polymer: A string with the name of the polymer matrix, written exactly like in the .csv file. e.g.: "PLA"
        
    Outputs:
        
        dispensers_map : A dict with the active dispensers and their main component 
        
        M: array with the fraction of polymer matrix in each dispenser
        
        A: array with the fraction of additive in each dispenser
        
    
    """
    
    setup = pd.read_csv(dispenser_setup_file, comment='#')
    n_dispensers = len(setup)
    relevant_component = dict()
    matrix = dict()
    additive = dict()
    
    for index, row in setup.iterrows():
        non_zero_elements = [(col, value) for col, value in row.items() if (value != 0 and col != "Dispenser")]
        if non_zero_elements:
            dispenser_string = "Dispenser " + str(index+1) + ": "
            for element in non_zero_elements:
                dispenser_string += str(element[1]*100) + "% of " + element[0] + ", "
                if element[0] == Polymer:
                    matrix[index+1] = element[1]
                else:
                    additive[index+1] = element[1]
            if len(non_zero_elements) == 1:
                if non_zero_elements[0][0] == Polymer:
                    relevant_component[index+1] = non_zero_elements[0][0]
                else:
                    print("Error, polymer in dispenser ", index, " not specified")
                    input("Press Enter to continue")

            elif len(non_zero_elements) == 2:
                relevant_component[index+1] = [element[0] for element in non_zero_elements if element[0] != Polymer][0]
            else:
                print("The material in dispenser ", index, " contains more than 2 additives")
                input("Press Enter to continue")
            print(dispenser_string)

            total = np.sum([element[1] for element in non_zero_elements])
            if np.abs(total-1) > tol:
                print("Error in composition in dispenser ", index, ", check if the setup file is correct")
                input("Press Enter to continue")
                
    value_counts = Counter(relevant_component.values())
    repeated_values = [value for value, count in value_counts.items() if count > 1]
    if repeated_values:
        print("There are repeated values in your setup, we recommend you to bridge those dispensers together for increased flow. Otherwise, only one of them will be used")
        input("Press Enter to continue")
    
    dispensers_map =  {value: key for key, value in relevant_component.items()}
    
    M = np.zeros(n_dispensers)
    A = np.zeros(n_dispensers)

    for i in range(n_dispensers):
        if i+1 in matrix.keys():
            M[i] = matrix[i+1]
        if i+1 in additive.keys():
            A[i] = additive[i+1]
            
    return dispensers_map, M, A