Sensing and Perception in Robotics

Course Projects

========== Description ==========

This course was a quarter-long, lab-driven robotics class built around a small mobile robot (Romi). Each week added new sensing, control, and perception capabilities on top of the previous work, gradually building toward a fully autonomous robot.

The final project required the robot to locate, approach, pick up, weigh, and transport “trash cans” (plastic buckets) to a designated dump site. The system combined odometry, AprilTag vision, distance sensors, a load cell, and a servo-based lifting mechanism, all coordinated through a multi-state control system.

Unlike a single standalone project, this course progressed continuously: each weekly lab became part of the final robot. This repository documents the topics learned each week and the final versions of the code that enabled the full autonomous behavior.

Start Date: 03/17/2025

End Date: 05/07/2025

Last Edited: 01/12/2026

========== Topics Covered in the Course ==========

Platform, Control, and Foundations

Early work focused on PlatformIO, Romi hardware, and GitHub-based collaboration. Motor encoders were measured and characterized using oscilloscopes, and wheel dimensions were used to convert encoder ticks into distance. PI control loops were implemented and tuned using Teleplot to stabilize driving performance. Reflectance sensors were used to implement line following in a grid, and the first state machines were introduced to manage robot behavior.

Distance Sensing and Filtering

IR and ultrasonic sensors were added and calibrated using measured distance samples to create correction curves. Multiple filtering techniques were implemented, including running averages, windowed averages, weighted averages, and median filters. Hysteresis and Schmitt-trigger style logic were used to reduce noise-induced oscillations in decision making.

Manipulation and Force Sensing

A servo-driven lifting mechanism and a load cell (with HX711 amplifier) were integrated to pick up and weigh objects. The load cell was calibrated in software, and amplification and sampling modes were selected from datasheets. Power was separated between logic and motors using a breadboard.

Inertial Navigation

The Romi’s IMU (accelerometer and gyroscope) was used with a complementary filter to produce smooth and reliable orientation data. Pitch was used to detect ramps, and gravity alignment was used to ensure the robot climbed at the steepest direction.

Odometry and Kinematics

Forward and inverse kinematics were implemented to track robot position in an x-y grid. Encoder-based odometry allowed the robot to drive repeatedly to specific coordinates and headings with minimal drift.

Vision-Based Perception

An OpenMV camera was used for AprilTag detection. Tag position, size, and ID were transmitted over I2C to the Romi, allowing the robot to search for, align with, and approach tagged trash cans autonomously.

========== What I Learned ==========

This class taught far more than just how to wire sensors to a robot. It showed how real robotic systems are built incrementally, tuned under uncertainty, and coordinated through software architecture.

===== Robotics as a layered system =====

By adding new sensors and capabilities each week, I learned how robotics systems evolve rather than being built all at once. Every new sensor (IMU, distance sensor, camera, load cell) had to integrate cleanly with existing control loops and state machines without breaking prior behavior.

===== Sensor calibration and data quality =====

Raw sensor values are rarely usable. I learned how to create calibration curves from measured data, choose appropriate filtering methods, and apply hysteresis to stabilize decisions. These techniques dramatically improved reliability and are directly applicable to any real-world sensing system.

===== State-machine driven autonomy =====

The checker/handler state-machine architecture allowed multiple conditions to be evaluated simultaneously and mapped to different robot behaviors. This made it possible to combine navigation, perception, and manipulation into a single coherent autonomous system.

===== Using documentation as an engineering tool =====

Datasheets for sensors like the HX711, IR rangefinder, and IMU were essential for choosing operating modes, gain settings, and communication protocols. This class reinforced how often engineering decisions are driven by documentation rather than guesswork.

===== Team-based engineering workflow =====

Using GitHub branches, commits, and merges with a partner taught me how to coordinate work, track changes, and recover from mistakes. Weekly deadlines also forced realistic decisions about scope, time management, and when a solution is “good enough” to ship.