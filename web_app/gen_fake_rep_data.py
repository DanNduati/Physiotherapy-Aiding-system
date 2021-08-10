from random import randint
import sys
from run import db,Patient,Rep,Reading

def gen_fake_data(n):
	for i in range(n):
		rep = Rep(weight=randint(5,40),reps=randint(1,20),patients_id=Patient.query.first().patients_id)
		reading = Reading(sensor1=randint(0,10),sensor2=randint(0,10),sensor3=randint(0,10),sensor4=randint(0,10),sensor5=randint(0,10),sensor6=randint(0,10),patients_id=Patient.query.first().patients_id)
		db.session.add(rep)
		db.session.add(reading)
	db.session.commit()
	print(f'Added {n} fake reps and readings records to the db')
if __name__== '__main__':
	if len(sys.argv) <=1:
		print('Pass the number of reps and readings you want to add to the database.')
		sys.exit(1)
	gen_fake_data(int(sys.argv[1]))