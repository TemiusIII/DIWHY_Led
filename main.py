from flask import Flask, render_template, request, jsonify
from flask_sock import Sock
import json

app = Flask(__name__)
sock = Sock(app)

connected_clients = set()

STATE = { # values for esp
    "led_mode": 0,
    "speed": 30,
    "brightness": 100,
    "custom_color": 0xFFFFFF,
    "ripple": {
        'hue_gap': 10,
        'hue_range': 5,
        'ripple_step': 15
    }
}

@app.route("/", methods=["GET"])
def index():
    return render_template("index.html",
                           led_mode=STATE['led_mode'],
                           custom_color=STATE["custom_color"],
                           speed=STATE["speed"],
                           brightness=STATE["brightness"],
                           hue_gap=STATE['ripple']['hue_gap'],
                           hue_range=STATE['ripple']['hue_range'],
                           ripple_step=STATE['ripple']['ripple_step'])


@app.route("/set_mode/<int:new_mode>", methods=["POST"])
def set_mode(new_mode):
    global STATE
    STATE["led_mode"] = new_mode
    send_to_all()
    return jsonify({"status": "ok", "led_mode": STATE["led_mode"]})

@app.route("/set_speed", methods=["POST"])
def set_speed():
    global STATE
    new_speed = request.json.get('speed', None)
    if new_speed is not None and str(new_speed).isdigit():
        new_speed = int(new_speed)
        if 0 <= new_speed <= 1000:
            STATE['speed'] = new_speed
            send_to_all()
    return jsonify({"status": "ok", "speed": STATE['speed']})


@app.route("/set_brightness", methods=["POST"])
def set_brightness():
    global STATE
    new_brightness = request.json.get('brightness', None)
    if new_brightness is not None and str(new_brightness).isdigit():
        new_brightness = int(new_brightness)
        if 0 <= new_brightness <= 255:
            STATE['brightness'] = new_brightness
            send_to_all()
            print(new_brightness)
    return jsonify({"status": "ok", "brightness": STATE['brightness']})

@app.route("/set_color", methods=["POST"])
def set_color():
    global STATE
    color = request.json.get('color', '#FFFFFF')
    if color.startswith('#') and len(color) == 7:
        STATE['custom_color'] = int(color[1:], 16)
        send_to_all()
    print(f"NIGGA {color} GIGA {STATE['custom_color']}")
    return jsonify({"status": "ok", "custom_color": STATE['custom_color']})

@app.route("/update_slider", methods=["POST"])
def update_slider():
    global STATE
    data = request.json
    slider_name = data.get('slider_name')
    slider_value = data.get('slider_value')
    if slider_name in STATE['ripple']:
        STATE['ripple'][slider_name] = slider_value
        send_to_all()
    return jsonify({"status": "ok", slider_name: slider_name})


@sock.route('/ws') # websocket url
def websocket(ws):
    connected_clients.add(ws)
    try:
        ws.send(json.dumps(STATE))
        while True:
            message = ws.receive()
            if message:
                print("From ESP:", message)
    except Exception as e:
        pass
    finally:
        connected_clients.discard(ws)


def send_to_all(): # send updated values to all websocket clients
    if not connected_clients:
        return

    msg = json.dumps(STATE)
    dead_clients = []

    for client in connected_clients:
        try:
            client.send(msg)
        except:
            dead_clients.append(client)

    for dead in dead_clients:
        connected_clients.discard(dead)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8080)
