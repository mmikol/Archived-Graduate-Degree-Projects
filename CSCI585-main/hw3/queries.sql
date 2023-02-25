CREATE EXTENSION POSTGIS;

CREATE TABLE coordinate (name varchar, geom geometry);
INSERT INTO coordinate VALUES
  ('HOME', 'POINT(-118.28250 34.01674)'),
  ('GEC', 'POINT(-118.280298 34.020654)'),
  ('BKS', 'POINT(-118.286338 34.020753)'),
  ('JKP', 'POINT(-118.283034 34.019003)'),
  ('DML', 'POINT(-118.284206 34.020439)'),
  ('LVL', 'POINT(-118.282933 34.021716)'),
  ('IYH', 'POINT(-118.288265 34.018645)'),
  ('SAL', 'POINT(-118.289556 34.019390)'),
  ('SGM', 'POINT(-118.289257 34.021228)'),
  ('CFX', 'POINT(-118.288550 34.022219)'),
  ('LRC', 'POINT(-118.287912 34.024359)'),
  ('MUS', 'POINT(-118.285140 34.022996)'),
  ('SHR', 'POINT(-118.281753 34.023551)')

SELECT ST_AsText(ST_ConvexHull(ST_Collect(ST_AsText(geom)))) AS convex_hull
FROM coordinate;

SELECT name, ST_AsText(geom) AS location
FROM coordinate
WHERE name != 'HOME'
ORDER BY geom <-> (SELECT geom FROM coordinate WHERE name = 'HOME')
LIMIT 4;