HPRC Tracking Gimbal Antenna

========== Project Description ==========

This project is for Worcester Polytechnic Institute’s High Powered Rocketry Club (HPRC). During launches, the rocket transmits telemetry at only a few watts, and at altitudes approaching 45,000 feet that signal becomes extremely difficult to receive. A high-gain directional antenna is required, but such antennas have narrow beamwidths and must remain pointed at the rocket within only a few degrees to maintain a reliable link.

Because the rocket quickly becomes invisible to the naked eye, manual tracking is not viable. Instead, a motorized two-axis gimbal is required to continuously aim the antennas based on live telemetry. I was tasked with designing this system for the HPRC ground station, with a focus on reliability, portability, and closed-loop tracking performance.

Start Date: 08/21/2025

End Date: Ongoing

Last Edited: 1/10/2026

========== Background and Design Requirements ==========

The previous year’s tracking system was large, heavy, and difficult to use. It weighed over 100 pounds, was poorly balanced, overheated under load, and took more than 30 minutes to assemble. Wind loads on a large dish antenna made tracking unstable, and the motors were unable to slew fast enough to keep up with the rocket. Low-quality sensors and poorly tuned control loops made calibration unreliable and caused the closed-loop system to behave inconsistently.

![IREC 2025 Tracker](https://github.com/user-attachments/assets/a17ca0c5-a895-4122-a8f6-7488a81c4580)

Last Year's Tracker

The new system was designed around several core goals: dramatically reducing weight, increasing modularity, improving dynamic performance, and making field deployment fast and reliable. A major architectural change was moving from a large parabolic dish to lightweight Yagi antennas, which significantly reduce wind loading while still providing high directional gain. The entire system also needed to be portable, quickly assembled by a single person, and capable of tracking the rocket with sufficient speed and precision.

Simulations were run to estimate the maximum angular velocity and acceleration required to follow the rocket’s trajectory. These values were used to set motor and gearbox requirements with a large factor of safety to ensure the tracker could handle worst-case conditions.

========== Mechanical and Electrical Architecture ==========

Rather than iterating on last year’s large turntable design, the new tracker was designed as a compact, modular gimbal built around lightweight components. A speaker tripod was selected as the base because it provides high load capacity with excellent portability. The azimuth and elevation axes were redesigned to be both lighter and easier to disassemble for transport.

The system uses dovetail interfaces and quick-latch clamps so that major subassemblies can be attached or removed without tools. This allows the tracker to be deployed in minutes rather than tens of minutes. The antennas are mounted so their center of mass is aligned with the rotation axes, minimizing required motor torque and improving dynamic response.

![Dovetail Initial Prototype Open](https://github.com/user-attachments/assets/8dacef8e-86b8-4faf-a0fd-372eac6e38c8)

Initial Dovetail Prototype (Open)

![Dovetail Initial Prototype Closed](https://github.com/user-attachments/assets/0ce2be4a-9e20-466d-b443-ee3bdb6456e3)

Initial Dovetail Prototype (Closed)

A worm gear is used on the elevation axis so that wind loads cannot back-drive the motors. This prevents the antennas from being pushed off target and allows the motors to hold position without constantly applying torque. The azimuth axis is driven through a gear reduction directly rather than through a large rotating platform, reducing mass and mechanical complexity. A lazy Susan bearing is used to smooth azimuth motion while the gearbox provides stiffness and structural support.

All motors, controllers, and electronics are mounted on the rotating structure rather than split between rotating and stationary frames. This eliminates cable wrap issues and simplifies electrical integration.

========== Motor Control and Sensing ==========

Like the previous system, this design uses stepper motors, but with a critical improvement: closed-loop stepper control. Instead of relying on open-loop stepping or low-quality potentiometer feedback, the motors use integrated encoders and closed-loop controllers to maintain accurate position even under load.

This approach provides several advantages. It prevents missed steps, improves dynamic response, and allows much tighter control of position and velocity. It also removes sensitivity to temperature drift and electrical noise that affected the previous year’s sensors. These changes allow the gimbal to behave like a true servo system while still using stepper motors for their torque and simplicity.

![StepperOnline Closed-Loop Stepper Motor](https://github.com/user-attachments/assets/00fea90d-e8ac-4fc2-b6ae-a7d574ab090a)

StepperOnline Closed-Loop Stepper Motor

========== CAD, Integration, and Design Reviews ==========

The full system was designed in Onshape, which is used across the club to allow different subteams to collaborate on shared hardware. I worked directly with the antenna team to design a compatible mounting interface and ensure that both mechanical and electromagnetic constraints were respected. In particular, several structural parts had to avoid metal near the antennas to prevent interference.

The CAD assembly includes stepper motors, gearboxes, electronics, batteries, tripod, structural elements, and all custom parts. I designed several custom components, including the dovetail mounts, battery holder, and the tripod interface bracket. These parts were modeled with tolerances suitable for 3D printing in polycarbonate for high strength and temperature resistance.

The design was presented at Preliminary Design Review (PDR) and Critical Design Review (CDR) to the HPRC board. Most feedback focused on cost and procurement, and no major architectural changes were required after CDR.



<img width="2547" height="3296" alt="Full Tracker Assembly" src="https://github.com/user-attachments/assets/9844e131-f4d8-4420-8c03-5c07f7395cfd" />

Full Tracker Assembly

<img width="2547" height="3296" alt="Tracker Electronics Assembly (3-4 View)" src="https://github.com/user-attachments/assets/4deb64e4-dd14-4812-b5ff-95e077997ede" />

Tracker Electronics Assembly (3/4 View)

<img width="2547" height="3296" alt="Final Assembly 3 Inverted" src="https://github.com/user-attachments/assets/dea8418d-e195-4b67-9087-894e6e058f00" />

Tracker Electronics Assembly (Side View)

========== Current Status and Expected Results ==========

After CDR approval, a complete bill of materials was created and approved, and parts have been ordered. Assembly and programming will begin at the start of C-term.

Current mass estimates from the CDR indicate that each major subassembly will weigh no more than five pounds, with the largest module around twenty pounds. The entire system is designed to be assembled by one person in a few minutes and transported easily to the launch site. If these estimates hold in testing, the new tracker will represent a massive improvement over last year’s system.

The tracker will be tested and tuned throughout the next two academic terms in preparation for IREC 2026, with extensive validation of tracking accuracy, slew rate, and reliability.

========== Engineering Takeaways ==========

This project significantly expanded my ability to think and work at a systems level rather than treating mechanical, electrical, and control problems as independent tasks. Designing a tracking gimbal forced me to consider how inertia, motor torque, wind loading, sensor quality, and control loop behavior all interact in a tightly coupled system. Small decisions in one area, such as shifting the center of mass or choosing a gearbox, had large effects on motor heating, tracking accuracy, and overall stability. Learning to reason about these interactions was one of the most valuable outcomes of the project.

I also gained experience converting vague, real-world goals into concrete engineering requirements. “Track a rocket at 45,000 feet” had to become numerical targets for angular velocity, acceleration, pointing accuracy, and mechanical stiffness. Running simulations to estimate worst-case motion and then designing with large safety margins taught me how to approach uncertainty in engineering design rather than guessing or relying on intuition alone.

Working with closed-loop stepper motors deepened my understanding of feedback control in real hardware. Unlike the previous year’s system, which relied on noisy potentiometers and loosely tuned control loops, this design uses encoder feedback and closed-loop motor drivers to actively correct for disturbances such as wind and changing loads. This required thinking about how sensor quality, control bandwidth, and mechanical backlash affect stability and responsiveness, which is directly applicable to robotic arms, mobile robots, and other mechatronic systems.

Collaboration with multiple subteams introduced me to the realities of large hardware projects. The antenna team, the club board, and the avionics group all had constraints that affected the tracker design, including cost limits, electromagnetic interference concerns, and mechanical interface changes. Using Onshape’s versioning and shared part system allowed me to branch designs, preserve stable configurations, and adapt quickly when other teams made changes. This taught me how to design in an environment where requirements are not fixed and interfaces evolve over time.

Creating and maintaining a complete bill of materials was another major learning experience. I had to consider not just what parts worked on paper, but what could be purchased within budget, delivered on time, and substituted if supply issues arose. This shifted my thinking from idealized designs to ones that are actually manufacturable and deployable, which is a critical skill in real engineering projects.

Finally, rapid prototyping and 3D printing allowed me to test assumptions early and cheaply. By printing and fitting parts before finalizing the design, I learned how tolerances, material stiffness, and assembly constraints affect real-world performance. This iterative approach reduced risk and helped me converge on a design that balances strength, weight, and ease of assembly.

========== Summary ==========

The HPRC tracking gimbal is a multidisciplinary robotics system combining mechanical design, motor control, sensing, and real-time tracking under environmental uncertainty. It is designed to be lightweight, portable, and robust enough to autonomously track a rocket traveling tens of thousands of feet into the air. The project has provided hands-on experience with the same challenges faced in real robotic platforms: dynamic loads, feedback control, modularity, and system integration.
