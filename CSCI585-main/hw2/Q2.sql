-- MILIANO MIKOL
-- Database used: https://livesql.oracle.com/

WITH SymptomCount AS (
    SELECT COUNT(symptom_category) AS report_count, symptom_category
    FROM Symptom
    GROUP BY symptom_category
)
SELECT symptom_category AS "Most Reported Symptom"
FROM SymptomCount
WHERE report_count IN (
    SELECT MAX(report_count)
    FROM SymptomCount
);