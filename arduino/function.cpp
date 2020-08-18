#include "function.h"

int float_to_int (float var)
{
  return (int)(var * 10);
}

//#################### EQUIPAMENTOS #############################
//Função para controle do equipamento.
void Equipamento :: control (int min_atual) 
{
  // Verifica o manual e automatico do equipamento. 
  // Verifica o interlock do equipamento.
  // Verifica o acionamento agendado.
  bool agenda;
  if (!this->habilita_agenda.value)
  {
    agenda = true;
  } else if ( min_atual >= this->p_total_min_ini.value && min_atual <= this->p_total_min_fim.value && this->active_agenda) {
    agenda = true;
  } else {
    agenda = false;
  }

  if (this->man_auto && !this->interlock && agenda)
  {
    this->out_equip = true;
  } else {
    // Verifica se o comando ligar manual está ativo.
    if (this->liga_man)
    {
      this->out_equip = true;
    } else {
      this->out_equip = false;  
    }
  }
}
//Função para iniciar o equipamento (iniciar as memórias).
void Equipamento :: init (int adr_base, int reg_base)
{
  this->liga_man = false;
  this->interlock = false;
  this->active_agenda = false;
  this->out_equip = false;
  //Inicia os endereços da memória EEPROM.
  this->init_adr(adr_base);
  //Apartir daqui deve ser implementada a função para pegar dados da eeprom.
  // Carrega valores da EEPROM (memórias da temperatura).
  this->ativa_temp.load_eeprom();
  this->setpoint.load_eeprom();
  this->temp_inicio.load_eeprom();
  this->temp_fim.load_eeprom();
  // Carrega valores da EEPROM (memórias da agenda).
  this->habilita_agenda.load_eeprom();
  this->domingo.load_eeprom();
  this->segunda.load_eeprom();
  this->terca.load_eeprom();
  this->quarta.load_eeprom();
  this->quinta.load_eeprom();
  this->sexta.load_eeprom();
  this->sabado.load_eeprom();
  this->p_hora_ini.load_eeprom();
  this->p_hora_fim.load_eeprom();
  this->p_min_ini.load_eeprom();
  this->p_min_fim.load_eeprom();
  this->p_total_min_ini.load_eeprom();
  this->p_total_min_fim.load_eeprom();
  //Inicia os registradores modbus.
  //this->init_reg(reg_base);
}
//Função para iniciar os endereços EEPROM do equipamento (apartir de um endereço base).
void Equipamento :: init_adr (int adr_base)
{
  int adr = adr_base;
  // Seta o endereço EEPROM (memórias da temperatura).
  this->ativa_temp.adr = adr;
  adr+=2;
  this->setpoint.adr = adr;
  adr+=2;
  this->temp_inicio.adr = adr;
  adr+=2;
  this->temp_fim.adr = adr;
  adr+=2;
  // Seta o endereço EEPROM (memórias da agenda).
  this->habilita_agenda.adr = adr;
  adr+=2;
  this->domingo.adr = adr;
  adr+=2;
  this->segunda.adr = adr;
  adr+=2;
  this->terca.adr = adr;
  adr+=2;
  this->quarta.adr = adr;
  adr+=2;
  this->quinta.adr = adr;
  adr+=2;
  this->sexta.adr = adr;
  adr+=2;
  this->sabado.adr = adr;
  adr+=2;
  this->p_hora_ini.adr = adr;
  adr+=2;
  this->p_hora_fim.adr = adr;
  adr+=2;
  this->p_min_ini.adr = adr;
  adr+=2;
  this->p_min_fim.adr = adr;
  adr+=2;
  this->p_total_min_ini.adr = adr;
  adr+=2;
  this->p_total_min_fim.adr = adr;
}
//Função para atualizar os valores na EEPROM.
void Equipamento :: update_eeprom ()
{
  // Atualiza os valores da EEPROM (memórias da temperatura).
  this->ativa_temp.write_eeprom();
  this->setpoint.write_eeprom();
  this->temp_inicio.write_eeprom();
  this->temp_fim.write_eeprom();
  // Atualiza os valores da EEPROM (memórias da agenda).
  this->habilita_agenda.write_eeprom();
  this->domingo.write_eeprom();
  this->segunda.write_eeprom();
  this->terca.write_eeprom();
  this->quarta.write_eeprom();
  this->quinta.write_eeprom();
  this->sexta.write_eeprom();
  this->sabado.write_eeprom();
  this->p_hora_ini.write_eeprom();
  this->p_hora_fim.write_eeprom();
  this->p_min_ini.write_eeprom();
  this->p_min_fim.write_eeprom();
  this->p_total_min_ini.write_eeprom();
  this->p_total_min_fim.write_eeprom();
}

