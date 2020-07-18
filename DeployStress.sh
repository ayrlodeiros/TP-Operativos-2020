# tp-2020-1c-C-aprueba

cd

#INICIO DE COMMONS

if test -d /home/utnso/so-commons-library; then
    cd /home/utnso/so-commons-library/
    sudo make unistall
    cd ..
    sudo rm -r ./so-commons-library
fi

cd

git clone https://github.com/sisoputnfrba/so-commons-library.git 
cd so-commons-library/
make install

cd

#FIN DE COMMONS

#COPIO CARPETA TP

if test -d /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/; then
	sudo rm -r /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress
fi

mkdir Documentos

cp -r tp-2020-1c-C-aprueba/ /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress

#FIN DE COPIAR CARPETA

#STRESS

if test -d /home/utnso/tallgrass-stress/; then
	sudo rm -r /home/utnso/tallgrass-stress/
fi

cd
mkdir -p /home/utnso/tallgrass-stress
cd /home/utnso/tallgrass-stress/

mkdir Metadata

#IMPORTANTE => ACORDARSE DE BORRAR EL METODO crear_archivo_metadata(64,1000); EN INICIAR GAMECARD
cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=1024\nMAGIC_NUMBER=TALL_GRASS" >> Metadata.bin

#FIN STRESS

cd
cd Documentos/tp-2020-1c-C-aprueba-stress


#INICIO DE GAMEBOY

#CREO LOS GAMEBOYS

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/

cp -r gameboy/ gameboy-team-1-stress
cp -r gameboy/ gameboy-team-2-stress

#CONFIGURO GAMEBOYS

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/gameboy-team-1-stress/src/resources/

if test -e gameBoy.config; then
	rm gameBoy.config
fi
#REVISAR -> No encuentro los parametros del gamecard(config en el archivo)
echo -e "IP_BROKER=127.0.0.1\n
IP_TEAM=127.0.0.2\n
IP_GAMECARD=127.0.0.3\n
PUERTO_BROKER=4444\n
PUERTO_TEAM=5002\n
PUERTO_GAMECARD=5001\n
ID_MODULO=1\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/gameboy-team-1-stress/src/resources/log_gameBoy.txt\n
MI_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/gameboy-team-1-stress/src/resources/mi_log_gameBoy.txt" >> gameBoy.config

cd ../../Debug
make clean
make all

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/gameboy-team-2-stress/src/resources/

if test -e gameBoy.config; then
	rm gameBoy.config
fi
#REVISAR -> No encuentro los parametros del gamecard(config en el archivo)
echo -e "IP_BROKER=127.0.0.1\n
IP_TEAM=127.0.0.2\n
IP_GAMECARD=127.0.0.3\n
PUERTO_BROKER=4444\n
PUERTO_TEAM=5003\n
PUERTO_GAMECARD=5001\n
ID_MODULO=2\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/gameboy-team-2-stress/src/resources/log_gameBoy.txt\n
MI_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/gameboy-team-2-stress/src/resources/mi_log_gameBoy.txt" >> gameBoy.config

cd ../../Debug
make clean
make all

#FIN DE GAMEBOY

cd

#INICIO DE GAMECARD
cd /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/gamecard-v2/src/resources/

if test -e gamecard.config; then
	rm gamecard.config
fi
#REVISAR -> No encuentro los parametros del gamecard(config en el archivo)
echo -e "TIEMPO_DE_REINTENTO_CONEXION=10\n
TIEMPO_DE_REINTENTO_OPERACION=5\n
TIEMPO_RETARDO_OPERACION=1\n
PUNTO_MONTAJE_TALLGRASS=/home/utnso/tallgrass-stress\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=PUERTO_BROKER=4444\n
ID_MODULO=25\n
IP_GAMECARD=127.0.0.3\n
PUERTO_GAMECARD=5001\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/gamecard-v2/src/resources/log_gamecard_stress.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/gamecard-v2/src/resources/nuestro_log_gamecard_stress.txt" >> gamecard.config

cd ../../Debug
make clean
make all

#FIN DE GAMECARD

cd

#INICIO BROKER

#CREO LOS BROKERS

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/

cp -r broker-v2/ broker-v2-Particiones-dinamicas-stress
cp -r broker-v2/ broker-v2-BUDDY-SYSTEM-stress

#CONFIGURO BROKERS

#Configuro broker consolidacion Particiones dinamicas

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/broker-v2-Particiones-dinamicas-stress/src/resources/

if test -e broker.config; then
	rm broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=PARTICIONES\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
