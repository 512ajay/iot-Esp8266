
import paho.mqtt.client as mqtt
mqtt_username = "server username"
mqtt_password = "server password"
mqtt_broker_ip = "server ip address or name"

mqtt_topic = "topic/topic1"
mqtt_topic_led = "topic/topic2"

client = mqtt.Client()

client.username_pw_set(mqtt_username, mqtt_password)

def on_connect(client, userdata, flags, rc):
    print ("Connected!", str(rc))
    client.subscribe(mqtt_topic)
    client.subscribe(mqtt_topic_led)
    client.publish(mqtt_topic_publish) 
    
def on_message(client, userdata, msg):
    print ("Topic: ", msg.topic + "\nMessage: " + str(msg.payload))
def on_log(client, userdata, level, buf):
    print("log: ",buf)

client.on_log=on_log
client.on_connect = on_connect
client.on_message = on_message
client.connect(mqtt_broker_ip, 1883)
client.loop_forever()
