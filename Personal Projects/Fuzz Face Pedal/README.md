Fuzz Face Guitar Pedal

========== Project Description ==========

This project was a Christmas gift for my father, who wanted a Jimi Hendrix–style Fuzz Face guitar pedal. After recently learning PCB design, I realized this would be a good opportunity to design and build a complete electronics product from scratch rather than assembling a kit or copying an existing board.

Beyond recreating a classic pedal, my primary goal was to become self-sufficient in the entire hardware development process. This included schematic design, PCB layout, component sourcing, surface-mount assembly, debugging, enclosure design, and final system integration. The project ended up being significantly more educational than expected and became one of my most hands-on learning experiences in electronics engineering.

Start Date: 11/15/25

End Date: 12/24/25

Last Edited: 1/8/2026

========== Research and Initial Design ==========

At the start of the project, I researched whether building a Fuzz Face pedal was feasible at my skill level. While the circuit itself is relatively simple, it is highly sensitive to component selection, particularly transistor gain. I relied on three primary references throughout the project: a technical breakdown of the Fuzz Face circuit, a silicon Fuzz Face build guide, and an article explaining BJT gain and beta.

Using these resources, I selected the “Fuzz Face with Roger Mayer / Jimi Hendrix mods” schematic as a starting point and recreated it in KiCad to familiarize myself with the circuit and prepare for PCB layout. While comparing multiple schematics, I noticed inconsistencies in component orientation and transistor polarity. Rather than copying a single design, I spent time understanding why these differences existed, which became important later when adapting the circuit.

========== Transistor Selection and Circuit Adaptation ==========

One of the most important discoveries during this project was how critical transistor gain (hFE) is to the sound of a Fuzz Face pedal. The target gains were approximately 90 hFE for the first transistor and 140 hFE for the second. Original Hendrix pedals used PNP germanium transistors, which are expensive, difficult to source consistently, and often sold individually after being manually tested.

To keep the project affordable and practical, I chose to redesign the circuit using NPN silicon transistors. This required fully inverting the circuit topology and reversing component orientation, rather than simply swapping parts. Understanding current flow, biasing, and polarity was essential to making this change correctly.

While testing transistors, I learned that bulk transistor tapes typically come from the same manufacturing batch and therefore have very similar gain values. This made it difficult to achieve the mismatched gains required for the circuit. I ultimately sourced transistors from mixed batches and measured them manually to select appropriate pairs.

========== PCB Design and Manufacturing ==========

The PCB was designed in KiCad with an emphasis on compactness. I initially planned to mount the audio jacks and battery directly on the PCB, but mechanical clearance issues and footprint limitations made this impractical. I eventually transitioned to a wired design paired with a custom enclosure.

The board used surface-mount components with hand-solderable footprints. Parts were sourced primarily from Digi-Key, and the PCB was fabricated by JLCPCB. Several design iterations were required due to footprint mismatches and underestimated component sizes, which reinforced the importance of selecting exact parts before committing to a layout.

========== Assembly and Early Testing ==========

Before leaving campus for winter break, I soldered all PCB components using lab equipment. Due to time constraints, I did not fully test the circuit before leaving, which later became a major lesson learned. At the time, visual inspection suggested the board was assembled correctly, but this proved to be unreliable without proper electrical verification.

========== Debugging, Prototyping, and Enclosure Design ==========

Once home for winter break, I tested the pedal with an electric guitar and amplifier and found that it did not function as expected. I began debugging using a multimeter, oscilloscope, and waveform generator. This process revealed several issues, including incomplete solder joints, incorrect audio jack wiring, and the difficulty of probing surface-mount components.

In parallel, I designed a custom enclosure modeled after a Fuzz Face Mini pedal. Due to limited published dimensions, I worked from reference images and estimated measurements. I imported manufacturer models of the jacks and potentiometers to verify fit, printed the enclosure in PLA, and finished it with primer and paint to match the appearance of classic pedals.

