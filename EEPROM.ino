

void EEPROM_init(void) {
 EEPROM.begin(512);
// При первом запуске в памяти пусто
if (EEPROM.read(adr_init_eeprom)!=1)
{

  //Запись позиции шторы как открыто (100%)
  EEPROM.write(addres_position, 100);
  EEPROM.commit();
  EEPROM.write(adr_init_eeprom, 1);
  EEPROM.commit();
}

//Загрузка текущих показаний
//motor_position_abs = 18450;
COUNT_STEPS = EEPROMReadInt(address_steps);
motor_position_abs = (COUNT_STEPS * EEPROM.read(addres_position) /100);
}


void EEPROM_float_write(int addr, float val) // запись в ЕЕПРОМ FLOAT 4 bite
{  
  byte *x = (byte *)&val;
  for(byte i = 0; i < 4; i++) EEPROM.write(i+addr, x[i]);
  EEPROM.commit();
}

float EEPROM_float_read(int addr) // чтение из ЕЕПРОМ
{    
  byte x[4];
  for(byte i = 0; i < 4; i++) x[i] = EEPROM.read(i+addr);
  float *y = (float *)&x;
  return y[0];
}


void EEPROMWriteInt(int address, int value)
{
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);
  
  EEPROM.write(address, two);
  EEPROM.write(address + 1, one);
}
 
int EEPROMReadInt(int address)
{
  long two = EEPROM.read(address);
  long one = EEPROM.read(address + 1);
 
  return ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
}


void EEPROM_string_write (int addr, int addr_length, String s)
{
    int charLength=s.length();
  
  Serial.println("writing eeprom ssid:");
          for (int i = 0; i < s.length(); ++i)
            {
              EEPROM.write(i+addr, s[i]);
             // Serial.print("Wrote: ");
             // Serial.println(s[i]); 
            }
          EEPROM.write(addr_length,charLength);
         EEPROM.commit();
}




String EEPROM_string_read (int addr, int charLength)
{
 //  Serial.println("Reading EEPROM ssid");
  String s;
  for (int i = 0; i < charLength; ++i)
    {
      s += char(EEPROM.read(i+addr));
    }
    //esid.trim();
    //Serial.println(s.length());
  //Serial.print("SSID: ");
  //Serial.println(s);
  return s;
}
