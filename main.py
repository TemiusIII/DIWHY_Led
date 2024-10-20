import flask
from flask import Flask, render_template, redirect, request
import colorsys

app = Flask(__name__)

led_mode = 0
speed = 30
custom_color = '#FFFFFF'
sliders = {
    'hue_gap': 10,
    'hue_range': 5,
    'ripple_step': 15
}


@app.route("/", methods=["GET", "POST"])
def index():
    return render_template("index.html", led_mode=led_mode, custom_color=custom_color, speed=speed,
                           hue_gap=sliders['hue_gap'], hue_range=sliders['hue_range'],
                           ripple_step=sliders['ripple_step'])


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
    new_speed = request.form['speed']

    if new_speed.isdigit() and (0 <= int(new_speed) <= 1000):
        speed = new_speed
    return redirect("/")


@app.route("/set_color", methods=["GET"])
def set_color():
    global custom_color
    custom_color = request.args.get('color')
    return "200"


@app.route('/update_slider', methods=['POST'])
def update_slider():
    data = request.json
    sliders[data['slider_name']] = data['slider_value']
    print(data['slider_name'], " = ", sliders[data['slider_name']])
    return "200"


@app.route('/color_hsv', methods=["GET", "POST"])
def color_hsv():
    r, g, b = int(custom_color[1:3], 16), int(custom_color[3:5], 16), int(custom_color[5:7], 16)
    hue_color = list(colorsys.rgb_to_hsv(r / 255, g / 255, b / 255))
    hue_color[0] = int(hue_color[0] * 360)
    hue_color[1] = int(hue_color[1] * 100)
    hue_color[2] = int(hue_color[2] * 100)
    hue_color = " ".join([str(x) for x in hue_color])
    return render_template("color_hsv.html", color_hsv=hue_color)

@app.route('/ripple_values', methods=["GET", "POST"])
def ripple_values():
    return render_template("ripple_params.html", hue_gap=sliders['hue_gap'], hue_range=sliders['hue_range'],
                           ripple_step=sliders['ripple_step'])


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8080)
