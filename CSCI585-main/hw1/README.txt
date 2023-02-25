Rational For Design Choices

1. Many decisions were made based on the assumption of a robust contact tracing algorithm that intelligently uses the data 
stored. i.e. the is_covid_positive boolean in the employee entity could be computed based on each new entry in the Test entity. The
 boolean could be used as the "trigger" for moving an employee to the positive case list and notifying all employees that are 
related by meetings or by a simple query for the same floor number (also stored).

2. I debated making a "Workspaces" entity to relate employees to each other as we do with the "Meetings" entity. However, just storing
the floor_number entity under an employee entry seems more efficient as a simple query could find all employees on the same floor who 
would need to be notified in the event of a postive test.

3. I chose the cardinality between "Visit" and "Employee" (an employee may visit the workspace zero or more times) to allow for the 
possibility of remote workers. I don't believe this relation would affect the other entities based on their cardinalities and the paramenters
of the problem.

4. I also put a zero-many cardinality between "Test" and "Employee" because based on the problem it is possible that an employee never 
gets tested. 

5. I considered forming a composite key for employees that uses the phone number and last name. But I chose to go with an employee ID because
we can fix a length and have a large amount of possibilities vs having a potentially long last name in addition to the phone number. This could
be more efficient in terms of storage space and simplicity in design.