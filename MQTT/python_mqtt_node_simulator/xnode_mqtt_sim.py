import time
import os
import paho.mqtt.client as mqtt  #python -m pip install paho-mqtt یا pip install paho-mqtt

BROKER = "localhost" #درصورتی که روی سیستم خودتون بروکر رو اجرا می کنین.
PORT = 1883 #اصولا پورت پیشفرض 1883 هست
DEVICE_ID = "xnode-aero-01"

TOPIC_STATUS = f"xmind/xnode/{DEVICE_ID}/status"
TOPIC_TELEMETRY = f"xmind/xnode/{DEVICE_ID}/telemetry"

client = mqtt.Client(client_id=DEVICE_ID, protocol=mqtt.MQTTv5)
client.username_pw_set("YOUR_MQTT_USER", "YOUR_MQTT_PASS") #اگه برای اتصال به بروکر نام کاربری و پسورد تنظیم کردین، اینجا وارد کنید.
# ۱. تنظیم وصیت‌نامه (LWT) قبل از اتصال به بروکر
lwt_payload = '{"state": "offline", "reason": "unexpected_crash"}'
client.will_set(TOPIC_STATUS, payload=lwt_payload, qos=1, retain=True)

print("[1] Connecting to EMQX...")
client.connect(BROKER, PORT, keepalive=10)
client.loop_start()

# ۲. ارسال پیام آنلاین شدن با Retain = True
online_payload = '{"state": "online", "firmware": "v1.0.4"}'
client.publish(TOPIC_STATUS, payload=online_payload, qos=1, retain=True)
print(f"[2] Published ONLINE status (Retain=True) to {TOPIC_STATUS}")

# ۳. ارسال داده‌های تلمتری با Retain = False
try:
    count = 1
    while True:
        telemetry_payload = f'{{"temp": {23.5 + count}, "hum": 55}}'
        client.publish(TOPIC_TELEMETRY, payload=telemetry_payload, qos=0, retain=False)
        print(f"[3] Published Telemetry #{count}")
        count += 1
        time.sleep(3)
except KeyboardInterrupt:
    print("\n[!] Simulating HARD CRASH (Force Kill)...")
    # عدم فراخوانی client.disconnect() برای تحریک LWT در بروکر
    os._exit(1)
