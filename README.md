# AIFRED
**Written by Chaidhat Chaimongkol, started on May 5, 2019**\
\
**Artificial Intelligence Facial Recognition, Emotion and Detection** or just a cool acronym.\
Written in 🌊++ and currently attempts to detects and recognises faces.\

# Please visit [the wiki](www.google.com) for my documentation & usage

## Facial Detection   🌲==👱🏼‍♀️?

Currently almost finished.\
Uses [Viola-Jones' paper](https://www.cs.cmu.edu/~efros/courses/LBMV07/Papers/viola-cvpr-01.pdf) as the main concept. Can create Haar Features for accurate object detection.

## Facial Recognition & Verification   🧔🏽== 👱🏼‍♀️?

Currently being worked on.\
Uses [Facebook's Deepface paper](https://www.cs.toronto.edu/~ranzato/publications/taigman_cvpr14.pdf) as inspiration.\
2D fiducial landmarking points given by [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace) (required external software).\
Facial alignment & representation & verification is done with my software using GLSL programming to take advantage of GPU processing.\
Uses Deep Learning and Convolutional Neural Networks for representation.

## Facial Identification 🧔🏽== {👩🏻,🧔🏽,👱🏼‍♀️}?

Currently not finished.\
Identification is done by repeatedly verifying faces.\
\
My program's workflow: detect, align, ~~represent, classify~~, verification, identification.\
(strikethrough was Facebook's Deepface method, for higher accuracy although more difficult to implement)

## Facial Emotion Recognition

No plans to be implemented
\

**libraries & software required -**
* GLFW
* GLEW
* libPNG
* [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace)
