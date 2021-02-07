# ProToolkitLExtrusion

My approach is OOP for the C API of Pro/Toolkit. <br />
In companies I learned to code in an OOP fashion and left C programming only if I have no other choise. <br />
In order for the example to work for you plese: <br />

Please update the protk.dat according to the path you will put the project. <br />

Please set the environmental variables for the pro/toolkit according to your Paths in order your system to find the libraries and includes of 
Pro/Toolkit. <br />
Please set the build type as x64 Release in your visual studio 2017 and newer. It is configured just for this type. <br />
Please start a Part session before starting the Application as it hooks to the current model. You can change this behavior if you want to by altering the code. <br />

The rights for the L shape definition as a figure belong to: <br />
https://camo.ici.ro/journal/vol13/v13a7.pdf (figure 4) <br />
In this project I saw the figure and used Creo's very analytic examples as reference to create quite a cute interface.<br />

Please change: <br />
errlog_fp = fopen("C:\\Users\\agalex\\Desktop\\Coding\\DPSDParametricModel\\x64\\Release\\ParametricModel.log", "w"); <br />
to a valid path otherwise Creo will crash. :-)
