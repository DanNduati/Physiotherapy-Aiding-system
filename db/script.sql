CREATE TABLE patients (
    id INT NOT NULL,
	patients_fname VARCHAR(50) NOT NULL,
	patients_lname VARCHAR(50) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=INNODB;

CREATE TABLE readings(
    id INT AUTO_INCREMENT PRIMARY KEY,
	reading INT NOT NULL,
	reps INT,
	date_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    patients_id INT NOT NULL,
    INDEX par_ind (patients_id),
    FOREIGN KEY (patients_id)
        REFERENCES patients(id)
        ON DELETE CASCADE
) ENGINE=INNODB;
