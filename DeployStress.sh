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

if test -d /home/utnso/Documentos/tp-2020-1c-C-aprueba/; then
	sudo rm -r /home/utnso/Documentos/tp-2020-1c-C-aprueba
fi

mkdir Documentos

cp -r tp-2020-1c-C-aprueba/ /home/utnso/Documentos/tp-2020-1c-C-aprueba

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
cd Documentos/tp-2020-1c-C-aprueba
git checkout develop

#INICIO DE GAMEBOY

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/gameboy-v2/Debug/

make clean
make all

cd ../src/resources/

if test -e gameBoy.config; then
	rm gameBoy.config
fi
#REVISAR -> No encuentro los parametros del gamecard(config en el archivo)
echo -e "IP_BROKER=127.0.0.1\nIP_TEAM=127.0.0.2\nIP_GAMECARD=127.0.0.3\nPUERTO_BROKER=4444\nPUERTO_TEAM=5002\nPUERTO_GAMECARD=5001\nLOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/gameboy-v2/src/resources/log_gameBoy.txt\nMI_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/gameboy-v2/src/resources/mi_log_gameBoy.txt" >> gameBoy.config

#FIN DE GAMEBOY

cd

#INICIO DE GAMECARD
cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/Debug/

make clean
make all

cd ../src/resources/

if test -e gamecard.config; then
	rm gamecard.config
fi
#REVISAR -> No encuentro los parametros del gamecard(config en el archivo)
echo -e "TIEMPO_DE_REINTENTO_CONEXION=10\nTIEMPO_DE_REINTENTO_OPERACION=5\nTIEMPO_RETARDO_OPERACION=1\nPUNTO_MONTAJE_TALLGRASS=/home/utnso/tallgrass-stress/\nIP_BROKER=IP_BROKER=127.0.0.1\nPUERTO_BROKER=PUERTO_BROKER=4444\nID_MODULO=4\nIP_GAMECARD=127.0.0.3\nPUERTO_GAMECARD=5001\nLOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/log_gamecard_stress.txt\nNUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/nuestro_log_gamecard_stress.txt" >> gamecard.config

#FIN DE GAMECARD

cd

#INICIO BROKER

#CREO LOS BROKERS

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2/Debug/

make clean
make all

cd ../../

cp -r broker-v2/ broker-v2-Particiones-dinamicas-stress
cp -r broker-v2/ broker-v2-BUDDY-SYSTEM-stress

#CONFIGURO BROKERS

#Configuro broker consolidacion Particiones dinamicas

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-Particiones-dinamicas-stress/src/resources/

if test -e broker.config; then
	rm broker.config
fi

echo -e "TAMANO_MEMORIA=64\nTAMANO_MINIMO_PARTICION=4\nALGORITMO_MEMORIA=PARTICIONES\nALGORITMO_REEMPLAZO=FIFO\nALGORITMO_PARTICION_LIBRE=FF\nIP_BROKER=127.0.0.1\nPUERTO_BROKER=4444\nFRECUENCIA_COMPACTACION=10\nLOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-Particiones-dinamicas-stress/src/resources/log_broker.txt\nNUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-Particiones-dinamicas-stress/src/resources/mi_log_broker.txt\n">> broker.config

#Configuro broker Buddy System

cd ../../../broker-v2-BUDDY-SYSTEM-stress/src/resources/

if test -e broker.config; then
	rm broker.config
fi

echo -e "TAMANO_MEMORIA=64\nTAMANO_MINIMO_PARTICION=4\nALGORITMO_MEMORIA=BS\nALGORITMO_REEMPLAZO=FIFO\nALGORITMO_PARTICION_LIBRE=FF\nIP_BROKER=127.0.0.1\nPUERTO_BROKER=4444\nFRECUENCIA_COMPACTACION=1\nLOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-BUDDY-SYSTEM-stress/src/resources/log_broker.txt\nNUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2-BUDDY-SYSTEM-stress/src/resources/mi_log_broker.txt\n">> broker.config

#FIN DE BROKER

cd

#CREO TEAMS

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2/Debug/

make clean
make all

cd ../../

