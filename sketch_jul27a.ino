#define BLYNK_TEMPLATE_ID "TMPL3QYARxHib"
#define BLYNK_TEMPLATE_NAME "Water Ph Monitoring System"
#define BLYNK_AUTH_TOKEN "VGb9OOafv-VgcEvTyxn631i87q5fEnDl"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "~subbu~";
char pass[] = "12345subbu";

#define PH_PIN 32

BlynkTimer timer;

int lowCount = 0;
int highCount = 0;

void sendPH()
{
  // Average 20 ADC readings
  long total = 0;

  for (int i = 0; i < 20; i++)
  {
    total += analogRead(PH_PIN);
    delay(10);
  }

  int adcValue = total / 20;

  float voltage = (adcValue / 4095.0) * 3.3;

  // Experimental calibration
  float phValue = 7.0 + ((voltage - 0.110) * (1.5 / 0.028));

  // Limit pH
  if (phValue < 0)
    phValue = 0;

  if (phValue > 14)
    phValue = 14;

  Serial.print("ADC: ");
  Serial.print(adcValue);

  Serial.print("  Voltage: ");
  Serial.print(voltage, 3);

  Serial.print(" V  pH: ");
  Serial.println(phValue, 2);

  // Send pH to Blynk V0
  Blynk.virtualWrite(V0, phValue);

 // Notification for abnormal pH
if (phValue < 6.5)
{
  Blynk.logEvent(
    "low_ph_alert",
    String("Low pH detected: ") + String(phValue, 2)
  );
}

if (phValue > 8.5)
{
  Blynk.logEvent(
    "ph_alert",
    String("High pH detected: ") + String(phValue, 2)
  );
}
}

void setup()
{
  Serial.begin(115200);

  analogReadResolution(12);

  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");

  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();

  timer.setInterval(2000L, sendPH);
}

void loop()
{
  Blynk.run();
  timer.run();
}