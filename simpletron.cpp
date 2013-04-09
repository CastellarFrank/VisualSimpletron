#include "simpletron.h"

Simpletron::Simpletron(int cant)
{
    memory=new QString[cant];
    this->size_memory=cant;
    this->reset();
}
void Simpletron::clear_memory(){
    for(int i=0;i<size_memory;i++){
        memory[i]='0';
    }
    this->totalInstrucciones=0;
}
void Simpletron::reset(){
    this->counter=0;
    this->clear_memory();
    this->texto="";
    this->acumulador=0;
    this->comando="+000";
    this->operators=0;
    this->instruction=0;
    this->totalInstrucciones=0;
    this->infoMemoria="";
}

void Simpletron::separar(QString palabra){
    QString val1=palabra,val2;
    val1.remove(0,1);
    val2=val1;
    this->instruction=val1.remove(2,2).toInt();
    this->operators=val2.remove(0,2).toInt();
}
bool Simpletron::verificarPalabra(QString palabra){
    if(!(palabra.length()==5 && (palabra.at(0)=='-' || palabra.at(0)=='+'))){
        return false;
    }else{
        for(int i=1;i<palabra.length();i++){
            if(palabra.at(i)< '0' || palabra.at(i)>'9'){
                return false;
            }
        }
        return true;
    }
}
bool Simpletron::validarInstruccion(int ins){
    if(ins==10 || ins==11 || ins==20  || ins==21 || ins==30 || ins==31  || ins==32  || ins==33
       || ins==40 || ins==41  || ins==42  || ins==43 ){
        return true;
    }
    return false;
}

void Simpletron::ingresar(QString palabra){
    if(palabra.at(0)!='-'){
        memory[this->totalInstrucciones++]=palabra;
    }
}
bool Simpletron::leerArchivo(QString path){
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }
    QTextStream read(&file);
    QString val;
    while(!read.atEnd()){
        val=read.readLine();
        if(this->verificarPalabra(val)){
            this->ingresar(val);
        }
    }
    file.close();
    return true;
}


Simpletron::~Simpletron()
{
    delete[] memory;
}
int Simpletron::getCounter()const{
    return this->counter;
}
int Simpletron::getInstruction()const{
    return this->instruction;
}
int Simpletron::getOperator() const{
    return this->operators;
}

