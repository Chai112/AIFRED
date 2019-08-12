# AIFRED
Artificial Intelligence Facial Recognition, Emotion and Detection - or just a cool acronym\
Written in 🌊++ and currently attempts to detects and recognises faces.

## Facial Detection   🌲==👱🏼‍♀️?

Currently almost finished.\
Uses [Viola-Jones' paper](https://www.cs.cmu.edu/~efros/courses/LBMV07/Papers/viola-cvpr-01.pdf) as the main concept. Can create Haar Features for accurate object detection.

## Facial Recognition & Verification   🧔🏽== 👱🏼‍♀️?

Currently being worked on.\
Uses [Facebook's Deepface paper](https://www.cs.toronto.edu/~ranzato/publications/taigman_cvpr14.pdf) as inspiration.
2D fiducial landmarking points given by [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace) (required external software).
Facial alignment & representation & verification is done with my software.\
Uses Deep Learning and Convolutional Neural Networks for representation.

## Facial Identification 👱🏼‍♀️== {👩🏻,🧔🏽,👱🏼‍♀️}?    🔍

Currently not finished.\
Identification is done by repeatedly verifying faces.

libraries & software required -
* GLFW
* GLEW
* libPNG
* [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace)
