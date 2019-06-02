#include <LiquidCrystal.h> 

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int elapsed = 0;
bool isOn = false;
long consumedWs = 0;

byte P_ON_POT = A1;
byte P_OFF_POT = A0;
byte P_REL = 6;
byte P_NP = 7;

long MAX_POWER = 1600;
int TIME_MULTIPLYER = 60;
double PRICE_KWH = 1.769;

void setup() {
	lcd.begin(20, 4); //inicializace displeje 
	pinMode(P_REL, OUTPUT);
	pinMode(P_NP, INPUT_PULLUP);
}

void loop()
{
	int onTime = analogRead(P_ON_POT) / 90;
	int offTime = analogRead(P_OFF_POT) / 90;
	bool isNP = digitalRead(P_NP) == LOW;

	double actPower = (double)MAX_POWER / 2;
	if (onTime + offTime > 0)
		actPower = ((double)onTime / (onTime + offTime)) * MAX_POWER;

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(onTime);
	lcd.print(":");
	lcd.print(offTime);

	lcd.setCursor(10, 0);
	if (isNP)
	{
		if (onTime + offTime > 0)
			lcd.print(actPower, 0);
		lcd.print("W");
	}
	else
	{
		lcd.print("OFF");
	}

	double consumedKwh = ((double)consumedWs / 1000) / 3600;
	lcd.setCursor(0, 1);
	lcd.print(consumedKwh, 0);
	lcd.print("kWh");

	lcd.setCursor(10, 1);
	lcd.print(consumedKwh * PRICE_KWH, 0);
	lcd.print("Kc");

	if (isNP)
	{
		if ((isOn && elapsed >= onTime * TIME_MULTIPLYER && offTime > 0) || (!isOn && elapsed >= offTime * TIME_MULTIPLYER && onTime > 0))
		{
			elapsed = 0;
			isOn = !isOn;
		}

		if (isOn)
		{
			digitalWrite(P_REL, LOW);   // topi
			consumedWs += MAX_POWER;
		}
		else
		{
			digitalWrite(P_REL, HIGH);
		}

		elapsed++;
	}
	else
	{
		digitalWrite(P_REL, HIGH);
	}

	delay(1000);
}
