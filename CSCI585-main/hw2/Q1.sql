-- MILIANO MIKOL
-- Database used: https://livesql.oracle.com/

CREATE TABLE Employee (
    employee_id   INT NOT NULL,
    first_name    VARCHAR(15),
    last_name     VARCHAR(15),
    office_number INT,
    floor_number  INT, 
    phone         INT,
    email         VARCHAR(50),
    PRIMARY KEY(employee_id),
    CHECK (floor_number BETWEEN 1 AND 10)
);

CREATE TABLE Meeting
(
    meeting_id   INT NOT NULL,
    employee_id  INT NOT NULL,
    room_number  INT,
    floor_number INT, 
    meeting_date DATE,
    start_time   INT,
    end_time     INT,
    PRIMARY KEY(meeting_id, employee_id),
    FOREIGN KEY (employee_id) REFERENCES Employee(employee_id),
    CHECK (floor_number BETWEEN 1 AND 10),
    CHECK (start_time > 8 AND start_time < 18)
);

CREATE TABLE ExposureNotification
(
    notification_id   INT NOT NULL,
    employee_id       INT NOT NULL,
    notification_date DATE,
    notification_type VARCHAR(10), 
    PRIMARY KEY(notification_id),
    FOREIGN KEY (employee_id) REFERENCES Employee(employee_id),
    CHECK (notification_type IN ('mandatory', 'optional'))
);

CREATE TABLE Symptom
(
    symptom_report_id   INT NOT NULL,
    employee_id         INT NOT NULL,
    symptom_report_date DATE,
    symptom_category    INT,
    PRIMARY KEY(symptom_report_id),
    FOREIGN KEY (employee_id) REFERENCES Employee(employee_id),
    CHECK (symptom_category BETWEEN 1 AND 5)
);

CREATE TABLE Scan
(
    scan_id     INT NOT NULL,
    employee_id INT NOT NULL,
    scan_date   DATE,
    scan_time   INT,
    temperature INT,
    PRIMARY KEY(scan_id),
    FOREIGN KEY (employee_id) REFERENCES Employee(employee_id),
    CHECK (scan_time >= 0 AND scan_time < 24)
);

CREATE TABLE Test
(
    test_id       INT NOT NULL,
    employee_id   INT NOT NULL,
    test_location VARCHAR(20),
    test_date     DATE,
    test_time     INT,
    test_result   VARCHAR(8),
    PRIMARY KEY(test_id),
    FOREIGN KEY (employee_id) REFERENCES Employee(employee_id),
    CHECK (test_location IN ('company', 'hospital', 'clinic', 'other')),
    CHECK (test_time >= 0 AND test_time < 24),
    CHECK (test_result IN ('positive', 'negative'))
);

CREATE TABLE PositiveCase 
(
    case_id     INT NOT NULL,
    employee_id INT NOT NULL,
    case_date   DATE,
    resolution  VARCHAR(8),
    PRIMARY KEY(case_id),
    FOREIGN KEY (employee_id) REFERENCES Employee(employee_id),
    CHECK (resolution IN ('working', 'resigned', 'deceased'))
);

CREATE TABLE HealthStatus
(
    status_id   INT NOT NULL,
    employee_id INT NOT NULL,
    status_category VARCHAR(12),
    PRIMARY KEY(status_id),
    FOREIGN KEY (employee_id) REFERENCES Employee(employee_id),
    CHECK (status_category IN ('sick', 'hospitalized', 'well'))
);

