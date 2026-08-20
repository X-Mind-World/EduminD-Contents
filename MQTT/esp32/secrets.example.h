#pragma once

// =================================================================
// INSTRUCTIONS:
// Copy this file and rename it to secrets.h.
// Then replace the values below with your actual network credentials.
// =================================================================
// این فایل رو کپی کنین و اسمش رو به secrets.h تغییر بدید.
// بعد مقادیر زیر رو با اطلاعات واقعی شبکه خودتون جایگزین کنید.
// =================================================================

namespace Secrets {
    inline constexpr char WIFI_SSID[] = "YOUR_WIFI_SSID_HERE";
    inline constexpr char WIFI_PASS[] = "YOUR_WIFI_PASSWORD_HERE";

    inline constexpr char MQTT_BROKER[] = "MQTT_BROKER_ADDRESS";
    inline constexpr int MQTT_PORT = 1883;      
    inline constexpr char MQTT_USER[] = "YOUR_MQTT_USERNAME";
    inline constexpr char MQTT_PASS[] = "YOUR_MQTT_PASSWORD";
}