void Equipamento :: modbus (int reg_base, Mudbus Mb)
{
  int reg_base_init = reg_base;
  
  //Registradores tipo bool.
  //Memórias para controle. Valores atribuidos pela IHM
  this->man_auto = Mb.C[reg_base];
  reg_base++;
  this->liga_man = Mb.C[reg_base];
  reg_base++;
  this->active_agenda = Mb.C[reg_base];
  reg_base++;
  //Memórias para função agenda. Valores atribuidos pela IHM
  this->habilita_agenda.value = Mb.C[reg_base];
  reg_base++;
  this->domingo.value = Mb.C[reg_base];
  reg_base++;
  this->segunda.value = Mb.C[reg_base];
  reg_base++;
  this->terca.value = Mb.C[reg_base];
  reg_base++;
  this->quarta.value = Mb.C[reg_base];
  reg_base++;
  this->quinta.value = Mb.C[reg_base];
  reg_base++;
  this->sexta.value = Mb.C[reg_base];
  reg_base++;
  this->sabado.value = Mb.C[reg_base];
  reg_base++;
  //Memórias para função temperatura. Valores atribuidos pela IHM
  this->ativa_temp.value = Mb.C[reg_base];

  //Registradores tipo word.
  //Memórias para função agenda. Valores atribuidos pela IHM
  reg_base = reg_base_init; 
  this->p_hora_ini.value = Mb.R[reg_base];
  reg_base++;
  this->p_hora_fim.value = Mb.R[reg_base];
  reg_base++;
  this->p_min_ini.value = Mb.R[reg_base];
  reg_base++;
  this->p_min_fim.value = Mb.R[reg_base];
  reg_base++;
  this->p_total_min_ini.value = Mb.R[reg_base];
  reg_base++;
  this->p_total_min_fim.value = Mb.R[reg_base];
  reg_base++;
  //Memórias para função temperatura. Valores atribuidos pela IHM
  this->setpoint.value = Mb.R[reg_base];
  reg_base++;
  this->temp_inicio.value = Mb.R[reg_base];
  reg_base++;
  this->temp_fim.value = Mb.R[reg_base];
}

//#################### TEMPORIZADORES #############################
//Inicia o temporizador.
void Timer :: init ()
{
  this->C = millis();
  this->Q = false;
  this->IN = false;
  this->up_IN = false;
}
//Função para temprizador ciclico.
void Timer :: TOS ()
{
  if (this->IN && millis() >= this->C + this->P)
  {
    this->C = millis();
    this->Q = true;
  } else {
    this->Q = false;
  }
}
//Função para temprizador com retardo de tempo para ligar a saída Q.
void Timer :: TON ()
{
  
  if (this->IN && !this->up_IN)
  {
    this->C = millis();
    this->up_IN = true;
  } else {
    this->up_IN = false;
  }

  if (this->IN && millis() >= this->C + this->P)
  {
    this->Q = true;
  } else {
    this->Q = false;
  }
}
//Função para temprizador com retardo de tempo para desligar a saída Q.
void Timer :: TOF ()
{
  if (this->IN)
  {
    this->C = millis();
    this->Q = true;
  } else if (millis() <= this->C + this->P){
    this->Q = true;
  } else {
    this->Q = false;
  }
}


//#################### EEPROM #############################
//Le os valores da EEPROM (Le um byte e converte para um bool) / (utiliza um byte inteiro para apenas um bool)
void RetainBool :: load_eeprom ()
{
  byte aux_value = EEPROM.read(this->adr);

  if (aux_value > 0)
  {
    this->value = true;
    this->old_value = true;  
  } else {
    this->value = false;
    this->old_value = false;  
  }
}
//Escreve valores na EEPROM (Converte de bool para byte) / (utiliza um byte inteiro para apenas um bool)
//Escreve na EEPROM apenas se o valor atual for diferem do ultimo valor carregado na EEPROM.
void RetainBool :: write_eeprom ()
{
  if (this->value != this->old_value)
  {
    this->old_value = this->value;
    if (this->value)
    {
      EEPROM.write(this->adr,1);
    } else {
      EEPROM.write(this->adr,0);
    }
  }

}

//Le os valores da EEPROM (Le dois byte e converte para um int) / (Um int no arduino possui 2 bytes)
void RetainInt :: load_eeprom ()
{
  //Faz a leitura de dois bytes separadamente na EEPROM
  byte aux_value_1 = EEPROM.read(this->adr);
  byte aux_value_2 = EEPROM.read(this->adr + 1);

  //Faz a concatenação dos dois bytes lidos da EEPROM e atribuio o valor concatenado para um int
  this->value = 0; 
  this->value = this->value + (aux_value_1 & 0x000000ff);
  this->value = this->value + ((aux_value_2 & 0x000000ff) << 8);
  this->old_value = this->value;
}

//Escreve valores na EEPROM (Converte um int em dois bytes) / (Um int no arduino possui 2 bytes)
void RetainInt :: write_eeprom ()
{
  if (this->value != this->old_value)
  {
    this->old_value = this->value;
    byte aux_value_1 = this->value & 0x000000ff;
    byte aux_value_2 = this->value  >> 8; 
    EEPROM.write(this->adr, aux_value_1);
    EEPROM.write(this->adr + 1, aux_value_2);
  }
}