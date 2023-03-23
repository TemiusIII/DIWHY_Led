from flask import Flask, render_template, redirect, request

app = Flask(__name__)

led_mode = 0
speed = 30
custom_color = '#FFFFFF'


@app.route("/", methods=["GET", "POST"])
def index():
    return render_template("index.html", led_mode=led_mode, custom_color=custom_color, speed=speed)


@app.route("/led_mode", methods=["GET", "POST"])
def check_mode():
    return render_template("check.html", led_mode=led_mode)


@app.route("/color", methods=["GET", "POST"])
def color_mode():
    return render_template("color.html", custom_color=custom_color[1::].upper())


@app.route("/speed", methods=["GET", "POST"])
def speed_mode():
    return render_template("speed.html", speed=speed)


@app.route("/set_mode/<int:new_mode>", methods=["GET", "POST"])
def set_mode(new_mode):
    global led_mode
    led_mode = new_mode
    return redirect("/")


@app.route("/set_speed", methods=["GET", "POST"])
def set_speed():
    global speed
    new_speed = request.form['title']

    if new_speed.isdigit() and (0 <= int(new_speed) <= 1000):
        speed = new_speed
    return redirect("/")


@app.route("/set_color", methods=["GET"])
def set_color():
    global custom_color
    custom_color = request.args.get('color')
    return "i'm gay"


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8080)