FRECUENCIA_COMPACTACION=10\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/broker-v2-Particiones-dinamicas-stress/src/resources/dump_file.txt\n
ID_MODULO=8\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/broker-v2-Particiones-dinamicas-stress/src/resources/log_broker.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/broker-v2-Particiones-dinamicas-stress/src/resources/mi_log_broker.txt\n">> broker.config

cd ../../Debug
make clean
make all

#Configuro broker Buddy System

cd ../../broker-v2-BUDDY-SYSTEM-stress/src/resources/

if test -e broker.config; then
	rm broker.config
fi

echo -e "TAMANO_MEMORIA=64\n
TAMANO_MINIMO_PARTICION=4\n
ALGORITMO_MEMORIA=BS\n
ALGORITMO_REEMPLAZO=FIFO\n
ALGORITMO_PARTICION_LIBRE=FF\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
FRECUENCIA_COMPACTACION=1\n
PATH_ARCHIVO_DUMP=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/broker-v2-BUDDY-SYSTEM-stress/src/resources/dump_file.txt\n
ID_MODULO=9\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/broker-v2-BUDDY-SYSTEM-stress/src/resources/log_broker.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/broker-v2-BUDDY-SYSTEM-stress/src/resources/mi_log_broker.txt\n">> broker.config

cd ../../Debug
make clean
make all

#FIN DE BROKER

cd

#CREO TEAMS

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/

cp -r team-v2/ team-v2-FIFO-stress
cp -r team-v2/ team-v2-RR-stress
cp -r team-v2/ team-v2-SJF-SD-stress
cp -r team-v2/ team-v2-SJF-CD-stress

#CONFIGURO LOS TEAMS

#Configuro team con FIFO

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/team-v2-FIFO-stress/src/resources/

if test -e team.config; then
	rm team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,2|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=FIFO\n
QUANTUM=0\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/team-v2-FIFO-stress/src/resources/log_team_FIFO.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/team-v2-FIFO-stress/src/resources/nuestro_log_team_FIFO.txt\n
ID_MODULO=15\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5002" >> team.config

cd ../../Debug
make clean
make all

#Configuro team con RR -> Quantum = 1

cd ../../team-v2-RR-stress/src/resources/

if test -e team.config; then
	rm team.config
fi

echo -e "POSICIONES_ENTRENADORES=[2|3,6|5,9|9,9|2,2|9]\n
POKEMON_ENTRENADORES=[]\n
OBJETIVOS_ENTRENADORES=[Vaporeon,Jolteon,Flareon,Umbreon,Espeon]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=RR\n
QUANTUM=1\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/team-v2-RR-stress/src/resources/log_team_RR.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/team-v2-RR-stress/src/resources/nuestro_log_team_RR.txt\n
ID_MODULO=16\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5003" >> team.config

cd ../../Debug
make clean
make all

#Configuro team con SJF - SD

cd ../../team-v2-SJF-SD-stress/src/resources/

if test -e team.config; then
	rm team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,2|2]\n
POKEMON_ENTRENADORES=[Pikachu]\n
OBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=SJF-SD\n
QUANTUM=0\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/team-v2-SJF-SD-stress/src/resources/log_team_SJF-SD.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/team-v2-SJF-SD-stress/src/resources/nuestro_log_team_SJF-SD.txt\n
ID_MODULO=17\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5002" >> team.config

cd ../../Debug
make clean
make all

#Configuro team con SJF - CD

cd ../../team-v2-SJF-CD-stress/src/resources/

if test -e team.config; then
	rm team.config
fi

echo -e "POSICIONES_ENTRENADORES=[2|3,6|5,9|9,9|2,2|9]\n
POKEMON_ENTRENADORES=[]\n
OBJETIVOS_ENTRENADORES=[Vaporeon,Jolteon,Flareon,Umbreon,Espeon]\n
TIEMPO_RECONEXION=30\n
RETARDO_CICLO_CPU=5\n
ALGORITMO_PLANIFICACION=SJF-CD\n
QUANTUM=1\n
ALPHA=0.5\n
ESTIMACION_INICIAL=5\n
IP_BROKER=127.0.0.1\n
PUERTO_BROKER=4444\n
LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/team-v2-SJF-CD-stress/src/resources/log_team_SJF-CD.txt\n
NUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba-stress/team-v2-SJF-CD-stress/src/resources/nuestro_log_team_SJF-CD.txt\n
ID_MODULO=18\n
IP_TEAM=127.0.0.2\n
PUERTO_TEAM=5003" >> team.config

cd ../../Debug
make clean
make all

#FIN DE TEAM

cd
