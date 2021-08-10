from datetime import datetime,date
from flask import Flask
from flask import render_template,redirect,url_for,request
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy.sql import expression, func
app = Flask(__name__)

#mysql database configuration
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://root:12345678@localhost/fsr_esp32'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db = SQLAlchemy(app)

#db models
class Patient(db.Model):
    __tablename__ = 'patients'
    id = db.Column(db.Integer, primary_key=True)
    patients_fname = db.Column(db.String(15),nullable=False)
    patients_lname = db.Column(db.String(15),nullable=False)
    in_use = db.Column(db.Boolean, server_default=expression.false(),default=False, nullable=True)
    patients_id = db.Column(db.Integer,unique=True,nullable=False)
    readings = db.relationship('Reading',backref='patient',lazy=True)
    reps = db.relationship('Rep',backref='patient',lazy=True)
    
    def __repr__(self):
        return f"Patient('{self.patients_fname}','{self.patients_lname}','{self.patients_id}')"


class Rep(db.Model):
    __tablename__ = 'reps'
    id = db.Column(db.Integer,primary_key=True)
    weight = db.Column(db.Integer,nullable=False)
    reps = db.Column(db.Integer,nullable=False)
    datetime = db.Column(db.DateTime(timezone=True),nullable=False,server_default=func.now())
    patients_id = db.Column(db.Integer,db.ForeignKey('patients.patients_id'),nullable=False)

    def __repr__(self):
        return f"Reps('{self.weight}','{self.reps}','{self.datetime}')"


class Reading(db.Model):
    __tablename__ = 'readings'
    id = db.Column(db.Integer,primary_key=True)
    sensor1 = db.Column(db.Integer,nullable=False)
    sensor2 = db.Column(db.Integer,nullable=False)
    sensor3 = db.Column(db.Integer,nullable=False)
    sensor4 = db.Column(db.Integer,nullable=False)
    sensor5 = db.Column(db.Integer,nullable=False)
    sensor6 = db.Column(db.Integer,nullable=False)
    datetime = db.Column(db.DateTime(timezone=True),nullable=False,server_default=func.now())
    patients_id = db.Column(db.Integer,db.ForeignKey('patients.patients_id'),nullable=False)

    def __repr__(self):
        return f"Readings('{self.sensor1}','{self.sensor2}','{self.sensor3}','{self.sensor4}','{self.sensor5}','{self.sensor6}','{self.datetime}')"

#summy patient data to test patient cards

#home route 
@app.route("/")
@app.route("/home")
def home():
    return render_template('home.html',title='Home',year=date.today().year)

#dashboard route
@app.route("/dashboard")
def dash():
    #query all users 
    patients = Patient.query.all()
    return render_template('dashboard.html',title='Dashboard',year=date.today().year,patients=patients)

#dynamic patient data route
@app.route("/dashboard/",methods=['GET'])
def pdata():
    patient_id = request.args.get('id',default=None,type =int)
    print(patient_id)
    #get the patients readings and reps here
    #for now lets get the details and render them to the page
    patient = Patient.query.filter_by(patients_id=patient_id).first_or_404(description='There is no data for patient with id:  {}'.format(patient_id))
    reps = Rep.query.filter_by(patients_id=patient_id).order_by(Rep.datetime.desc())
    readings = Reading.query.filter_by(patients_id=patient_id).order_by(Reading.datetime.desc())
    return render_template("patient_data.html",patient=patient,reps=reps,readings=readings)


if __name__ == "__main__":
    app.run(debug=True)