import flask
from flask import Flask, render_template, redirect, request, jsonify
import colorsys

app = Flask(__name__)

# Глобальные переменные для управления LED
led_mode = 0
speed = 30
custom_color = '#FFFFFF'
sliders = {
    'hue_gap': 10,
    'hue_range': 5,
    'ripple_step': 15
}

@app.route("/", methods=["GET"])
def index():
    """
    Главная страница, где всё управление происходит на одной странице.
    Передаём текущие значения, чтобы при загрузке можно было инициализировать элементы.
    """
    return render_template("index.html",
                           led_mode=led_mode,
                           custom_color=custom_color,
                           speed=speed,
                           hue_gap=sliders['hue_gap'],
                           hue_range=sliders['hue_range'],
                           ripple_step=sliders['ripple_step'])

@app.route("/set_mode/<int:new_mode>", methods=["POST"])
def set_mode(new_mode):
    global led_mode
    led_mode = new_mode
    return jsonify({"status": "ok", "led_mode": led_mode})

@app.route("/set_speed", methods=["POST"])
def set_speed():
    global speed
    new_speed = request.json.get('speed', None)
    if new_speed is not None and str(new_speed).isdigit():
        new_speed = int(new_speed)
        if 0 <= new_speed <= 1000:
            speed = new_speed
    return jsonify({"status": "ok", "speed": speed})

@app.route("/set_color", methods=["POST"])
def set_color():
    global custom_color
    color = request.json.get('color', '#FFFFFF')
    if color.startswith('#') and len(color) == 7:
        custom_color = color
    return jsonify({"status": "ok", "custom_color": custom_color})

@app.route("/update_slider", methods=["POST"])
def update_slider():
    data = request.json
    slider_name = data.get('slider_name')
    slider_value = data.get('slider_value')
    if slider_name in sliders:
        sliders[slider_name] = slider_value
    return jsonify({"status": "ok", slider_name: sliders[slider_name]})

@app.route("/color_hsv", methods=["GET"])
def color_hsv():
    """
    Возвращаем текущий цвет в HSV.
    """
    r = int(custom_color[1:3], 16)
    g = int(custom_color[3:5], 16)
    b = int(custom_color[5:7], 16)
    hsv = list(colorsys.rgb_to_hsv(r / 255, g / 255, b / 255))
    # Переводим в привычный вид: H [0..360], S [0..100], V [0..100]
    hsv[0] = int(hsv[0] * 360)
    hsv[1] = int(hsv[1] * 100)
    hsv[2] = int(hsv[2] * 100)
    return jsonify({"h": hsv[0], "s": hsv[1], "v": hsv[2]})

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8080)