Despite extensive debugging, it became unclear whether the remaining issues were caused by individual components, soldering quality, or the PCB design itself. To isolate the problem, I rebuilt the circuit using through-hole components on a breadboard. The circuit worked immediately, confirming that the schematic was correct and that the issues were related to implementation rather than design.

========== Final Implementation and Testing ==========

After validating the circuit on a breadboard, I transferred the design to a protoboard that could fit inside the enclosure. I carefully recreated the circuit, checked continuity and resistance across all connections, and resolved several soldering issues. Once completed, the circuit was significantly more stable than the breadboard version and produced consistent output without noise when components were moved.
I performed extended testing using a guitar and amplifier, verifying the behavior of the volume, fuzz, and custom modification potentiometer. The final assembly was then completed and prepared for gifting.

========== Results ==========

A commercially available Fuzz Face pedal costs approximately $180, while this build cost around $75. The cost was higher than necessary due to replacement parts and redesigns, but those expenses were a direct result of learning through iteration. With the experience gained from this project, I am confident that I could recreate a similar pedal more efficiently and at a lower cost.

The final sound closely matched the classic Hendrix-style fuzz I was aiming for. The added modification control provided additional tonal flexibility, giving the pedal a distinct character beyond the original design. The enclosure fit well, and the overall build quality exceeded my initial expectations. Most importantly, my father was very happy with the final result.

========== Engineering Takeaways ==========

This project was my first experience building a complete electronic system from schematic to physical product, and it fundamentally changed how I think about hardware design. I learned that a working schematic is only a small part of a successful system. Component selection, footprint accuracy, solderability, mechanical constraints, and signal integrity all play equally important roles. Small mismatches between what exists in CAD and what arrives in the mail can easily cause a design to fail if they are not carefully planned for.

Debugging the pedal forced me to develop a structured approach to fault isolation. When the PCB did not work, I had to determine whether the issue came from the circuit design, the PCB layout, the soldering quality, or individual components. Rebuilding the same circuit on a breadboard allowed me to separate the electrical design from the physical implementation, which is a critical debugging technique in robotics and embedded systems. This process taught me how to systematically reduce uncertainty rather than randomly changing parts and hoping the problem disappears.

I gained practical experience working with analog electronics, particularly transistor biasing and gain matching. The Fuzz Face is extremely sensitive to transistor parameters, which meant I could not treat parts as interchangeable. Measuring hFE, understanding manufacturing batches, and selecting matched transistor pairs taught me how real components differ from their idealized datasheet values. This is directly relevant to any robotics system that relies on sensors, amplifiers, or power electronics, where component variation can significantly affect behavior.

The project also highlighted the importance of feedback between electrical and mechanical design. Decisions about PCB size, connector placement, and component orientation affected how the enclosure could be designed and how reliable the final product felt. Designing the enclosure after the PCB rather than before created unnecessary constraints, which reinforced the importance of co-designing mechanical and electrical systems in parallel.

Using surface-mount components gave me experience with manufacturing-scale electronics, but also revealed the tradeoffs between compactness and serviceability. SMDs made the board smaller and more professional-looking, but they were much harder to probe, rework, and debug than through-hole parts. This taught me how to choose different technologies depending on whether the goal is rapid prototyping, testing, or final production.

Finally, the project taught me how to manage iteration under real constraints. Time, cost, part availability, and tool access all influenced the decisions I had to make. Instead of aiming for a perfect first build, I learned to converge on a working solution through testing, failure, and incremental improvement. That mindset is central to building reliable robotic and electronic systems in the real world.

========== Summary ==========

This project was more than a guitar pedal build. It was a complete hardware development cycle involving electrical design, manufacturing, mechanical integration, debugging, and iteration under time constraints. The experience directly improved my confidence and competence in designing, building, and troubleshooting real-world electronic systems, and it closely reflects the type of multidisciplinary problem solving required in robotics engineering.
