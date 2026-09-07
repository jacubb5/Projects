HPRC Tracking Gimbal Antenna

========== Project Description ==========

This project is for Worcester Polytechnic Institute’s High Powered Rocketry Club (HPRC). During launches, the rocket transmits telemetry at only a few watts, and at altitudes approaching 45,000 feet that signal becomes extremely difficult to receive. A high-gain directional antenna is required, but such antennas have narrow beamwidths and must remain pointed at the rocket within only a few degrees to maintain a reliable link.

Because the rocket quickly becomes invisible to the naked eye, manual tracking is not viable. Instead, a motorized two-axis gimbal is required to continuously aim the antennas based on live telemetry. I was tasked with designing this system for the HPRC ground station, with a focus on reliability, portability, and closed-loop tracking performance.

Start Date: 08/21/2025

End Date: 06/21/2026

Last Edited: 09/07/2026

========== Background and Design Requirements ==========

The previous year’s tracking system was large, heavy, and difficult to transport. It weighed over 100 pounds, was poorly balanced, overheated under load, and took more than 30 minutes to assemble. Wind loads on a large dish antenna made tracking unstable, and the motors were unable to slew fast enough to keep up with the rocket. Low-quality sensors and poorly tuned control loops made calibration unreliable and caused the closed-loop system to behave inconsistently.

