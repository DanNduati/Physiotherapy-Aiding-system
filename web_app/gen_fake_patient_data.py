from random import randint
import sys
from faker import Faker
from run import db,Patient,Rep,Reading

def gen_fake_data(n):
	faker = Faker()
	for i in range(n):
		pat = Patient(patients_fname=faker.first_name(),patients_lname=faker.last_name(),patients_id=randint(100000000,999999999))
		db.session.add(pat)

	db.session.commit()
	print(f'Added {n} fake patients to the db')
if __name__== '__main__':
	if len(sys.argv) <=1:
		print('Pass the number of patients you want to add to the database.')
		sys.exit(1)
	gen_fake_data(int(sys.argv[1]))