INSERT INTO Employee VALUES(1213, 'Joe', 'Shmoe', 123, 1, 8203728192, 'a@email.com');
INSERT INTO Employee VALUES(1214, 'Jane', 'Doe', 125, 1, 8203728193, 'b@email.com');
INSERT INTO Employee VALUES(1215, 'Bobby', 'Brown', 126, 1, 8203728194, 'c@email.com');
INSERT INTO Employee VALUES(1216, 'Michael', 'Smith', 127, 1, 8203728195, 'd@email.com');
INSERT INTO Employee VALUES(1217, 'Jack', 'Smack', 225, 2, 8203728196, 'e@email.com');
INSERT INTO Employee VALUES(1218, 'Abby', 'Johnson', 226, 2, 8203728197, 'f@email.com');
INSERT INTO Employee VALUES(1219, 'Alex', 'Jones', 227, 2, 8203728198, 'g@email.com');
INSERT INTO Employee VALUES(1220, 'Eric', 'Garcia', 228, 2, 8203728199, 'h@email.com');
INSERT INTO Employee VALUES(1221, 'David', 'Miller', 229, 2, 8203728200, 'i@email.com');
INSERT INTO Employee VALUES(1222, 'Eric', 'Gonzales', 331, 3, 8203728201, 'j@email.com');
INSERT INTO Employee VALUES(1223, 'Adam', 'Davis', 332, 3, 8203728202, 'k@email.com');
INSERT INTO Employee VALUES(1224, 'Emily', 'Rodriguez', 333, 3, 8203728203, 'l@email.com');
INSERT INTO Employee VALUES(1225, 'Guadalupe', 'Lopez', 423, 4, 8203728204, 'm@email.com');
INSERT INTO Employee VALUES(1226, 'Heather', 'Wilson', 424, 4, 8203728205, 'n@email.com');
INSERT INTO Employee VALUES(1227, 'Gabe', 'Anderson', 425, 4, 8203728206, 'o@email.com');
INSERT INTO Employee VALUES(1228, 'Thomas', 'Thomas', 501, 5, 8203728207, 'p@email.com');
INSERT INTO Employee VALUES(1229, 'Angelina', 'Harris', 502, 5, 8203728208, 'q@email.com');
INSERT INTO Employee VALUES(1230, 'John', 'Clark', 665, 6, 8203728209, 'r@email.com');
INSERT INTO Employee VALUES(1231, 'Edward', 'Lewis', 666, 6, 8203728210, 's@email.com');
INSERT INTO Employee VALUES(1232, 'Daniel', 'Mitchell', 667, 6, 8203728211, 't@email.com');
INSERT INTO Employee VALUES(1233, 'Eric', 'Carter', 668, 6, 8203728212, 'u@email.com');
INSERT INTO Employee VALUES(1234, 'Andrew', 'Roberts', 797, 7, 8203728213, 'v@email.com');
INSERT INTO Employee VALUES(1235, 'Christian', 'Wright', 798, 7, 8203728214, 'w@email.com');
INSERT INTO Employee VALUES(1236, 'John', 'Nguyen', 799, 7, 8203728215, 'x@email.com');
INSERT INTO Employee VALUES(1237, 'Tommy', 'Hill', 856, 8, 8203728216, 'y@email.com');
INSERT INTO Employee VALUES(1238, 'Jessica', 'Flores', 857, 8, 8203728217, 'z@email.com');
INSERT INTO Employee VALUES(1239, 'Maria', 'Green', 858, 8, 8203728218, 'aa@email.com');
INSERT INTO Employee VALUES(1240, 'Adriana', 'Adams', 997, 9, 8203728219, 'bb@email.com');
INSERT INTO Employee VALUES(1241, 'Amy', 'Baker', 998, 9, 8203728220, 'cc@email.com');
INSERT INTO Employee VALUES(1242, 'Annika', 'Baker', 999, 9, 8203728221, 'dd@email.com');
INSERT INTO Employee VALUES(1243, 'Ariana', 'Baker', 1002, 10, 8203728222, 'ee@email.com');
INSERT INTO Employee VALUES(1244, 'Ariel', 'Roberts', 1003, 10, 8203728223, 'ff@email.com');
INSERT INTO Employee VALUES(1245, 'Brandon', 'Campbell', 1004, 10, 8203728224, 'gg@email.com');
INSERT INTO Employee VALUES(1246, 'Brendan', 'King', 1005, 10, 8203728225, 'hh@email.com');
INSERT INTO Employee VALUES(1247, 'Chris', 'Scott', 1006, 10, 8203728226, 'ii@email.com');