![IREC 2025 Tracker](https://github.com/user-attachments/assets/a17ca0c5-a895-4122-a8f6-7488a81c4580)

Last Year's Tracker

The new system was designed around several core goals: dramatically reducing weight, increasing modularity, improving dynamic performance, and making field deployment fast and reliable. A major architectural change was moving from a large parabolic dish to lightweight Yagi antennas, which significantly reduce wind loading while still providing high directional gain. The entire system also needed to be portable, quickly assembled by a single person, and capable of tracking the rocket with sufficient speed and precision.

Simulations were run to estimate the maximum angular velocity and acceleration required to follow the rocket’s trajectory. These values were used to set motor and gearbox requirements with a large factor of safety to ensure the tracker could handle worst-case conditions.

========== Mechanical and Electrical Architecture ==========

Rather than iterating on last year’s large turntable design, the new tracker was designed as a compact, modular gimbal built around lightweight components. A speaker tripod was selected as the base because it provides high load capacity with excellent portability. The azimuth and elevation axes were redesigned to be both lighter and easier to disassemble for transport.

The system uses dovetail interfaces and locking pins so that major subassemblies can be attached or removed without tools. This allows the tracker to be deployed in minutes rather than tens of minutes. The antennas are mounted so their center of mass is aligned with the rotation axes, minimizing required motor torque and improving dynamic response.

![Dovetail Initial Prototype Open](https://github.com/user-attachments/assets/8dacef8e-86b8-4faf-a0fd-372eac6e38c8)

Initial Dovetail Prototype (Open)

![Dovetail Initial Prototype Closed](https://github.com/user-attachments/assets/0ce2be4a-9e20-466d-b443-ee3bdb6456e3)

Initial Dovetail Prototype (Closed)

A worm gear is used on the elevation axis so that wind loads cannot back-drive the motors. This prevents the antennas from being pushed off target and allows the motors to hold position without constantly applying torque. The azimuth axis is driven through a gear reduction directly rather than through a large rotating platform, reducing mass and mechanical complexity. A lazy Susan bearing is used to smooth azimuth motion while the gearbox provides stiffness and structural support.

All motors, controllers, and electronics are mounted on the rotating structure rather than split between rotating and stationary frames. This eliminates cable wrap issues and simplifies electrical integration.

========== Motor Control and Sensing ==========

Like the previous system, this design uses stepper motors, but with a critical improvement: closed-loop stepper control. Instead of relying on open-loop stepping or low-quality potentiometer feedback, the motors use integrated encoders and closed-loop controllers to maintain accurate position even under load.

This approach provides several advantages. It prevents missed steps, improves dynamic response, and allows much tighter control of position and velocity. It also removes sensitivity to temperature drift and electrical noise that affected the previous year’s sensors. These changes allow the gimbal to behave like a true servo system while still using stepper motors for their torque and simplicity.

<img width="487" height="506" alt="image" src="https://github.com/user-attachments/assets/77c8bb19-87f1-4032-9012-aff62175e2b4" />

StepperOnline Closed-Loop Stepper Motor

========== CAD, Integration, and Design Reviews ==========

The full system was designed in Onshape, which is used across the club to allow different subteams to collaborate on shared hardware. I worked directly with the antenna team to design a compatible mounting interface and ensure that both mechanical and electromagnetic constraints were respected. In particular, several structural parts had to avoid metal near the antennas to prevent interference.

The CAD assembly includes stepper motors, gearboxes, electronics, batteries, tripod, structural elements, and all custom parts. I designed several custom components, including the dovetail mounts, battery mounting, and the tripod interface bracket. These parts were modeled with tolerances suitable for 3D printing in polycarbonate for high strength and temperature resistance.

The design was presented at Preliminary Design Review (PDR) and Critical Design Review (CDR) to the HPRC board. Most feedback focused on cost and procurement, and no major architectural changes were required after CDR.

<img width="2547" height="3296" alt="Full Tracker Assembly" src="https://github.com/user-attachments/assets/9844e131-f4d8-4420-8c03-5c07f7395cfd" />

Full Tracker Assembly

<img width="2547" height="3296" alt="Tracker Electronics Assembly (3-4 View)" src="https://github.com/user-attachments/assets/4deb64e4-dd14-4812-b5ff-95e077997ede" />

Tracker Electronics Assembly (3/4 View)

<img width="2547" height="3296" alt="Final Assembly 3 Inverted" src="https://github.com/user-attachments/assets/dea8418d-e195-4b67-9087-894e6e058f00" />

Tracker Electronics Assembly (Side View)

========== Structural Assembly ==========

<img width="2160" height="3240" alt="image" src="https://github.com/user-attachments/assets/e8890ce2-fcec-4e5e-8c1e-aa0599005168" />

Main Tracker Assembly

Assembly was executed cleanly, with the overall physical structure demonstrating high rigidity and confirming the strength of the 3D-printed polycarbonate components. The complete tracker assembly weighed in at 13 pounds—significantly under the 20-pound CDR mass estimate, representing a massive improvement over the previous year’s 100+ pound system.

The primary mechanical issue encountered was minor backlash in the worm gear setup. Several 3D-printed components required minor tolerancing adjustments and re-printing to ensure proper fitment. However, because polycarbonate stock was limited, an over-reliance on CAD iteration in Onshape was substituted for rapid physical prototyping to conserve material. This caution backfired: skipping multiple physical test-fits delayed assembly and delayed finding minor fitment issues early. Ultimately, overall mechanical assembly proceeded without major redesigns, but it highlighted the cost of hoarding raw material instead of testing early.

<img width="2130" height="3787" alt="image" src="https://github.com/user-attachments/assets/fbcf3cfb-49d8-48ef-9c5b-f16a130ae2cf" />

Main Tracker Assembly on Tripod

========== Programming and Debugging ==========

Since the setup lacked absolute position encoders, a dedicated homing and calibration sequence was developed to establish an accurate spatial reference relative to the rocket’s launch coordinates:

- State-Aware Sensor Calibration: Combined the absolute positioning reference of magnetic limit switches with the precise step-count feedback of the integrated closed-loop stepper drivers. When a magnetic limit switch was triggered, the firmware dynamically set the current absolute axis angle based on both which specific switch was tripped and the vector direction the axis was actively traveling.

- Control Loop Architecture: Built around a simple proportional control loop targeting position with a strict velocity limit. Complex closed-loop PID control was handled directly onboard the stepper motor drivers.

- Black-Box Command Interface: Designed to accept high-level serial commands from the main ground station laptop (e.g., C for calibration routines, V + value for velocity target, S for emergency stop). This modular interface cleanly separated tracker logic from ground station software.

- Closed-Loop Reliability: Operating closed-loop eliminated the need for continuous position telemetry back to the main controller, as the driver guarantee ensured no lost steps under expected load conditions.

- Field Testing Setup: Manual control and response tuning were verified by mapping inputs from a standard USB game controller over a serial COM port, allowing smooth stress-testing across diverse operational scenarios.

========== IREC 2026 Field Experience ==========

<img width="2160" height="3840" alt="image" src="https://github.com/user-attachments/assets/481de283-f0a2-4099-8945-71ea772efd26" />

Fully Assembled Tracker System

Deploying the system at IREC provided valuable hands-on insights into field performance under extreme environmental conditions:

- Portability & Field Setup: Transportation and physical setup at the launch site were rapid and seamless. The modular dovetails and lightweight 13-pound frame met the key goal of single-person assembly, with antenna mounting taking up the majority of setup time.

- Thermal Challenges: The intense heat of the Texas sun caused severe thermal soaking. Black surfaces (like stepper motor bodies) became extremely hot, metal transport handles became painful to grip without gloves, and the polycarbonate 3D prints softened enough to show increased flexibility under load.

- System Interactions: Because full antenna integration couldn't occur until arriving at the competition venue, dynamic instability and structural wobbling—exacerbated by thermal softening—were discovered on-site where physical geometry changes could no longer be printed.

<img width="3840" height="2160" alt="image" src="https://github.com/user-attachments/assets/b38c403f-bd91-4f20-aac4-30da94487d72" />

Tracker and Ground Station Setup (Launch Day)

========== Future Improvements ==========

To address the limitations discovered during testing and field deployment, the next iteration will focus on several key redesigns:

- Design for Maintenance (DFM/DFA): Access points, counterbores, and clearance around hardware will be overhauled. The initial build contained recessed nuts that were difficult to tighten and sharp pinch points that made maintenance hazardous.

- Automated Reference & Sensing: The manual calibration sequence was slow and tedious. Integrating a magnetometer, GPS unit, and cheap absolute magnetic encoders directly on the output shafts will allow the system to self-orient to true north and establish precise angular position instantly.

- Natural Manual Override Interface: Replacing the dual-axis game controller inputs with an IMU-enabled "direction stick." This will allow an operator to intuitively point a physical wand at the rocket to command matching gimbal orientation if telemetry lock is lost.

- Thermal & Structural Stiffness: Replacing thermal-sensitive plastic dovetail interfaces and sloppier azimuth bearings with higher-precision, metallic or heat-stable load-bearing hardware to eliminate mechanical play.

- Motion Profiling: Replacing the sharp velocity-limited proportional control with S-curve motion profiling to smooth out acceleration and jerk, eliminating the aggressive snapping that excited mechanical resonance in the structure.

========== Engineering Takeaways ==========

This project significantly expanded my ability to think and work at a systems level rather than treating mechanical, electrical, and control problems as independent tasks. Designing a tracking gimbal forced me to consider how inertia, motor torque, wind loading, sensor quality, and control loop behavior all interact in a tightly coupled system. Small decisions in one area, such as shifting the center of mass or choosing a gearbox, had large effects on motor heating, tracking accuracy, and overall stability. Learning to reason about these interactions was one of the most valuable outcomes of the project.  

I also gained experience converting vague, real-world goals into concrete engineering requirements. “Track a rocket at 30,000 feet” had to become numerical targets for angular velocity, acceleration, pointing accuracy, and mechanical stiffness. Running simulations to estimate worst-case motion and then designing with large safety margins taught me how to approach uncertainty in engineering design rather than guessing or relying on intuition alone.  

Working with closed-loop stepper motors deepened my understanding of feedback control in real hardware. Unlike the previous year’s system, which relied on noisy potentiometers and loosely tuned control loops, this design uses encoder feedback and closed-loop motor drivers to actively correct for disturbances such as wind and changing loads. Fusing multiple sensing approaches, such as using direction and switch-sensitive state logic to set absolute reference angles before handing tracking off to high-resolution quadrature encoders, taught me how to establish reliable absolute spatial positioning out of relative sensors.  

A major practical takeaway was learning the critical engineering balance between cost/material optimization and schedule execution. In an effort to conserve our limited supply of polycarbonate filament, I spent excessive time tweaking models inside Onshape rather than rapidly printing prototypes. In hindsight, trying to "get it perfect on screen" to save a small amount of material created unnecessary development bottlenecks and delayed physical integration until late in the timeline. I learned that cheap, fast iterations in physical space yield insights that CAD never reveals, and balancing material budgets against project timelines is a critical lesson I will carry into future projects.

Collaboration with multiple subteams introduced me to the realities of large hardware projects. The antenna team, the club board, and the avionics group all had constraints that affected the tracker design, including cost limits, electromagnetic interference concerns, and mechanical interface changes. Using Onshape’s versioning and shared part system allowed me to branch designs, preserve stable configurations, and adapt quickly when other teams made changes.  

Creating and maintaining a complete bill of materials was another major learning experience. I had to consider not just what parts worked on paper, but what could be purchased within budget, delivered on time, and substituted if supply issues arose. Hands-on fabrication drastically improved my practical electronics and wiring skills. Initially lacking experience joining larger wire gauges, I learned to rely on specialized crimps, heat shrink, header pins, and dedicated connectors rather than solder alone. Understanding the brittle nature of solder joints under mechanical stress and vibration led me to adopt proper strain-relief and crimping practices necessary for high-reliability mobile systems.  

Finally, rapid prototyping and field testing highlighted the vital importance of Design for Assembly (DFA) and Maintenance. Experiencing hardware access issues in the field emphasized that an engineering design is only as good as its serviceability. By evaluating how fastener accessibility, pinch points, tolerances, and thermal environments affect real-world performance, I learned how to approach future designs holistically from concept to field teardown.
