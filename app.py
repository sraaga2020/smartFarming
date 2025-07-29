from flask import Flask, render_template
import serial
import time

app = Flask(__name__)

# Replace with your correct port (e.g., 'COM3' on Windows)
ser = serial.Serial('/dev/cu.usbmodem14101', 9600, timeout=1)
time.sleep(2)  # Wait for Arduino to reset

@app.route('/')
def index():
    data = {
        "temp": "N/A",
        "hum": "N/A",
        "moisture": "N/A",
        "light": "N/A",
        "hour": "N/A"
    }

    # Read multiple lines until we get all required info
    lines_read = 0
    while lines_read < 6:
        if ser.in_waiting:
            line = ser.readline().decode().strip()
            lines_read += 1

            if "Temperature" in line:
                data["temp"] = line.split(":")[1].strip()
            elif "Humidity" in line:
                data["hum"] = line.split(":")[1].strip()
            elif "Moisture" in line:
                data["moisture"] = line.split(":")[1].strip()
            elif "Light" in line:
                data["light"] = line.split(":")[1].strip()
            elif "Hour" in line:
                data["hour"] = line.split(":")[1].strip()

    return render_template("index.html", **data)

if __name__ == '__main__':
    app.run(debug=True)
