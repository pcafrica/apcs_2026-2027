# Course project rules

## Purpose

The course project provides practical experience in the incremental development of scientific software using concepts and tools introduced during the course.

Starting from a simple computational problem, students will progressively add software design, object-oriented and generic programming, testing, documentation, build systems, third-party libraries, and C++–Python integration.

Participation is **optional**. Students who participate must complete every required component, including the final submission, presentation, and individual verification.

If you have questions or need clarification, contact the instructor or tutor well before the relevant deadline.

---

## Structure

The project will be developed incrementally through milestones announced during the course. Indicative milestones may include:

1. problem definition and baseline implementation;
2. automated testing and validation;
3. object-oriented redesign;
4. generic programming and use of templates;
5. integration of third-party libraries;
6. build configuration with CMake;
7. development of an extensible component or plugin architecture;
8. Python bindings and comparison with relevant Python implementations;
9. final release, documentation, and presentation.

Milestones support continuous progress and formative feedback. Unless otherwise stated, the final evaluation will consider the complete final submission rather than assign an independent grade to each milestone.

#### Project tracks

The instructor will provide a set of tracks with comparable scope and difficulty. Each group will select one, subject to personal preferences, capacity, and instructor approval.

Different groups may choose the same track, but each submission must be the group's independent work. Alternative topics may be proposed by the announced deadline; they must cover comparable course content and receive prior approval.

---

## Groups

Projects must normally be completed in **groups of three students**.

For justified academic or organizational reasons, the instructor may authorize groups of two or, exceptionally, individual projects. **Groups of more than three are not permitted.**

Students may form their own groups by the announced deadline. The instructor may assign students who have not joined a group by then.

Changes to group composition after the first project milestone require prior approval from the instructor.

---

## Academic integrity

The common definitions and procedures in the [academic integrity rules](academic_integrity.md) apply to the course project. **Compliance is mandatory.**

### Collaboration and authorship

The project is collaborative **within each group**. Different groups may discuss general concepts but must not share or jointly produce solution-specific code, documentation, results, or other assessed material.

Each group must accurately declare individual contributions. All members remain responsible for the complete submission and must understand its overall structure and purpose. Each student must be able to explain, test, and modify their own contributions.

### External material

Students may consult and use books, documentation, tutorials, forums, datasets, software libraries, and other external resources, subject to the project instructions and applicable licenses.

**All external material incorporated into the project must be acknowledged.** Reused or adapted code must be clearly identified and must not replace the substantive work expected from the group.

The submission must distinguish:
- work produced by the group;
- code or material reused or adapted from external sources;
- third-party libraries and datasets;
- content produced with material assistance from generative AI tools.

### Use of generative AI

Generative AI tools may support project development and learning, provided their use is **declared, critically evaluated, and consistent with the project instructions**.

Each group **must** submit an `AI_USAGE.md` file stating:
- which AI tools, if any, were used;
- the tasks for which they were used;
- representative examples of generated suggestions or code;
- how those suggestions were tested, verified, corrected, or rejected;
- which project components were completed without AI assistance.

**All group members remain fully responsible** for the correctness, quality, security, attribution, and licensing of AI-assisted content.

Undeclared, misleading, or prohibited use of AI tools may be treated as a **violation of these rules** and may result in a **penalty** to the final grade.

---

## Submission guidelines

### Deadlines and milestones

Milestone dates, the final deadline, and presentation sessions will be announced during the course.

The final submission will be due before the presentations. **The submitted version is frozen for evaluation** unless the instructor explicitly authorizes changes.

Late milestones or final submissions may be rejected or penalized. Extension requests must be submitted in advance and supported by justified circumstances.

### Format

- Submit the exam through Google Classroom using valid personal information and email address.
- Only **one student per group** should upload the final project submission on behalf of the entire group.
- Each submission must list all members, their email addresses, and a concise description of individual contributions.
- Submit the final project as a single `.tar.gz` or `.zip` archive named `Project_Surname1_Surname2_Surname3.{zip,tar.gz}`. The archive must contain the source code, tests, documentation, examples, and configuration files needed to reproduce the results.
- Include a `README.md` containing a brief description of the code structure and instructions for configuring, building, and running the code.
- Include an `AI_USAGE.md`, as detailed in the academic integrity section above.

:warning: **Do not include** generated files, executables, object files, compiled libraries, build directories, virtual environments, or large generated datasets.

### Code and reproducibility

Follow the project instructions and document the algorithms, software structure, and relevant design decisions. Code should be correct, readable, efficient, and consistent with good software-development practices.

**The project must build from a clean environment using the documented procedure.** Where applicable, separate reusable library code, executables, tests, examples, and Python bindings.

All third-party dependencies must be documented.

:warning: **Maintain backups of your work and project repository to prevent data loss.**

---

## Final presentation and individual verification

Presentations will normally take place during the **last scheduled class weeks**.

Each group will present the project and briefly demonstrate its functionality and results. **All group members must participate.**

During or immediately after the presentation, each student will answer one or more individual questions. They may be asked to:
- explain selected lines or components of the code;
- justify a design or implementation choice;
- predict the behavior of the program under a modified input;
- explain a test or numerical result;
- identify and correct a defect;
- make a small modification to the submitted code.

Individual verification assesses each student's understanding and contribution. It is part of the project assessment and is **distinct from the optional oral exam** described in the [exam rules](exam_rules.md).

Inability to explain or modify submitted work may lead to a reduction of the individual component and, where substantial discrepancies arise, to an additional verification or review.

Alternative arrangements may be granted in the presence of documented circumstances and with prior authorization from the instructor.

---

## Grading and feedback

### Grading

The course project is worth **0 to 5 points**, which contribute to the final grade as specified in the exam rules.

The evaluation will consider:
- correctness and completeness: up to **1.5 points**;
- software design and code quality: up to **0.75 points**;
- testing, build system, and reproducibility: up to **1.0 point**;
- documentation and compliance with the academic integrity rules: up to **0.75 points**;
- presentation and individual understanding: up to **1.0 point**;
- **(Bonus)** appropriate use of constructs and data structures beyond the course content.

The first four components are normally assessed at group level; presentation and individual understanding are assessed individually. **Members of the same group may therefore receive different scores.**

Without an authorized alternative arrangement, failure to attend the presentation or complete the individual verification may result in loss of the corresponding individual component.

A project that cannot be built or executed according to its documentation may receive a substantially reduced score.

### Feedback

Formative feedback may be provided at the project milestones. Further feedback on the final submission will be available upon request.
