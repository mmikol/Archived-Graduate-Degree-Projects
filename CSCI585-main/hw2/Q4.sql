-- MILIANO MIKOL
-- Database used: https://livesql.oracle.com/

SELECT COUNT(*) AS "Number of Scans"
FROM Scan
WHERE scan_date BETWEEN TO_DATE('08/15/2021', 'MM/DD/YYYY') AND TO_DATE('08/24/2021', 'MM/DD/YYYY');

SELECT COUNT(*) AS "Number of Tests"
FROM Test
WHERE test_date BETWEEN TO_DATE('08/15/2021', 'MM/DD/YYYY') AND TO_DATE('08/24/2021', 'MM/DD/YYYY');

SELECT COUNT(*) AS "Number of Reports"
FROM Symptom
WHERE symptom_report_date BETWEEN TO_DATE('08/15/2021', 'MM/DD/YYYY') AND TO_DATE('08/24/2021', 'MM/DD/YYYY');

SELECT COUNT(*) AS "Number of Positive Cases"
FROM PositiveCase
WHERE case_date BETWEEN TO_DATE('08/15/2021', 'MM/DD/YYYY') AND TO_DATE('08/24/2021', 'MM/DD/YYYY');
