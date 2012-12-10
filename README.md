ALOE++ 
===========
Hola Vuk!
ALOE stands for Abstraction Layer and Open Operating Environment. It is an Open Source framework for distributed real-time signal processing for SDR (Software-Defined Radio) applications. ALOE is released under the LGPL license (see license.txt) 

The project is partially founded by the NLnet foundation (http://www.nlnet.nl), as part of the OSLD project. OSLD aims at building an Open Source LTE system based on ALOE. More information and documentation can be found in the OSLD project website (https://sites.google.com/site/osldproject/)

This is the first release of ALOE++ version 0.2. The old ALOE versions are still active in http://flexnets.upc.edu.

Requirements
=============

To install ALOE++, the only requirement is the libconfig parsing library and cmake:
 * libconfig 1.4.8
 * cmake

The current ALOE++ release comes with an OFDM demo waveform and a small set of useful modues. These modules have more requirements: 
 * libfftw3 is used by the gen_dft module 
 * plplot + output driver is used by the plp_sink module to display signals.

To install all requirements in ubuntu, just type:

sudo apt-get install libconfig-dev libfftw3-dev libplplot-dev plplot11-driver-xwin plplot11-driver-cairo

The last two packages are optional. PLplot can work with many different output drivers. The xwin driver works just fine and is fast. The driver used by plp_sink can be selected from the file modrep_ofdm/plp_sink/src/plp_sink.h (see the Documentation Section)
  
Install
=========

 * git clone git://github.com/flexnets/aloe.git
 * cd aloe
 * mkdir build
 * cd build
 * cmake ../
 * make
 * sudo make install

Running the OFDM demo waveform
===============================

The waveform modules is described in the file ofdm.app, in the root aloe source folder. You may take a look at it to see how simply waveforms are created and parametrized in ALOE. 

To run the demo, type:

runcf ts_len nof_cores path_to_ofdm.app

for instance:

runcf 50000 1 ./ofdm.app

if you are running "runcf" from the root aloe source folder and want to run the waveform with a periodicity of 50000 microseconds. If you look at ofdm.app, you will see that the source generates 308 bits each invocation period. Therefore, with this command you will be generating a signal that encodes 6.16 kbps. Such a low throughput is required to plot the signal using the plp_sink module. 

You may increase the throughput by editing the ofdm.app file, module plp_sink and setting parameter "mode" to 0. For instance, type: 

runcf 500 1 ./ofdm.app

to run the waveform with a granularity of 0.5 ms, that is a throughput of 616 kbps and an end-to-end latency of 4 ms.

If you have a multi-core system you may experience with the nof_cores parameter. This parameter allows to formats:
  * Number of cores, e.g. 2 or 4 will use all cores 0..n for allocating tasks
  * Comma-separated values, e.g. 1,3,5, for instance will use only cores 1, 3 and 5 for allocating tasks. 

Documentation 
===============

Doxygen-generated documentation is available in the following links:
 * RTDAL API: http://flexnets.github.com/aloe/rtdal/html/index.html
 * OESR API: http://flexnets.github.com/aloe/oesr/html/index.html
 * OESR Manager API: http://flexnets.github.com/aloe/oesr_man/html/index.html
 * Default Modules: http://flexnets.github.com/aloe/modrep_default/html/index.html
 * OFDM Modules: http://flexnets.github.com/aloe/modrep_ofdm/html/index.html


Brief Project Description 
==================

The ALOE framework entails two sub-projects:
 * Real-Time Distributed Abstraction Layer (RTDAL)
 * Operating Environment for Software-defined Radio (OESR)

RTDAL facilitates real-time synchronous execution of tasks in a distributed environment. Tasks are executed periodically on each processor in a pipeline fashion. Each processor creates one thread per core, which runs each task (dynamically loaded as a shared library) one after another. The threads period on each core of each processor are continuously synchronized, offering the user an abstracted virtual platform. It is also possible to synchronize the task execution with a digital converter (AD/DA) for coherent transmission and processing of samples. 

Besides, the RTDAL API also provides other functions to abstract the specific platform-dependant characteristics:
 * Low-priority tasks: Synchronous or asynchronous low-priority tasks can be created and managed.
 * Interfaces: Two tasks sharing a common interface can communicate between each other. Interfaces can be external or internal, to communicate tasks in remote or local processors, respectively. Internal interfaces support zero-copy mode where only a pointer is transfered between the writer and reader, minimizing memory bandwidth consumption. The current implementation employs a wait-free SPSC bounded queue for best real-time performance.   
 * AD/DA abstraction, time functions, shared memory, file I/O, etc.

The OESR, on the other hand, is built on top of the RTDAL. This allows future portability to other platforms (currently, RTDAL employs the POSIX interface and gcc atomic functions). OESR provides functionalities specifically tailored for SDR applications:
 * Automatic mapping of waveforms to a set of processing cores (distributed or in a multi-core, or both). 
 * Location-transparent inter-module communications.
 * Global variables and parameters configuration/visualization
 * Logs, counters and others.



Related Projects
=================
 * OSLD (https://sites.google.com/site/osldproject/)
 * FlexNets (http://flexnets.upc.edu)
 * GNU Radio (http://gnuradio.org)
 * OSSIE from Wireless@VT (http://ossie.wireless.vt.edu/)


Contact
========
ismael.gomez@tsc.upc.edu





[![githalytics.com alpha](https://cruel-carlota.pagodabox.com/fbe47a2652453cdc1eb50219b38ab2f0 "githalytics.com")](http://githalytics.com/flexnets/aloe)
