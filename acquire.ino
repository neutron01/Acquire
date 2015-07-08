// Acquire.pde
// modification of Blocking.pde, AccelStepper example
// added sleep mode between transfers
// added code for relay firing
// -*- mode: C++ -*-
//
// Shows how to use the blocking call runToNewPosition
// Which sets a new target position and then waits until the stepper has 
// achieved it.
//
// Copyright (C) 2009 Mike McCauley
// $Id: Blocking.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $
// wiring help from Jonahtan Wallick
// modified Raul Ocampo
// moddate 5/28/2014, 6/19/2014, 3/24/2015

/* Circuit
  Motor Driver
  DRV VMOT -> +7V motor power supply
  DRV GND -> 0V motor power supply
  DRV B1 -> red, motor
  DRV B2 -> blue, motor
  DRV A1 -> blk, motor
  DRV A2 -> green, motor
  DRV GND -> Uno Pin 10
  DRV SLEEP -> Uno 5V
  DRV STEP -> Uno pin 12
  DRV DIR ->  Uno pin 11
*/

const int BRD = 1; // pin definitions
const int PINSTEP = 12;
const int PINDIR = 11;
const int PINSLEEP = 10;
const int LOAD = 0; // position definitions
const int REACTOR = 3200;
const int DUMP = 6400;
const int SPEED = 500; // motor movement parameters
const int ACCEL = 500;

/*
  Relay Module   
  RLY Vcc -> +5V relay power supply
  RLY IN1 -> Uno Pin 6
  RLY IN2 -> Uno Pin 7
  RLY GND -> Relay power supply Gnd, Uno Gnd
*/

const int PINREL1 = 6;
const int PINREL2 = 7;

/*
  Acquire trigger
  MyDaq DIO 0 -> Uno pin 8
*/

const int PINTRG = 8;

#include <AccelStepper.h>

/* Motor Driver */

// Define a stepper and the pins it will use
AccelStepper stepper(BRD, PINSTEP, PINDIR);

void setup()
{
  /* Set parameters */
  stepper.setMaxSpeed(SPEED); // setup motor
  stepper.setAcceleration(ACCEL);
  stepper.setEnablePin(PINSLEEP);
  pinMode(PINREL1, OUTPUT); // setup relay module
  digitalWrite(PINREL1, HIGH);
  pinMode(PINREL2, OUTPUT);
  digitalWrite(PINREL2, HIGH);
  pinMode(PINTRG, OUTPUT); // setup Daq trigger
  digitalWrite(PINTRG, LOW);

  /* Move stage and acquire */
  stepper.enableOutputs();
  stepper.runToNewPosition(LOAD); // start at loading position
  stepper.disableOutputs();
  delay(1000);
  stepper.enableOutputs();
  stepper.runToNewPosition(REACTOR); // move stage to reactor tube
  stepper.disableOutputs();
  digitalWrite(PINREL1, LOW); // fire relay 1, sample to reactor
  delay(750);
  digitalWrite(PINREL1, HIGH); // close relay 1
  delay(23250);
  digitalWrite(PINREL2, LOW); // fire relay 2, sample to detector
  digitalWrite(PINTRG, HIGH); // trigger acquisition
  delay(750);
  digitalWrite(PINREL2, HIGH); // close relay 2
  stepper.enableOutputs();
  stepper.runToNewPosition(LOAD); // move stage back to loading position
  stepper.disableOutputs();
  digitalWrite(PINTRG, LOW); // stop trigger
}

void loop()
{
}
