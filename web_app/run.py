from datetime import date
from flask import Flask
from flask import render_template,redirect,url_for

app = Flask(__name__)

#home route 
@app.route("/")
@app.route("/home")
def home():
    return render_template('home.html',title='Home',year=date.today().year)

#dashboard route


#dynamic patient data route

if __name__ == "__main__":
    app.run(debug=True)