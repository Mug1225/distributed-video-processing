# Distributed Video Processing

## Introduction

Distributed Video Processing is a system designed for face anonymization in video content. The system utilizes a parallelized distributed computing framework, where four separate processes run three different algorithms to process each frame. The system leverages Message Passing Interface (MPI) for communication between processes, thereby serializing the flow and enabling individual frame processing. The core anonymization approach involves face recognition, chaotic mapping, and DNA encryption.

## Algorithms

The face anonymization process is driven by three main algorithms:

1. **Face Recognition:** Detects and recognizes faces in the video frames.
2. **Chaotic Mapping:** Applies a chaotic map to the recognized faces to distort them, providing a certain degree of anonymity.
3. **DNA Encryption:** Further enhances the privacy of individuals by encrypting the distorted face data using a key.

## Modules

### The system requires the following modules to function:

1. **OpenCV for C++:** OpenCV (Open Source Computer Vision Library) is an open-source computer vision and machine learning software library that includes several hundreds of computer vision algorithms. The library can be installed by following the instructions provided [here](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html).
2. **mpicxx Compiler:** mpicxx is a MPI compiler for C++ applications, which is required for parallelizing tasks across multiple processors.
3. **CMake:** CMake is an open-source, cross-platform family of tools designed to build, test and package software.

## Usage

To utilize the Distributed Video Processing system, follow these commands in a bash shell:

```bash
 cmake . 
 make
 mpirun -np 4 ./try "{videofilename}.mp4" "{degree-of-face-anonymization 1-10}" "{key-for-encrypting-face}"
```
Where:

- `{videofilename}.mp4` is the filename of the video you wish to process.
- `{degree-of-face-anonymization 1-10}` is the degree to which you want the faces in the video to be anonymized. This value should be between 1 and 10, with 1 indicating minimal anonymization and 10 indicating maximum anonymization.
- `{key-for-encrypting-face}` is the key that will be used for the DNA encryption process.

Please ensure that all modules and dependencies have been properly installed before running these commands.

## Support

If you encounter any issues or have questions, please visit the [Issues](https://github.com/Mug1225/distributed-video-processing/issues) tab in the repository. We are committed to providing support and addressing any concerns promptly.

## Contributions

The following contributors made Distributed Video Processing a collaborative project possible:

- [Mugesh](https://github.com/Mug1225)
- [Prasanna Kumar](https://github.com/prasannakumar227)
- [Nithik SR](https://github.com/Nithik-09)

We express our gratitude to all the contributors who have dedicated their time and expertise to improve Distributed Video Processing.

We also welcome contributions from everyone! If you find any issues or have suggestions for improvement, please fork the repository and submit a pull request. 🚀

Thank you for your interest in our Distributed Video Processing!

### With Love ❤️ from the Distributed Video Processing Team.

--------------------------------------------------------