INSERT INTO Meeting VALUES(68432, 1244, 23, 2, TO_DATE('08/03/2021', 'MM/DD/YYYY'), 9, 10);
INSERT INTO Meeting VALUES(68432, 1231, 23, 2, TO_DATE('08/03/2021', 'MM/DD/YYYY'), 9, 10);
INSERT INTO Meeting VALUES(68432, 1226, 23, 2, TO_DATE('08/03/2021', 'MM/DD/YYYY'), 9, 10);
INSERT INTO Meeting VALUES(68433, 1224, 32, 3, TO_DATE('08/04/2021', 'MM/DD/YYYY'), 13, 15);
INSERT INTO Meeting VALUES(68433, 1247, 32, 3, TO_DATE('08/04/2021', 'MM/DD/YYYY'), 13, 15);
INSERT INTO Meeting VALUES(68433, 1225, 32, 3, TO_DATE('08/04/2021', 'MM/DD/YYYY'), 13, 15);
INSERT INTO Meeting VALUES(68433, 1244, 32, 3, TO_DATE('08/04/2021', 'MM/DD/YYYY'), 13, 15);
INSERT INTO Meeting VALUES(68433, 1223, 32, 3, TO_DATE('08/04/2021', 'MM/DD/YYYY'), 13, 15);
INSERT INTO Meeting VALUES(68434, 1246, 55, 5, TO_DATE('08/05/2021', 'MM/DD/YYYY'), 11, 16);
INSERT INTO Meeting VALUES(68434, 1234, 55, 5, TO_DATE('08/05/2021', 'MM/DD/YYYY'), 11, 16);
INSERT INTO Meeting VALUES(68434, 1228, 55, 5, TO_DATE('08/05/2021', 'MM/DD/YYYY'), 11, 16);
INSERT INTO Meeting VALUES(68435, 1247, 12, 1, TO_DATE('08/06/2021', 'MM/DD/YYYY'), 9, 12);
INSERT INTO Meeting VALUES(68435, 1213, 12, 1, TO_DATE('08/06/2021', 'MM/DD/YYYY'), 9, 12);
INSERT INTO Meeting VALUES(68435, 1217, 12, 1, TO_DATE('08/06/2021', 'MM/DD/YYYY'), 9, 12);
INSERT INTO Meeting VALUES(68435, 1234, 12, 1, TO_DATE('08/06/2021', 'MM/DD/YYYY'), 9, 12);
INSERT INTO Meeting VALUES(68436, 1233, 100, 10, TO_DATE('08/06/2021', 'MM/DD/YYYY'), 16, 17);
INSERT INTO Meeting VALUES(68436, 1235, 100, 10, TO_DATE('08/06/2021', 'MM/DD/YYYY'), 16, 17);
INSERT INTO Meeting VALUES(68437, 1244, 76, 7, TO_DATE('08/07/2021', 'MM/DD/YYYY'), 10, 13);
INSERT INTO Meeting VALUES(68437, 1215, 76, 7, TO_DATE('08/07/2021', 'MM/DD/YYYY'), 10, 13);
INSERT INTO Meeting VALUES(68437, 1217, 76, 7, TO_DATE('08/07/2021', 'MM/DD/YYYY'), 10, 13);

