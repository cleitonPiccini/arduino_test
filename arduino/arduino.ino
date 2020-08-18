#include <SPI.h>
#include <Ethernet.h>
#include <max6675.h>
#include <EEPROM.h>
#include "Mudbus.h"
#include "function.h"

Mudbus Mb;
//Function codes 1(read coils), 3(read registers), 5(write coil), 6(write register)
//signed int Mb.R[0 to 125] and bool Mb.C[0 to 128] MB_N_R MB_N_C
//Port 502 (defined in Mudbus.h) MB_PORT

//Pinos sensor 1.
int SO_S1 = 22;
int CS_S1 = 24;
int SKS_S1 = 26;
//Pinos sensor 2.
int SO_S2 = 28;
int CS_S2 = 30;
int SKS_S2 = 32;

Equipamento filtro;
Equipamento hidro_1;
Equipamento hidro_2;
Equipamento aquecimento;
Equipamento led;
Equipamento aux_1;
Equipamento aux_2;
Equipamento aux_3;

Timer timer_temp; // Temporizador para aquisição da temperatura.
Timer timer_update_eeprom; // Temporizador para atualizar as memórias na EEPROM.

int tempo_atual;

float temp_s1; //Váriavel utilizada para retorno da temperatura do sensor 1
float temp_s2; //Váriavel utilizada para retorno da temperatura do sensor 2

const int Reg_Piscina = 2864;
const int Reg_Coletor = 2865;

int pin_out_filtro = 34;
int pin_out_hidro_1 = 35;
int pin_out_hidro_2 = 36;
int pin_out_aquecimento = 37;
int pin_out_led = 38;
int pin_out_aux_1 = 39;
int pin_out_aux_2 = 40;
int pin_out_aux_3 = 41;


MAX6675 sensor_1(SKS_S1, CS_S1, SO_S1); //Cria uma instancia de leitura do sensor 1.
MAX6675 sensor_2(SKS_S2, CS_S2, SO_S2); //Cria uma instancia de leitura do sensor 2

void setup()
{
  uint8_t mac[]     = { 0x90, 0xA2, 0xDA, 0x00, 0x51, 0x06 };
  uint8_t ip[]      = { 192, 168, 0, 20 };
  uint8_t gateway[] = { 192, 168, 0, 1 };
  uint8_t subnet[]  = { 255, 255, 255, 0 };
  Ethernet.begin(mac, ip, gateway, subnet);
  //Avoid pins 4,10,11,12,13 when using ethernet shield

  delay(5000);
  Serial.begin(9600);

  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(41, OUTPUT);
  
  timer_temp.init();
  timer_update_eeprom.init();

  //Inicialização dos equipamentos.
  filtro.init(0,0);
  hidro_1.init(40,0);
  hidro_2.init(80,0);
  aquecimento.init(120,0);
  led.init(160,0);
  aux_1.init(200,0);
  aux_2.init(240,0);
  aux_3.init(280,0);

}

void loop()
{
  Mb.Run();
  
  filtro.control(10);

  filtro.modbus(0, Mb);
  hidro_1.modbus(15, Mb);
  hidro_2.modbus(30, Mb);
  aquecimento.modbus(45, Mb);
  aquecimento.ativa_temp.value = true; //Ativa o controle de temperatura
  led.modbus(60, Mb);
  aux_1.modbus(75, Mb);
  aux_2.modbus(90, Mb);
  aux_3.modbus(105, Mb);

  tempo_atual = Mb.R[106];
  
  filtro.control(tempo_atual);
  hidro_1.control(tempo_atual);
  hidro_2.control(tempo_atual);
  aquecimento.control(tempo_atual);
  led.control(tempo_atual);
  aux_1.control(tempo_atual);
  aux_2.control(tempo_atual);
  aux_3.control(tempo_atual);
  
  //Temperaturas.
  Mb.R[107] = float_to_int(temp_s1);
  Mb.R[108] = float_to_int(temp_s2);
  
  timer_temp.IN = true;
  timer_temp.P = 500;
  timer_temp.TOS();
  if (timer_temp.Q) {
    temp_s1 = sensor_1.readCelsius(); //Função que retorna um float com a temperatura do sensor 1.
    temp_s2 = sensor_2.readCelsius(); //Função que retorna um float com a temperatura do sensor 2.
  }

  //Saídas do Arduino.
  digitalWrite(pin_out_filtro, filtro.out_equip);
  digitalWrite(pin_out_hidro_1, hidro_1.out_equip);
  digitalWrite(pin_out_hidro_2, hidro_2.out_equip);
  digitalWrite(pin_out_aquecimento, aquecimento.out_equip);
  digitalWrite(pin_out_led, led.out_equip);
  digitalWrite(pin_out_aux_1, aux_1.out_equip);
  digitalWrite(pin_out_aux_2, aux_2.out_equip);
  digitalWrite(pin_out_aux_3, aux_3.out_equip);
  
  //Atualiza os valores na EEPROM
  timer_update_eeprom.IN = true;
  timer_update_eeprom.P = 2000;
  timer_update_eeprom.TOS();
  if (timer_update_eeprom.Q)
  {
    filtro.update_eeprom();
    hidro_1.update_eeprom();
    hidro_2.update_eeprom();
    aquecimento.update_eeprom();
    led.update_eeprom();
    aux_1.update_eeprom();
    aux_2.update_eeprom();
    aux_3.update_eeprom();
  }
}
