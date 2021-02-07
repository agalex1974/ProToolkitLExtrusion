# ProToolkitLExtrusion

My approach is OOP for the C API of Pro/Toolkit. 
In companies I learned to code in an OOP fashion and left C programming only if I have no other choise. 
In order for the example to work for you plese: 

Please update the protk.dat according to the path you will put the project.

Please set the environmental variables for the pro/toolkit according to your Paths in order your system to find the libraries and includes of 
Pro/Toolkit. 
Please set the build type as x64 Release in your visual studio 2017 and newer. It is configured just for this type. 
Please start a Part session before starting the Application as it hooks to the current model. You can change this behavior if you want to by altering the code. 

The rights for the L shape definition as a figure belong to: 
https://camo.ici.ro/journal/vol13/v13a7.pdf (figure 4) 
In this project I saw the figure and used Creo's very analytic examples as reference to create quite a cute interface.

Please change: 
errlog_fp = fopen("C:\\Users\\agalex\\Desktop\\Coding\\DPSDParametricModel\\x64\\Release\\ParametricModel.log", "w"); 
to a valid path otherwise Creo will crash. :-)