cp -r team-v2/ team-v2-FIFO-stress
cp -r team-v2/ team-v2-RR-stress
cp -r team-v2/ team-v2-SJF-SD-stress
cp -r team-v2/ team-v2-SJF-CD-stress

#CONFIGURO LOS TEAMS

#Configuro team con FIFO

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-FIFO-stress/src/resources/

if test -e team.config; then
	rm team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,2|2]\nPOKEMON_ENTRENADORES=[Pikachu]\nOBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\nTIEMPO_RECONEXION=30\nRETARDO_CICLO_CPU=5\nALGORITMO_PLANIFICACION=FIFO\nQUANTUM=0\nALPHA=0.5\nESTIMACION_INICIAL=5\nIP_BROKER=127.0.0.1\nPUERTO_BROKER=4444\nLOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-FIFO-stress/src/resources/log_team_FIFO.txt\nNUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-FIFO-stress/src/resources/nuestro_log_team_FIFO.txt" >> team.config

#Configuro team con RR -> Quantum = 2

cd ../../../team-v2-RR-stress/src/resources/

if test -e team.config; then
	rm team.config
fi

echo -e "POSICIONES_ENTRENADORES=[2|3,6|5,9|9,9|2,2|9]\nPOKEMON_ENTRENADORES=[]\nOBJETIVOS_ENTRENADORES=[Vaporeon,Jolteon,Flareon,Umbreon,Espeon]\nTIEMPO_RECONEXION=30\nRETARDO_CICLO_CPU=5\nALGORITMO_PLANIFICACION=RR\nQUANTUM=1\nALPHA=0.5\nESTIMACION_INICIAL=5\nIP_BROKER=127.0.0.1\nPUERTO_BROKER=4444\nLOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-RR-stress/src/resources/log_team_RR.txt\nNUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-RR-stress/src/resources/nuestro_log_team_RR.txt\n" >> team.config

#Configuro team con SJF - SD

cd ../../../team-v2-SJF-SD-stress/src/resources/

if test -e team.config; then
	rm team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,2|2]\nPOKEMON_ENTRENADORES=[Pikachu]\nOBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\nTIEMPO_RECONEXION=30\nRETARDO_CICLO_CPU=5\nALGORITMO_PLANIFICACION=SFJ-SD\nQUANTUM=0\nALPHA=0.5\nESTIMACION_INICIAL=5\nIP_BROKER=127.0.0.1\nPUERTO_BROKER=4444\nLOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-SJF-SD-stress/src/resources/log_team_SJF-SD.txt\nNUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-SJF-SD-stress/src/resources/nuestro_log_team_SJF-SD.txt\n" >> team.config

#Configuro team con SJF - CD

cd ../../../team-v2-SJF-CD-stress/src/resources/

if test -e team.config; then
	rm team.config
fi

echo -e "POSICIONES_ENTRENADORES=[2|3,6|5,9|9,9|2,2|9]\nPOKEMON_ENTRENADORES=[]\nOBJETIVOS_ENTRENADORES=[Vaporeon,Jolteon,Flareon,Umbreon,Espeon]\nTIEMPO_RECONEXION=30\nRETARDO_CICLO_CPU=5\nALGORITMO_PLANIFICACION=RR\nQUANTUM=1\nALPHA=0.5\nESTIMACION_INICIAL=5\nIP_BROKER=127.0.0.1\nPUERTO_BROKER=4444\nLOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-SJF-CD-stress/src/resources/log_team_RR.txt\nNUESTRO_LOG_FILE=/home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2-SJF-CD-stress/src/resources/nuestro_log_team_RR.txt\n" >> team.config
#FIN DE TEAM

cd

# -------------
#Deploy de los sistemas

if test ! -d /home/utnso/Documentos; then
    mkdir /home/utnso/Documentos
fi

cd

mv /home/utnso/tp-2020-1c-C-aprueba/  /home/utnso/Documentos

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/Debug
    make clean
    make all

cd

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/gameboy-v2/Debug
    make clean
    make all

cd

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/broker-v2/Debug
    make clean
    make all

cd

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba/team-v2/Debug
    make clean
    make all

cd /home/utnso/Documentos/tp-2020-1c-C-aprueba
