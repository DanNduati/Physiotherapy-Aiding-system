CREATE TABLE patients (
    patients_fname VARCHAR(100),
	patients_lname VARCHAR(100),
	patients_id INT PRIMARY KEY
);

CREATE TABLE readings(
	reading_id INT AUTO_INCREMENT PRIMARY KEY,
	reading INT,
	patients_id INT	FOREIGN KEY REFERENCES patients(patients_id),
	date_time DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE reps(
	rep_id INT AUTO_INCREMENT PRIMARY KEY,
	rep INT,
	patients_id INT FOREIGN KEY REFERENCES patients(patients_id)
);
