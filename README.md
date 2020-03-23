# ReDynELA
This repository contains the source code of the solver of DynELA v.1.0 parallel version published in 2005 for the following paper:

Olivier Pantalé, Parallelization of an object-oriented FEM dynamics code: influence of the strategies on the Speedup. (2005) Advances in Engineering Software, 36 (6). 361-373.

Official URL: http://dx.doi.org/10.1016/j.advengsoft.2005.01.003

Open access version: https://oatao.univ-toulouse.fr/5378/

**This is an experimental version only for testing. A more friendly version exists and is available in**: https://github.com/pantale/DynELA

### Prerequisites:

Compilation of the FEM code requires a number of libraries.

Genaration of Makefiles for DynELA compilation is based on the use of the CMake application. CMAke is a cross-platform, open-source build system generator. It can be installed with the following command:

	sudo apt install cmake

DynELA is written in C++ therefore it needs a C++ compiler and some libraries. Under Ubuntu this can be installed with the following command:

	sudo apt install bison flex-old

This sould also install the bison parser and the flex interpreter, (DynELA requires the old-flex interpreter 2.5.4, called flex-old).

### Download and compilation procedure:

Download and compilation is done using the following procedure:

	git clone https://github.com/pantale/ReDynELA.git
	cd DynELA
	mkdir Build
	cd Build
	cmake ../Sources
	make
	
## Testing and usage:

Running one of the provided samples in Samples floder:

	../../Build/bin/DynELA_solve sample_name_without_extension
	
There is no post-processor to visualize the results, but the code must run, and provide advancing informations. It il also possible to control the number of CPUs used by setting the following ENV variables

	OMP_NUM_THREADS = number_of_requested_cores
	OMP_DYNAMIC = FALSE
	computeInternalForces =  [1 to 8]

- OMP_NUM_THREADS : sets the requested number of cores
- OMP_DYNAMIC : to disable automatic load balancing from the OS [FALSE]
- computeInternalForces : test the various parallelization methods [1 to 8]

And compare the running times available in the file: CPU-TIMES-2.txt
The balancing of the elements in files : proc_*

All results are available, but unfortunately all those results are in binary *.bin files, and you will need the post processor to see it. A compiled version of it is in directory PostProcessor, you can use it, but this requires to have the same libraries as mime. I'm on an Ubuntu 18.04.4 LTS version.

	../../PostProcessor/DynELA_post sample_name_without_extension
	
***
Olivier Pantalé  
Full Professor of Mechanics  
email : olivier.pantale@enit.fr

Laboratoire Génie de Production  
Ecole Nationale d'Ingénieurs de Tarbes  
Université de Toulouse  
47 Avenue d'Azereix - BP 1629  
65016 TARBES - CEDEX - FRANCE