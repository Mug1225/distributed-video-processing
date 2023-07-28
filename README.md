# distributed-video-processing
Face anonymization using Distributed video processing which uses 4 process running three different processes to process each frame and use MPI to pass messages in between to serialize the flow and run individually.
The algorithms include 
- face recognition
- chaotic mapping
- DNA encryption
---------------------------------------------------
### Modules
1. opencv for c++
https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html
2. mpicxx compiler
3. cmake
------------------------------------------------

### usage
 cmake . 
 make
 mpirun -np 4 ./try "{videofilename}.mp4" "{degree-of-face-anonymization 1-10}" "{key-for-encrypting-face}"

--------------------------------------------------------
