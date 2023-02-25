-- MILIANO MIKOL
-- Database used: https://livesql.oracle.com/

/*
Query: "It was noticed that some employees claimed to be sick but their covid tests returned 'negative'. The company 
needs to contact these employees to collect any sort of 'proof' so the big boss can  grant sick leave. This query will 
return the name and contact info of all employees whose status is sick or hospitalized but covid tests are negative."
*/

SELECT first_name, last_name, phone, email
FROM Employee
WHERE employee_id IN (
    SELECT employee_id
    FROM Test
    WHERE test_result = 'negative' AND employee_id IN (
        SELECT employee_id FROM HealthStatus WHERE status_category IN ('sick', 'hospitalized')
    )
);