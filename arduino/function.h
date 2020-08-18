#include <Arduino.h>
#include <EEPROM.h>
#include "Mudbus.h"

#ifndef function_h

class RetainBool
{
  public:
    int adr; // Endereço da memória EEPROM - use apenas números pares
    bool value; // Valor do boleano.
    bool old_value; // Último valor atribuido a memória EEPROM.

    void load_eeprom ();
    void write_eeprom ();
};

class RetainInt
{
  public:
    int adr; // Endereço da memória EEPROM - use apenas números pares
    int value; // Valor do interio.
    int old_value; // Último valor atribuido a memória EEPROM.
    //int reg_modbus; // Endereço do registrador modbus.

    void load_eeprom ();
    void write_eeprom ();  
};

class Equipamento
{
public:
  //Memórias para controle.
  bool man_auto; //Indica manuel e automatico.
  bool liga_man; //Aciona de forma manual.
  bool interlock; //Trava externa.
  bool out_equip; //Saída para acionar o equipamento.
  bool active_agenda;
  int adr_base;
  int reg_base;
  //Memórias para função agenda. Valores atribuidos pela IHM e Salvos na EEPROM do arduino.
  RetainBool habilita_agenda;
  RetainBool domingo, segunda, terca, quarta, quinta, sexta, sabado;
  RetainInt p_hora_ini, p_hora_fim; //parametros da hora de inicio e fim.
  RetainInt p_min_ini, p_min_fim; //parametros de minuto de inicio e fim.
  RetainInt p_total_min_ini, p_total_min_fim; //parametros de inicio e fim ( hora e minutos convertidos para base de minutos).
  //Memórias para função temperatura. Valores atribuidos pela IHM e Salvos na EEPROM do arduino.
  RetainBool ativa_temp;
  RetainInt setpoint, temp_inicio, temp_fim;

  // void agenda ();
  void control (int min_atual);
  void init (int adr_base, int reg_base);
  void init_adr (int adr_base);
  void update_eeprom ();
  void modbus (int reg_base, Mudbus Mb);
};

class Timer
{
  public : 
  bool Q; //Saída do temporizador.
  bool IN; //Ativa a contagem de tempo.
  bool up_IN; //Flanco positivo da ativação de contagem de tempo.
  unsigned long int C; //Tempo corrente (passar o valor da função millis())
  long int P; //Tempo para acionar a saída do temporizador

  void init ();
  void TOS ();
  void TON ();
  void TOF ();
};

struct _reg_modbus
{
  int i;
}typedef reg_modbus;

reg_modbus read_values_modbus (reg_modbus registradores);

int float_to_int (float var);
#endif
