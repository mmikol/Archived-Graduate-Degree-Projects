-- MILIANO MIKOL
-- Database used: https://livesql.oracle.com/

WITH SickCountPerFloor AS (
    SELECT COUNT(floor_number) AS number_of_sick, floor_number
    FROM Employee
    WHERE Employee.employee_id IN (
        SELECT employee_id
        FROM HealthStatus
        WHERE status_category IN ('sick', 'hospitalized')
    )
    GROUP BY floor_number
)
SELECT floor_number
FROM SickCountPerFloor
WHERE number_of_sick IN (
    SELECT MAX(number_of_sick)
    FROM SickCountPerFloor
);