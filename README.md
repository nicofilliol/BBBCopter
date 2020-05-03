# BBBCopter

## About The Project
The goal of the project is to build a quadcopter. Of course, there are many different possibilities to chose from for both hardware and software. For this project, the goal was to get a drone that would be as affordable as possible but it should also be possible to control the drone from a computer/base station and it has to be programmable so that the project could later on be improved and things such as computer vision could be implemented.

### Built With
In order to fulfil the requirements of our drone listed above, we decided to use following hardware and software:
* [BeagleBone Blue](https://beagleboard.org/blue)
* [ArduPilot](https://ardupilot.org/)
* [ROS](https://www.ros.org/)

## Getting Started

In this section, the building process of the drone will be described.

### Parts List

| Part | Amount | Weight per Unit | Total Weight | Link |
|-------------------------------------|--------|-----------------|--------------|-------------------------------------------------------------------------------------------------------|
| BeagleBone Blue | 1         | 36g | 36g | [Farnell](https://ch.farnell.com/beagleboard/bbone-blue/beaglebone-blue-robotics-platform/dp/2612583) |
| F450 Frame | 1         | 278g | 278g | [AliExpress](https://de.aliexpress.com/item/4000261171695.html) |
| 2212 920KV Motor | 4                                       | 53g | 212g | [AliExpress](https://de.aliexpress.com/item/4000126748240.html) |
| 30A ESC 2-4S Lipo | 4                                       | 25g | 100g | [AliExpress](https://de.aliexpress.com/item/4000126748240.html) |
| 9450 Self-Locking Propeller | 4                                       | 22g | 88g | [AliExpress](https://de.aliexpress.com/item/4000126748240.html) |
| S603 6CH 2.4GHz PPM Receiver | 1         | 12g | 12g | [AliExpress](https://de.aliexpress.com/item/4000340190534.html) |
| NEO-M8N GPS Module | 1         | 29g | 29g | [AliExpress](https://de.aliexpress.com/item/33054561368.html) |
| 3DR 500MW 433 MHz Telemetry Module  | 1         | 45g | 45g | [AliExpress](https://de.aliexpress.com/item/4000255803909.html) |
| 3S Lipo Battery XXXmAh | 1         |  |  |  |

### Thrust Estimation
The motor, propeller and battery combination listed above must provide enough thrust to lift the whole drone. In order to achieve this, we want a thrust/weight ratio greater than 1.5. According to experiments with similar motor/propeller/battery setup, we can expect following estimates:

| Thrust per Motor | Maximum Total Thrust | Maximum Current Draw |
|------------------|----------------------|----------------------|
| 600 | 2400 | XXA |

<br>

| Total Weight | Target T/R Ratio | Target Thrust |
|--------------|------------------|---------------|
| XXX | 1.5 | XXX |

As we can see by comparing maximum total thrust and target thrust, our final drone should be able to lift itself into the air. Furthermore, we also must make sure that the maximum current draw can be handled by the ESC, which in our case is fine.