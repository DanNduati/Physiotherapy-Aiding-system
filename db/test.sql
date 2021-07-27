UPDATE patients SET in_use =1 WHERE patients_id ='123456789';
INSERT INTO reps (weight,reps,patients_id) VALUES('5','25','123456789');
INSERT INTO readings (sensor1,sensor2,sensor3,sensor4,sensor5,sensor6,patients_id) VALUES('420','69','728','54','892','204','123456789');
INSERT INTO readings (sensor1,sensor2,sensor3,sensor4,sensor5,sensor6,total_mass,patients_id) VALUES('3','7','9','8','2','6','14','313415192')