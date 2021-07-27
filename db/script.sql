CREATE TABLE patients (
    id INT AUTO_INCREMENT,
	patients_fname VARCHAR(50) NOT NULL,
	patients_lname VARCHAR(50) NOT NULL,
    in_use BOOLEAN DEFAULT 0,
    patients_id VARCHAR(9) NOT NULL UNIQUE,
    PRIMARY KEY (id)
) ENGINE=INNODB;

CREATE TABLE readings(
    id INT AUTO_INCREMENT PRIMARY KEY,
	sensor1 INT NOT NULL,
    sensor2 INT NOT NULL,
    sensor3 INT NOT NULL,
    sensor4 INT NOT NULL,
    sensor5 INT NOT NULL,
    sensor6 INT NOT NULL,
    total_mass INT NOT NULL,
	date_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    patients_id VARCHAR(9) NOT NULL,
    INDEX par_ind (patients_id),
    FOREIGN KEY (patients_id)
        REFERENCES patients(patients_id)
        ON DELETE CASCADE
) ENGINE=INNODB;

CREATE TABLE reps(
    id INT AUTO_INCREMENT PRIMARY KEY,
    weight INT NOT NULL,
    reps INT NOT NULL,
    date_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    patients_id VARCHAR(9) NOT NULL,
    INDEX par_ind (patients_id),
    FOREIGN KEY (patients_id)
        REFERENCES patients(patients_id)
        ON DELETE CASCADE
) ENGINE=INNODB;
