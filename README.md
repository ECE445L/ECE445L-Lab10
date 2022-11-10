# Lab 10 DC Motor Control

## Table of Contents

- [Lab 10 DC Motor Control](#lab-10-dc-motor-control)
  - [Table of Contents](#table-of-contents)
  - [0 Repository Structure](#0-repository-structure)
    - [0.1 HW](#01-hw)
    - [0.2 SW](#02-sw)
    - [0.3 Resources](#03-resources)
    - [0.4 Git and Github](#04-git-and-github)
  - [1 Summary](#1-summary)
    - [1.1 Goals](#11-goals)
    - [1.2 Team Size](#12-team-size)
    - [1.3 Review](#13-review)
    - [1.4 Starter Files](#14-starter-files)
    - [1.5 Required Hardware](#15-required-hardware)
    - [1.6 Background](#16-background)
      - [1.6.1 Motor Interface](#161-motor-interface)
      - [1.6.2 Tachometer Interface](#162-tachometer-interface)
      - [1.6.3 Digital Controller Design](#163-digital-controller-design)
      - [1.6.4 System Controller and Output](#164-system-controller-and-output)
    - [1.7 Requirements Document](#17-requirements-document)
  - [2 Preparation](#2-preparation)
  - [3 Procedure](#3-procedure)
    - [3.1 Motor Interface](#31-motor-interface)
    - [3.2 Tachometer Interface](#32-tachometer-interface)
    - [3.3 Digital Controller Design](#33-digital-controller-design)
    - [3.4 Display](#34-display)
  - [4 Checkout](#4-checkout)
  - [5 Report](#5-report)
    - [5.1 Deliverables](#51-deliverables)
    - [5.2 Analysis and Discussion Questions](#52-analysis-and-discussion-questions)
  - [6 Hints](#6-hints)

---

## 0 Repository Structure

The typical explanation for the repo structure. Lab specific instructions can be found further below.

### 0.1 HW

The `hw` folder should contain your schematic and board files for your PCB or circuits. In labs 1-5 and 10, you will be creating schematics for your circuit in EAGLE. A setup tutorial can be found [here](https://www.shawnvictor.net/autodesk-eagle.html).

### 0.2 SW

The `sw` folder should contain your application firmware and software written for the lab. The `sw/inc` folder contains firmware drivers written for you by Professor Valvano. Feel free to write your own (in fact, in some labs, you may be required to write your own).

You can place any other source files in the `sw` folder. TAs will look at the files you create and/or modify for software quality and for running your project.

### 0.3 Resources

A couple files are provided in the Resources folder so you don't have to keep searching for that one TI document. Some of them are immediately useful, like the TM4C datasheet. Others may be useful for your final project, like the TM4C_System_Design_Guidelines page.

### 0.4 Git and Github

We will extensively use Git and Github for managing lab projects. This makes it easier for TAs to grade and help debug the project by allowing us to see commit histories, maintain a common project structure, and likewise, it makes it easier for students to collaborate with partners, merge different codebases, and to debug their work by having a history of commits.

Two common ways of using Git and Github are [Github Desktop](https://desktop.github.com/) and the [command line](https://git-scm.com/downloads). [Tutorials](https://dev.to/mollynem/git-github--workflow-fundamentals-5496) are also abundant on the net for you to peruse. We've provided a cheatsheet for git in the Resources folder.

It is highly recommended to make the most out of Git, even if you've never used it before. Version control will save you a lot of suffering, and tools like Git or SVN are ubiquitous in the industry.

A gitignore file is added to the root of this repo that may prevent specific files from being tagged to the repo. This are typically autogenerated output
files we don't care about, but sometimes other stuff (like .lib files) falls through that we want. Feel free to modify if necessary.

---

## 1 Summary

### 1.1 Goals

1. To interface a DC motor using PWM so software can adjust delivered power
2. To interface a tachometer so the software can measure speed
3. To implement the data acquisition with input capture interrupts
4. To implement the digital controller processing with periodic interrupts
5. To study the behavior of the system as parameters to the proportional-integral (PI) controller are adjusted

### 1.2 Team Size

The team size is 4

> "Four!" - Jhin, the Virtuoso

### 1.3 Review

1. Valvano Section 6.1.3 about using input capture to measure period
2. Valvano Section 6.3 about generating PWM outputs
3. Valvano Section 6.5 about interfacing a DC motor with a IRLD024 or IRLD120
4. [IRLD024 data sheet](resources/datasheets/irld024.pdf)
5. [IRLD120 data sheet](resources/datasheets/irld120.pdf)
6. [Pololu motor](https://www.pololu.com/product/3675)
7. Valvano Section 6.6 about integral control (you will need to add [proportional control](https://www.et.byu.edu/~tom/classes/436/ClassNotes/Class07-PI.pdf))

### 1.4 Starter Files

1. [`PWM.c`](sw/inc/PWM.c)
2. [`pid_controller.c`](sw/lib/pid_controller/pid_controller.c)
3. [`pid_controller.h`](sw/lib/pid_controller/pid_controller.h)
4. [`pid_controller_parser.c`](sw/lib/pid_controller/pid_controller_parser.c)
5. [`pid_controller_parser.h`](sw/lib/pid_controller/pid_controller_parser.h)
6. [`lab10.sch`](hw/lab10.sch)
7. EE445M http://users.ece.utexas.edu/%7Evalvano/arm/MotorTestProject.zip
8. EE445M https://www.dropbox.com/s/vtjc5j6hz9b15dj/SensorTestProject.zip?dl=1

### 1.5 Required Hardware

| Part                          | Datasheet                                                             |
|-------------------------------|-----------------------------------------------------------------------|
| EK-TM4C123GXL                 | [TM4C123GH6PM datasheet](resources/TM4C_Datasheet.pdf)                |
| ST7735R Display Driver Chip   | [ST7735R datasheet](resources/datasheets/st7735r.pdf)                 |
| Pololu Motor                  | [Link to Pololu product #3675](https://www.pololu.com/product/3675)   |
| 1N914B Switching Diode        | [1N914B datasheet](resources/datasheets/1n914.pdf)                    |
| IRLD024 MOSFET                | [IRLD024 datasheet](resources/datasheets/irld024.pdf)                 |
| IRLD120 MOSFET                | [IRLD120 datasheet](resources/datasheets/irld120.pdf)                 |
| Resistors and Capacitors      | N/A                                                                   |

### 1.6 Background

The advantages of DC motors include low cost, high speed, and high torque. However, they need sensors and closed-loop controllers in order to operate them with predictable responses. Applications of DC motors include electric cars, robotics, industrial machines (pumps, drills, mills, and lathes), consumer products (blender, washing machine, and AC/heaters) and medical devices (pumps). In this lab, you will spin a brushed DC motor (*Figure 10.2*) at a constant speed using a proportional-integral (PI) controller. The user interface will allow the operator to increase or decrease the desired speed. A tachometer will be used to measure the motor speed. A background periodic interrupt will execute the three steps of a digital controller.

1. Calculate error as the difference between actual and desired speed
2. Execute a control equation to determine the next output
3. Adjust the power to the actuator in an attempt to drive the error to zero

The software will set the power delivered to the motor using pulse-width-modulation (PWM). The essence of an integral controller is the amount you add to the power is linearly related to the error. If the motor is spinning much too slowly, you will increase power a lot. If the motor is spinning just a little bit too slowly, you will increase power a little bit. If the motor is spinning much too quickly, you will decrease power a lot. If the motor is spinning just a little bit too quickly, you will decrease power a little bit. The input capture input and the PWM output must be written at a low level, like the book, without calling TivaWare driver code. No TivaWare code is allowed.

![Figure 10.1](resources/figures/figure_10.1.jpg)

*Figure 10.1: Pololu item #3675*

![Figure 10.2](resources/figures/figure_10.2.png)

*Figure 10.2: Pololu item #3675 tachometer wiring*

Below are an example data flow graph and call graph. However, in this lab you will not be using Blynk to control the motor controller's coefficients.

![Figure 10.3](resources/figures/figure_10.3.png)

*Figure 10.3: Lab 10 data flow graph*

![Figure 10.4](resources/figures/figure_10.4.png)

*Figure 10.4: Lab 10 call graph*

#### 1.6.1 Motor Interface

Choose Port-B[6] on the TM4C123 for the PWM output. Use a 10k resistor into the gate to reduce transients. Use a 1N914 (any fast diode is ok) to remove back EMF caused by the V = L * dI/dt across the motor (see *Figure 10.5*). Connect M2 to +5V and use the IRLD024 or IRLD120 to sink current from the M1 to ground.

![Figure 10.5](resources/figures/figure_10.5.png)

*Figure 10.5: Voltage from drain to source without the shunt diode*

![Figure 10.6](resources/figures/figure_10.6.png)

*Figure 10.6: Voltage from drain to source with the shunt diode*

In order to choose a PWM period you will need to know the time constant of your motor. *Figure 10.6* is measured speed as a function of time as PWM is increased from 25% to 50%. You can estimate the time constant of this motor to be 50 ms. Therefore if the PWM period is 1ms, the motor will only respond to the average power (duty cycle), and not to the individual high and low signals of the interface.

#### 1.6.2 Tachometer Interface

To interface the tachometer, connect VCC to 3.3V and GND to GND. Choose a TM4C123 pin that supports input capture interrupts. Use a 10k pullup to 3.3V for the tachometer output (A or B). You can use either A or B. You could interface both A and B to input capture if you wished to reduce the time to measure speed or if you wished to measure direction. There are 12 tachometer pulses per rotation. Figure 10.7 shows the tachometer output of 973.4 Hz, which is 60*973.4/360 rpm = 162 rpm. If the motor spins from 0 to 162 RPM, then choose a fixed-point resolution of 0.1 or 0.01 RPM.  Choose a resolution based on the ability of the tachometer to measure speed at 100 RPM.

![Figure 10.7](resources/figures/figure_10.7.png)

*Figure 10.7: Step response of the DC motor used to stimate the time constant. Expect your measured speeds to be slower (this data was collected at 7.2V*

The tachometer interface may require filtering to improve SNR. However, be careful to minimize time delay. Time delays in the digital controller will cause the system to be unstable. For any digital filter measure the step response. A simple measure of filter delay is to make input go from …x,x,x,x,y,y,y,y… and determine many samples it takes for the output of the filter to reach x+(y-x)e-1.  For example, the delay of a 3-wide median filter is 1 sample.

You will need a separate periodic timer interrupt to determine if the motor is spinning too slow to be accurate or not spinning at all.

#### 1.6.3 Digital Controller Design

A closed loop feedback is required because not all motors are created equal. In Figure 10.8 we see the results from 8 different motors (the ones from the lab board, not this one however). One of the first steps in designing a digital controller is to make an estimate of motor speed in RPM versus PWM duty cycle. This curve will help estimate the range of speeds that your controller will be able to manage. In other words, what speeds correspond to duty cycles from 25 to 75%? Note: this response is highly dependent on friction. We expect the usable range to be around 50 to 120 RPM, but your motor may be different. You are free to control speed in rps or RPM as long as the fixed point resolution matches the tachometer error. The design progression is:

1. Incremental (stable but slow)
2. Integral (stable, a little faster)
3. Proportional-integral (may be unstable, fast)

#### 1.6.4 System Controller and Output

Display key output values of the motor controller on the LCD such as: RPS, Error, and a plot of the desired and actual speeds.

### 1.7 Requirements Document

Edit the [requirements document](requirements_document.docx) to reflect your design

---

## 2 Preparation

1. Edit the [requirements document](requirements_document.docx) to reflect your design
2. Edit the [`lab10.sch`](hw/lab10.sch) file to interface the motor and the tachometer to the TM4C. Power the motor from the USB +5V and not the +3.3V. Without a snubber diode, the back EMF will destroy your microcontroller and possibly your laptop if it is connected to the TM4C board.

![Figure 10.8](resources/figures/figure_10.8.png)

*Figure 10.8: Lab 10 schematic without routed components*

3. Design the software modules that implement the digital controller. You must have header files that compile. If you wish to see more timer and PWM examples, see the ECE445M sensor board and motor board starter projects.

---

## 3 Procedure

### 3.1 Motor Interface

With an ohmmeter, measure the resistance of the motor coil. The very fast turn-off times of the digital transistors can easily produce large back EMF voltages, so please test the hardware before connecting it to the microcontroller. Make sure your TA checks your hardware diagram before connecting it to the microcontroller.

Build the hardware interface to the motor. However, before connecting it to the microcontroller, you must use a simple switch to simulate a PWM output of 100%. Connect ammeter in series with the motor current path. Measure the motor current at full speed (100% duty cycle). Please contact your professor or TA if this current is vastly different from `120mA`. Measure the voltage drop across the motor at full speed (100% duty cycle). Call this full speed motor voltage V<sub>m</sub>.  The voltage drop across the MOSFET V<sub>DS</sub> should be much less than 1V.

*Note: V<sub>DS</sub> + V<sub>m</sub> should equal 5V.*

Test the PWM output with a scope or logic analyzer before connecting it to the motor. Write software so a switch causes the PWM to cycle through 10%, 30%, 50%, 70% and 90%.

Remove the switch, connect the PWM to the motor circuit, and reconnect the current meter in series with the motor current path. Using the PWM output in open loop fashion, measure the motor current at duty cycles `10%`, `30%`, `50%`, `70%,` and `90%`. Do this under no load. Estimate the average motor voltage as *duty * V<sub>m</sub>*. Create a graph similar to *Figure 10.9* below. Plot the straight line suggested by the ohmmeter measurement of motor resistance.

*Figure 10.9* was measured with the lab board motor, so expect your measurements to be different. However, the motor does not behave like a simple resistor. The response will also be a function of the mechanical torque (or friction) applied to the shaft.

![Figure 10.9](resources/figures/figure_10.9.png)

*Figure 10.9: Current to the lab board DC motor versus applied voltage*

### 3.2 Tachometer Interface

Build the hardware interface to the tachometer. Using an oscilloscope, verify the tachometer interface generates a digital (0V and 3.3V) signal like *Figure 10.7*. Speed measurement must occur in the background using an input capture and periodic interrupt.

Using the PWM output in open loop fashion measure the motor speed at duty cycles `10%`, `30%`, `50%`, `70%,` and `90%`. Do this under no load. Estimate the average motor power as *duty * I<sub>m</sub> * V<sub>m</sub>*. Create a graph similar to *Figure 10.10* below.

![Figure 10.10](resources/figures/figure_10.10.png)

*Figure 10.10: Motor speed versus applied power on seven different motors. Power is motor voltage multiplied by motor current multiplied by PWM duty cycle. Motor speed (RPS) is tachometer frequency (Hz) divided by 4. Notice at 100 mW, these DC motors do not spin at all. Also notice the variability between these seven motors. This response will also be a function of the mechanical torque (or friction) applied to the shaft.*

### 3.3 Digital Controller Design

After you are sure the motor and tachometer interfaces are operating properly, test a simple incremental controller, see *Figure 10.11*. Choose a PWM period, a range of usable speeds, and a controller rate consistent with the data collected in the above two procedures.

![Figure 10.11](resources/figures/figure_10.11.png)

*Figure 10.11: Incremental controller. Change the +3, -3, +100, -100, 14898 constants.*

### 3.4 Display

Create a mechanism to visualize and control the system (a method to control the system has been created for you in the form of [`pid_controller_parser.c`](sw/lib/pid_controller/pid_controller_parser.c)).

In the images below measured and desired speeds are plotted versus time as the set point is increased from 20 to 50 RPS.

![Figure 10.12](resources/figures/figure_10.12.png)

*Figure 10.12: Critically-damped PI loop*

![Figure 10.13](resources/figures/figure_10.13.png)

*Figure 10.13: Over-damped PI loop*

![Figure 10.14](resources/figures/figure_10.14.png)

*Figure 10.14: Under-damped PI loop*

![Figure 10.15](resources/figures/figure_10.15.png)

*Figure 10.15: Unstable PI loop*

---

## 4 Checkout

1. Demonstrate correct operation of the DC motor system
2. Describe how the motor interface works
3. Demonstrate debugging features that allow you to visualize the software behavior
4. Show four dampings:
   1. Critically-damped PI loop
   2. Over-damped PI loop
   3. Under-damped PI loop
   4. Unstable PI loop

---

## 5 Report

### 5.1 Deliverables

1. Objectives (final requirements document)
2. Hardware design (`.sch` file)
   1. DC motor and tachometer interfaces, showing all external components
   2. LCD and switch interfaces, showing all external components
3. Software design
   1. Include units on all software variables
   2. Clear destinction between variables used for debugging and variables needed in the controller
4. Measurement data
   1. Voltage, current and resistance measurements of the motor
   2. Graph of `motor current` vs `duty cycle * Vm`
   3. Plot of the typical tachometer output
   4. Graph of motor speed vs PWM
   5. Graph of motor speed vs power
   6. Graphs of 4 dampings:
      1. Critically-damped PI loop
      2. Over-damped PI loop
      3. Under-damped PI loop
      4. Unstable PI loop
   7. Specify maximum time to execute one instance of the ISR
   8. Determine percentage time required to run all ISRs
   9. Specify average controller error for different controllers:
      1. P controller
      2. I controller
      3. PI controller
   10. Specify response time for differenct controllers:
       1. P controller
       2. I controller
       3. PI controller

### 5.2 Analysis and Discussion Questions

1. What is torque? What are its units?
2. Draw an electrical circuit model for the DC motor coil and explain the components. Use the circuit model to explain why the current goes up when friction is applied to the shaft.
3. Explain what parameters were important for choosing a motor drive interface chip (eg. IRLD024 or IRLD120). How does your circuit satisfy these parameters?
4. You implemented an integral controller because it is simple and stable. What other controllers could you have used? How would another type of controller been superior to your integral controller?
5. If the motor is spinning at a constant rate, give a definition of electrical power in terms of parameters of this lab.
6. Research the term *mechanical power*. Give a definition of mechanical power.
7. Are electrical power and mechanical power related?

---

## 6 Hints

1. You should use a periodic interrupt to determine if the motor is stopped or spinning very slowly.
2. The USB +5V regulated supply is specified to 500mA total current. We strongly suggest you do not mechanically load the motor. It runs 120mA unloaded, but much larger if you load the motor. If your PC disconnects you, then you will need to power cycle your computer and restart the OS.