int Simpletron::getAcumulador()const{
    return this->acumulador;
}
int Simpletron::ejecutarInstruccion(){
    if(this->instruction==10){
        return 1;
    }else if(this->instruction==11){
        return 2;
    }else if(this->instruction==20){
        this->cargar();
        return 0;
    }else if(this->instruction==21){
        this->almacenar();
        return 0;
    }else if(this->instruction==30){
        this->sumar();
        return 0;
    }else if(this->instruction==31){
        this->restar();
        return 0;
    }else if(this->instruction==32){
        this->dividir();
        return 0;
    }else if(this->instruction==33){
        this->multiplicar();
        return 0;
    }else if(this->instruction==40){
        this->bifurcar();
    }else if(this->instruction==41){
        this->bifurcarNegativo();
        return 0;
    }else if(this->instruction==42){
        this->bifucarCero();
        return 0;
    }else if(this->instruction==43){
        this->alto();
        return 3;
    }
    return 0;
}
int Simpletron::ejecutarFuncion(int pos){
    this->comando=this->memory[pos];
    this->separar(this->comando);
    int valor=this->ejecutarInstruccion();
    this->counter++;
    return valor;
}
void Simpletron::ingresarPalabra(QString palabra){
    char val[10];
    this->memory[this->operators]=palabra;
    this->texto+="- Se ingreso la palabra: ";
    texto+=palabra;
    texto+=" en la ubicacion ";
    texto+=itoa(this->operators,val,10);
    texto+=" de la memoria.\n";
    texto+="----------------------------------------\n";
}
QString Simpletron::obtenerPalabra(){
    char val[10];
    this->texto+="- Se imprimio la palabra: ";
    texto+=memory[this->operators];
    texto+=" almacenada en la ubicacion ";
    texto+=itoa(this->operators,val,10);
    texto+=" de la memoria.\n";
    texto+="----------------------------------------\n";
    this->infoMemoria+="- Impresion de palabra: ";
    this->infoMemoria+=this->memory[this->operators];
    this->infoMemoria+=" de la ubicacion ";
    infoMemoria+=itoa(this->operators,val,10);
    this->infoMemoria+=" de la memoria.\n";
    infoMemoria+="----------------------------------------\n";
    return infoMemoria;

}
void Simpletron::cargar(){
    char val[10];
    this->acumulador=this->memory[this->operators].toInt();
    this->texto+="- Se cargo la palabra ";
    texto+=memory[this->operators];
    texto+=" de la ubicacion ";
    texto+=itoa(this->operators,val,10);
    texto+=" de la memoria, en el acumulador.\n";
    texto+="----------------------------------------\n";
}
void Simpletron::almacenar(){
    char val[10];
    this->memory[this->operators]=itoa(this->acumulador,val,10);
    this->texto+="- Se almaceno la palabra ";
    texto+=itoa(this->acumulador,val,10);
    texto+=" del acumulador, en la ubicacion ";
    texto+=itoa(this->operators,val,10);
    texto+=" de la memoria.\n";
    texto+="----------------------------------------\n";
}
void Simpletron::sumar(){
    char val[10];
    int temp=this->acumulador;
    this->acumulador=this->memory[this->operators].toInt()+temp;
    this->texto+="- Se sumo la palabra ";
    texto+=this->memory[this->operators];
    texto+=" de la ubicacion ";
    texto+=itoa(this->operators,val,10);
    texto+=" de la memoria, junto con la palabra ";
    texto+=itoa(temp,val,10);
    texto+=" del acumulador, cuyo resultado se coloco en el acumulador.\n";
    texto+="----------------------------------------\n";
}
void Simpletron::restar(){
    char val[10];
    int temp=this->acumulador;
    this->acumulador=temp-this->memory[this->operators].toInt();
    this->texto+="- Se le resto a la palabra ";
    texto+=itoa(temp,val,10);
    texto+=" del acumulador, la palabra ";
    texto+=this->memory[this->operators];
    texto+=" que se encuentra en la ubicacion";
    texto+=itoa(this->operators,val,10);
    texto+=" de la memoria, cuyo resultado se coloco en el acumulador.\n";
    texto+="----------------------------------------\n";
}
void Simpletron::dividir(){
    char val[10];
    int temp=this->acumulador;
    this->acumulador=temp/this->memory[this->operators].toInt();
    this->texto+="- Se le dividio a la palabra ";
    texto+=itoa(temp,val,10);

    texto+=" del acumulador, la palabra ";
    texto+=this->memory[this->operators];
    texto+=" que se encuentra en la ubicacion";
    texto+=itoa(this->operators,val,10);
    texto+=" de la memoria, cuyo resultado se coloco en el acumulador.\n";
    texto+="----------------------------------------\n";
}
void Simpletron::multiplicar(){
    char val[10];
    int temp=this->acumulador;
    this->acumulador=temp*this->memory[this->operators].toInt();
    this->texto+="- Se le multiplico a la palabra ";
    texto+=itoa(temp,val,10);
    texto+=" del acumulador, la palabra ";
    texto+=this->memory[this->operators];
    texto+=" que se encuentra en la ubicacion";
    texto+=itoa(this->operators,val,10);
    texto+=" de la memoria, cuyo resultado se coloco en el acumulador.\n";
    texto+="----------------------------------------\n";
}
int Simpletron::getTotalInstrucciones()const{
    return this->totalInstrucciones;
}
void Simpletron::bifurcar(){
    this->counter=this->operators;
    char val[10];
    this->texto+="- Se bifurco a la instruccion de la posicion ";
    texto+=itoa(this->operators,val,10);
    texto+=" de la memoria.\n";
    texto+="----------------------------------------\n";
}
void Simpletron::bifurcarNegativo(){
    if(this->acumulador<0){
        this->counter=this->operators-1;
        char val[10];
        this->texto+="- Se bifurco a la instruccion de la posicion ";
        texto+=itoa(this->operators,val,10);
        texto+=" de la memoria, debido a que el valor del acumulador era negativo.\n";
        texto+="----------------------------------------\n";
    }else{
        texto+="- Proceso de bifurcacion fallido, debido a que el valor del acumulador no era un valor negativo.\n";
        texto+="----------------------------------------\n";
    }
}
void Simpletron::bifucarCero(){
    if(this->acumulador==0){
        this->counter=this->operators-1;
        char val[10];
        this->texto+="- Se bifurco a la instruccion de la posicion ";
        texto+=itoa(this->operators,val,10);
        texto+=" de la memoria, debido a que el numero del acumulador es igual a 0.\n";
        texto+="----------------------------------------\n";
    }else{
        texto+="- Proceso de bifurcacion fallido, debido a que el valor del acumulador no era igual a 0.\n";
        texto+="----------------------------------------\n";
    }

}
void Simpletron::alto(){
    texto+="- EL PROGRAMA COMPLETO SU TAREA -\n";
}
bool Simpletron::guardarInstrucciones(QString path){
    QFile file(path);
    if(file.open(QIODevice::WriteOnly| QIODevice::Text)){
        QTextStream write(&file);
        QString temporal="";
        for(int i=0;i<this->totalInstrucciones;i++){
            temporal+=this->memory[i];
            temporal+="\n";
        }
        write<<temporal;
        file.close();
        return true;
    }else{
        return false;
    }
}
bool Simpletron::guardarInformacionEjecucion(QString path){
    QFile file(path);
    if(file.open(QIODevice::WriteOnly| QIODevice::Text)){
        QString temporal="<----MENSAJES---->\n";
        temporal+=this->infoMemoria;
        temporal+="<----PROCESAMIENTO---->\n";
        temporal+=this->texto;
        QTextStream write(&file);
        write<<temporal;
        file.close();
        return true;
    }else{
        return false;
    }
}

