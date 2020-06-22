
# LPROG 1

## Description

Final project for the *"Programing Laboratory 1"* course in the first semester *Information Engineering* program of ESTG *(Superior School of Technology and Management)*.
Developed during the course of about one month in the C programming language.

## Goal

The goal of the project was to create a system capable of sending orders to clients.

The system is able to register new users and one administrator; store and retrieve state from a file; create new orders and price them according to a table of prices defined by the administrator; print recites to screen and file; preform a fuzzy search on the orders that have been placed and a bunch more.

The code is heavily generalized using C macros and well documented using doxygen, although it is written in Portuguese.

## Build the Project

The easiest way to build the project is using a UNIX-based operating system. You will need *make* and *Cmake* installed.

Open a terminal and type:

```bash
make build_release
```

Then you will find the projects executable in `bin/main.x86` *(notice that this might be a 64-bit executable)*.
