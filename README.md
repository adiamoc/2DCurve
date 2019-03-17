[TOC]

# About Me

Name: Chen Ma

E-mail: chema@ucdavis.edu

# 2D Curve Drawing System

## Overview

This project is a 2D drawing system that is able to draw Bézier and B-spline curves, and should also be able to open and save files.

## Specifications

* User should be able to specify:
  * the name of the input file,
  * all parapmeters in the Bézier and B-Spline curves' equation.

## Preface

First of all, wish you have a good time playing with it. I added a lot of my own ideas into this project and devoted a lot of time on it. I made my own decisions on the layout and user interface and tried my best to provide the user with best experience; if you have any suggestions on improving the user experience, please do not hesitate to e-mail me.

## Manual

### Running the program

1. Open terminal and open the folder
2. Type `make` to compile the code and type `./project4 filename` to run, or just type `./project4` if you don't want to open any file or you don't want to save drawing as a specific filename.
   * filename
     * the file you want to open (if the file exists)
     * the filename you want to use to save (if the file doesn't exist)
     * e.g.: type `./project4 apple.txt`, if you want to save the drawing as `apple.txt` or if you want to open the file `apple.txt`
   * In case of compile error, these is a precompiled file called `project4`

### Extra

* Three windows
  * Color Picker Panel
    * Set current curve color
  * Main Drawing Panel
    * Draw and edit curve
    * Change current curve parameters
    * Change current stroke width
  * Knot Panel
    * Change current knot position
* Different windows have different reshape approach
  * Color Picker Panel is non-resizable 
  * Main Drawing Panel can be freely resized
  * Knot Panel is resized according to the Main Drawing Panel

### Manipulation

*Key-pressing note: you do not need to press shift, and all key is in low cap.*

* Press "b" to start drawing Bézier curve
  * Press again to end drawing
  * <img src="img/Bezier.png" height="30px"> This color is used for the control points and lines of Bézier curves
* Press "p" to start drawing B-spline curve
  * Press again to end drawing
  * <img src="img/B-Spline.png" height="30px"> This color is used for the control points and lines of B-spline curves
* Press "a" to show all control points
  * Press again to hide
* Press "d" to delete control points
  * <img src="img/Delete.png" height="30px"> This color is used for indicating delete points.
* Press "i" to insert control points
  * Click and drag from current existing control points to add
  * <img src="img/Insert.png" height="30px"> This color is used for indicating insert points.
* Click on the curve's control point to select the curve
* Press "↑", "↓" to change the resolution of selected curve
* Press "←", "→" to change the k value of selected B-spline curve (default k = 3)
* Press "[", "]" to change stroke width of selected curve.
* Click on the Color Picker to set color for selected curve.
* Drag on the knot bar to change the knots of current B-spline curve
* Press "s" to save the drawing
  * If no filename is specified in the beginning, then it will save as `curves.txt`

### Quit

* Press "cmd+q" to quit
* Give me 100 points (more is better)
