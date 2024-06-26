# 정적 라우팅
from flask import Flask

app = Flask(__name__)

@app.route("/")
def hello():
  return "Hello World!!"

@app.route("/name")
def name():
  return "<h1>my name is MSG</h1>"

@app.route("/age")
def age():
  return "<h1> 25 year's old</h1>"

if __name__=="__main__":
  app.run(host="0.0.0.0", port=10800, debug=True)
