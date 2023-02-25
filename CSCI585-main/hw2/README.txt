Miliano Mikol
CSCI 585
HW 2

Question 2, 3:
I needed to find the max or min of some attribute. I considered using a 'LIMIT' statement however,
I realized there was no smart way to limit how many rows would meet my condition as there could be ties in the most reported
symptom and number of sick people per floor. This is why I decided to create a temporary table using a 'WITH' clause  and search
for all rows that meet the condition 'IN' that table.

Question 5:
As described in the text file, the query is as follows: "It was observed that some employees claimed to be hospitalized or sick 
but their covid tests returned 'negative'. The company needs to contact these employees to collect any sort of 'proof' so the big 
boss can determine whether to grant sick leave. This query will return the name and contact info of all employees whose status is 
sick or hospitalized but covid tests are negative."