#define AVG_CYCLES              100
#define REF_VOLTAGE             5.0
#define TRANSIMPEDANCE_GAIN     10000000
#define VOLTAGE_GAIN            4.3
#define RESPONSIVITY            0.04
#define DIE_SIZE                0.00076
#define CONSTANT (VOLTAGE_GAIN*TRANSIMPEDANCE_GAIN*RESPONSIVITY*DIE_SIZE)

void setup()
{
    Serial.begin(57600);
    Serial.println("NukeBox UV-C Intensity Measurement");
}

void loop()
{   
    uint16_t sensorVal;
    uint32_t sensorSum = 0;
    float sensorAvg, sensorVoltage, intensity;

    // average ADC values over many readings
    for (int i=0; i<AVG_CYCLES; i++)
    {
        sensorVal = analogRead(A0);
        sensorSum += sensorVal;
        delay(2);
    }
    sensorAvg = (float)sensorSum / (float)AVG_CYCLES;

    // convert ADC value to voltage
    sensorVoltage = sensorAvg * (REF_VOLTAGE / 1024.0);
    Serial.print("Sensor Voltage: ");
    Serial.print(sensorVoltage); 
    Serial.print("V, Intensity");    

    /* 
     * Intensity is calculated based on voltage
     * totalCurrent = voltage / (VOLTAGE_GAIN * TRANSIMPEDANCE_GAIN) 
     * totalPower = totalCurrent / RESPONSIVITY 
     * intensity = totalPower / DIE_SIZE
     * 
     * For  reference
     * VOLTAGE_GAIN = 4.3 and comes from the circuit configuration
     * TRANSIMPEDANCE_GAIN = 10x10^6 and comes from the feedback resistor on the transimpedance amplifier
     * RESPONSIVITY = 0.04 A/W at 260 nm and comes from the GUVA-S12SD datasheet
     * DIE_SIZE = 0.076 mm^2 = 0.00076 cm^2 and comes from the GUVA-S12SD datasheet
     */

    // power density is in mW/cm^2
    intensity = sensorVoltage * 1000.0 / CONSTANT; 
    Serial.print(intensity); 
    Serial.println();       
}
