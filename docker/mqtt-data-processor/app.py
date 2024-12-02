import paho.mqtt.client as mqtt
import sqlite3
import json

# SQLite setup
db_conn = sqlite3.connect('db/data.db', check_same_thread=False)
cursor = db_conn.cursor()
cursor.execute('''CREATE TABLE IF NOT EXISTS mqtt_data (
                    humidity REAL,
                    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
                  )''')
db_conn.commit()

# MQTT setup
BROKER = "mqtt-broker"  # Hostname of MQTT broker from Docker Compose
PORT = 1883
TOPIC = "esp32/hum"

def on_message(client, userdata, msg):
    data = msg.payload.decode();
    cursor.execute("INSERT INTO mqtt_data (humidity) VALUES (?)", (float(data),))
    db_conn.commit()
    print("Inserted successfully", data)

client = mqtt.Client()
client.on_message = on_message
client.connect(BROKER, PORT, 60)
client.subscribe(TOPIC)

client.loop_forever()