INSERT INTO Symptom VALUES(650, 1215, TO_DATE('08/03/2021', 'MM/DD/YYYY'), 1);
INSERT INTO Symptom VALUES(651, 1216, TO_DATE('08/03/2021', 'MM/DD/YYYY'), 2);
INSERT INTO Symptom VALUES(652, 1217, TO_DATE('08/03/2021', 'MM/DD/YYYY'), 1);
INSERT INTO Symptom VALUES(653, 1215, TO_DATE('08/04/2021', 'MM/DD/YYYY'), 5);
INSERT INTO Symptom VALUES(654, 1244, TO_DATE('08/04/2021', 'MM/DD/YYYY'), 4);
INSERT INTO Symptom VALUES(655, 1235, TO_DATE('08/04/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(656, 1215, TO_DATE('08/05/2021', 'MM/DD/YYYY'), 1);
INSERT INTO Symptom VALUES(657, 1216, TO_DATE('08/05/2021', 'MM/DD/YYYY'), 2);
INSERT INTO Symptom VALUES(658, 1234, TO_DATE('08/05/2021', 'MM/DD/YYYY'), 5);
INSERT INTO Symptom VALUES(659, 1215, TO_DATE('08/06/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(660, 1227, TO_DATE('08/06/2021', 'MM/DD/YYYY'), 5);
INSERT INTO Symptom VALUES(661, 1221, TO_DATE('08/06/2021', 'MM/DD/YYYY'), 2);
INSERT INTO Symptom VALUES(662, 1215, TO_DATE('08/07/2021', 'MM/DD/YYYY'), 4);
INSERT INTO Symptom VALUES(663, 1245, TO_DATE('08/07/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(664, 1233, TO_DATE('08/07/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(666, 1217, TO_DATE('08/08/2021', 'MM/DD/YYYY'), 2);
INSERT INTO Symptom VALUES(667, 1221, TO_DATE('08/08/2021', 'MM/DD/YYYY'), 5);
INSERT INTO Symptom VALUES(668, 1224, TO_DATE('08/08/2021', 'MM/DD/YYYY'), 1);
INSERT INTO Symptom VALUES(669, 1226, TO_DATE('08/08/2021', 'MM/DD/YYYY'), 2);
INSERT INTO Symptom VALUES(670, 1215, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(671, 1217, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 5);
INSERT INTO Symptom VALUES(672, 1224, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 4);
INSERT INTO Symptom VALUES(673, 1214, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 4);
INSERT INTO Symptom VALUES(674, 1232, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(675, 1234, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 2);
INSERT INTO Symptom VALUES(676, 1227, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 1);
INSERT INTO Symptom VALUES(677, 1233, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 1);
INSERT INTO Symptom VALUES(678, 1221, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 2);
INSERT INTO Symptom VALUES(679, 1240, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 4);
INSERT INTO Symptom VALUES(680, 1216, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(681, 1226, TO_DATE('08/09/2021', 'MM/DD/YYYY'), 1);
INSERT INTO Symptom VALUES(682, 1215, TO_DATE('08/10/2021', 'MM/DD/YYYY'), 1);
INSERT INTO Symptom VALUES(683, 1245, TO_DATE('08/10/2021', 'MM/DD/YYYY'), 2);
INSERT INTO Symptom VALUES(684, 1232, TO_DATE('08/10/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(685, 1240, TO_DATE('08/10/2021', 'MM/DD/YYYY'), 4);
INSERT INTO Symptom VALUES(686, 1219, TO_DATE('08/10/2021', 'MM/DD/YYYY'), 5);
INSERT INTO Symptom VALUES(687, 1215, TO_DATE('08/11/2021', 'MM/DD/YYYY'), 5);
INSERT INTO Symptom VALUES(688, 1214, TO_DATE('08/11/2021', 'MM/DD/YYYY'), 5);
INSERT INTO Symptom VALUES(689, 1216, TO_DATE('08/11/2021', 'MM/DD/YYYY'), 4);
INSERT INTO Symptom VALUES(690, 1215, TO_DATE('08/12/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(691, 1243, TO_DATE('08/12/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(692, 1215, TO_DATE('08/13/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(693, 1227, TO_DATE('08/13/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(694, 1224, TO_DATE('08/13/2021', 'MM/DD/YYYY'), 1);
INSERT INTO Symptom VALUES(695, 1215, TO_DATE('08/14/2021', 'MM/DD/YYYY'), 2);
INSERT INTO Symptom VALUES(696, 1223, TO_DATE('08/14/2021', 'MM/DD/YYYY'), 3);
INSERT INTO Symptom VALUES(697, 1234, TO_DATE('08/14/2021', 'MM/DD/YYYY'), 5);
INSERT INTO Symptom VALUES(698, 1215, TO_DATE('08/15/2021', 'MM/DD/YYYY'), 4);
INSERT INTO Symptom VALUES(699, 1243, TO_DATE('08/15/2021', 'MM/DD/YYYY'), 4);
INSERT INTO Symptom VALUES(700, 1215, TO_DATE('08/16/2021', 'MM/DD/YYYY'), 2);
INSERT INTO Symptom VALUES(701, 1214, TO_DATE('08/16/2021', 'MM/DD/YYYY'), 1);

INSERT INTO Scan VALUES(1, 1213, TO_DATE('08/15/2021', 'MM/DD/YYYY'), 9, 98);
INSERT INTO Scan VALUES(2, 1214, TO_DATE('08/15/2021', 'MM/DD/YYYY'), 9, 97);
INSERT INTO Scan VALUES(3, 1215, TO_DATE('08/15/2021', 'MM/DD/YYYY'), 10, 99);
INSERT INTO Scan VALUES(4, 1216, TO_DATE('08/15/2021', 'MM/DD/YYYY'), 9, 100);
INSERT INTO Scan VALUES(5, 1217, TO_DATE('08/18/2021', 'MM/DD/YYYY'), 12, 102);
INSERT INTO Scan VALUES(6, 1218, TO_DATE('08/18/2021', 'MM/DD/YYYY'), 9, 102);
INSERT INTO Scan VALUES(7, 1219, TO_DATE('08/18/2021', 'MM/DD/YYYY'), 9, 98);
INSERT INTO Scan VALUES(8, 1220, TO_DATE('08/18/2021', 'MM/DD/YYYY'), 9, 98);
INSERT INTO Scan VALUES(9, 1221, TO_DATE('08/21/2021', 'MM/DD/YYYY'), 9, 98);
INSERT INTO Scan VALUES(10, 1222, TO_DATE('08/21/2021', 'MM/DD/YYYY'), 9, 97);
INSERT INTO Scan VALUES(11, 1223, TO_DATE('08/21/2021', 'MM/DD/YYYY'), 9, 97);
INSERT INTO Scan VALUES(12, 1224, TO_DATE('08/21/2021', 'MM/DD/YYYY'), 9, 99);
INSERT INTO Scan VALUES(13, 1225, TO_DATE('08/21/2021', 'MM/DD/YYYY'), 10, 99);
INSERT INTO Scan VALUES(14, 1226, TO_DATE('08/25/2021', 'MM/DD/YYYY'), 9, 100);
INSERT INTO Scan VALUES(15, 1227, TO_DATE('08/25/2021', 'MM/DD/YYYY'), 9, 97);
INSERT INTO Scan VALUES(16, 1228, TO_DATE('08/25/2021', 'MM/DD/YYYY'), 11, 96);
INSERT INTO Scan VALUES(17, 1229, TO_DATE('08/25/2021', 'MM/DD/YYYY'), 9, 98);
INSERT INTO Scan VALUES(18, 1230, TO_DATE('08/25/2021', 'MM/DD/YYYY'), 9, 98);
INSERT INTO Scan VALUES(19, 1231, TO_DATE('08/25/2021', 'MM/DD/YYYY'), 9, 98);
INSERT INTO Scan VALUES(20, 1232, TO_DATE('08/25/2021', 'MM/DD/YYYY'), 9, 99);
INSERT INTO Scan VALUES(21, 1233, TO_DATE('08/26/2021', 'MM/DD/YYYY'), 9, 100);
INSERT INTO Scan VALUES(22, 1234, TO_DATE('08/26/2021', 'MM/DD/YYYY'), 9, 98);
INSERT INTO Scan VALUES(23, 1235, TO_DATE('08/26/2021', 'MM/DD/YYYY'), 9, 97);
INSERT INTO Scan VALUES(24, 1236, TO_DATE('08/26/2021', 'MM/DD/YYYY'), 11, 99);
INSERT INTO Scan VALUES(25, 1237, TO_DATE('08/27/2021', 'MM/DD/YYYY'), 9, 97);
INSERT INTO Scan VALUES(26, 1238, TO_DATE('08/27/2021', 'MM/DD/YYYY'), 9, 98);
INSERT INTO Scan VALUES(27, 1239, TO_DATE('08/27/2021', 'MM/DD/YYYY'), 10, 99);
INSERT INTO Scan VALUES(28, 1240, TO_DATE('08/27/2021', 'MM/DD/YYYY'), 9, 99);
INSERT INTO Scan VALUES(29, 1241, TO_DATE('08/27/2021', 'MM/DD/YYYY'), 9, 98);
INSERT INTO Scan VALUES(30, 1242, TO_DATE('08/27/2021', 'MM/DD/YYYY'), 9, 100);
INSERT INTO Scan VALUES(31, 1243, TO_DATE('08/27/2021', 'MM/DD/YYYY'), 10, 103);
INSERT INTO Scan VALUES(32, 1244, TO_DATE('08/27/2021', 'MM/DD/YYYY'), 9, 102);
INSERT INTO Scan VALUES(33, 1245, TO_DATE('08/27/2021', 'MM/DD/YYYY'), 8, 99);
INSERT INTO Scan VALUES(34, 1246, TO_DATE('08/27/2021', 'MM/DD/YYYY'), 9, 98);
INSERT INTO Scan VALUES(35, 1247, TO_DATE('08/27/2021', 'MM/DD/YYYY'), 9, 99);

INSERT INTO Test VALUES(10, 1230, 'company', TO_DATE('08/01/2021', 'MM/DD/YYYY'), 11, 'negative');
INSERT INTO Test VALUES(11, 1231, 'hospital', TO_DATE('08/17/2021', 'MM/DD/YYYY'), 8, 'negative');
INSERT INTO Test VALUES(12, 1232, 'hospital', TO_DATE('08/17/2021', 'MM/DD/YYYY'), 7, 'positive');
INSERT INTO Test VALUES(13, 1233, 'company', TO_DATE('08/18/2021', 'MM/DD/YYYY'), 12, 'positive');
INSERT INTO Test VALUES(14, 1234, 'company', TO_DATE('08/20/2021', 'MM/DD/YYYY'), 9, 'negative');
INSERT INTO Test VALUES(15, 1235, 'company', TO_DATE('08/22/2021', 'MM/DD/YYYY'), 17, 'negative');
INSERT INTO Test VALUES(16, 1236, 'clinic', TO_DATE('08/23/2021', 'MM/DD/YYYY'), 20, 'negative');
INSERT INTO Test VALUES(17, 1237, 'other', TO_DATE('08/24/2021', 'MM/DD/YYYY'), 13, 'negative');
INSERT INTO Test VALUES(18, 1238, 'other', TO_DATE('09/09/2021', 'MM/DD/YYYY'), 10, 'negative');
INSERT INTO Test VALUES(19, 1239, 'clinic', TO_DATE('09/13/2021', 'MM/DD/YYYY'), 7, 'negative');
INSERT INTO Test VALUES(20, 1240, 'clinic', TO_DATE('09/20/2021', 'MM/DD/YYYY'), 9, 'positive');

INSERT INTO PositiveCase VALUES(112, 1213, TO_DATE('08/17/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(113, 1214, TO_DATE('08/17/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(114, 1215, TO_DATE('08/17/2021', 'MM/DD/YYYY'), 'resigned');
INSERT INTO PositiveCase VALUES(115, 1216, TO_DATE('08/17/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(116, 1217, TO_DATE('08/17/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(117, 1218, TO_DATE('08/17/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(118, 1219, TO_DATE('08/20/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(119, 1220, TO_DATE('08/21/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(120, 1221, TO_DATE('08/22/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(121, 1222, TO_DATE('08/24/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(122, 1223, TO_DATE('08/24/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(123, 1224, TO_DATE('08/24/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(124, 1225, TO_DATE('08/24/2021', 'MM/DD/YYYY'), 'resigned');
INSERT INTO PositiveCase VALUES(125, 1226, TO_DATE('08/26/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(126, 1227, TO_DATE('08/26/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(127, 1228, TO_DATE('08/26/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(128, 1229, TO_DATE('08/28/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(129, 1230, TO_DATE('08/29/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(130, 1231, TO_DATE('08/29/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(131, 1232, TO_DATE('08/29/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(132, 1233, TO_DATE('08/29/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(133, 1234, TO_DATE('08/30/2021', 'MM/DD/YYYY'), 'deceased');
INSERT INTO PositiveCase VALUES(134, 1235, TO_DATE('08/30/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(135, 1236, TO_DATE('09/01/2021', 'MM/DD/YYYY'), 'working');
INSERT INTO PositiveCase VALUES(136, 1237, TO_DATE('09/17/2021', 'MM/DD/YYYY'), 'working');

INSERT INTO HealthStatus VALUES(76777, 1213, 'sick');
INSERT INTO HealthStatus VALUES(76778, 1214, 'well');
INSERT INTO HealthStatus VALUES(76779, 1215, 'well');
INSERT INTO HealthStatus VALUES(76780, 1216, 'well');
INSERT INTO HealthStatus VALUES(76781, 1217, 'sick');
INSERT INTO HealthStatus VALUES(76782, 1218, 'hospitalized');
INSERT INTO HealthStatus VALUES(76783, 1219, 'hospitalized');
INSERT INTO HealthStatus VALUES(76784, 1220, 'sick');
INSERT INTO HealthStatus VALUES(76785, 1221, 'well');
INSERT INTO HealthStatus VALUES(76786, 1222, 'sick');
INSERT INTO HealthStatus VALUES(76787, 1223, 'well');
INSERT INTO HealthStatus VALUES(76788, 1224, 'hospitalized');
INSERT INTO HealthStatus VALUES(76789, 1225, 'well');
INSERT INTO HealthStatus VALUES(76790, 1226, 'sick');
INSERT INTO HealthStatus VALUES(76791, 1227, 'sick');
INSERT INTO HealthStatus VALUES(76792, 1228, 'well');
INSERT INTO HealthStatus VALUES(76793, 1229, 'sick');
INSERT INTO HealthStatus VALUES(76794, 1230, 'sick');
INSERT INTO HealthStatus VALUES(76795, 1231, 'well');
INSERT INTO HealthStatus VALUES(76796, 1232, 'well');
INSERT INTO HealthStatus VALUES(76797, 1233, 'well');
INSERT INTO HealthStatus VALUES(76798, 1234, 'sick');
INSERT INTO HealthStatus VALUES(76799, 1235, 'hospitalized');
INSERT INTO HealthStatus VALUES(76800, 1236, 'sick');
INSERT INTO HealthStatus VALUES(76801, 1237, 'well');
INSERT INTO HealthStatus VALUES(76802, 1238, 'sick');
INSERT INTO HealthStatus VALUES(76803, 1239, 'hospitalized');
INSERT INTO HealthStatus VALUES(76804, 1240, 'sick');
INSERT INTO HealthStatus VALUES(76805, 1241, 'well');
INSERT INTO HealthStatus VALUES(76806, 1242, 'well');
INSERT INTO HealthStatus VALUES(76807, 1243, 'sick');
INSERT INTO HealthStatus VALUES(76808, 1244, 'sick');
INSERT INTO HealthStatus VALUES(76809, 1245, 'sick');
INSERT INTO HealthStatus VALUES(76810, 1246, 'sick');
INSERT INTO HealthStatus VALUES(76811, 1247, 'sick');
