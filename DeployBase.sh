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
sudo make install

cd

#FIN DE COMMONS

#BASE

if test -d /home/utnso/delibird-base/; then
	sudo rm -r /home/utnso/delibird-base/
fi

cd
mkdir -p /home/utnso/tallgrass-base
cd /home/utnso/tallgrass-base/

mkdir Metadata

#IMPORTANTE => ACORDARSE DE BORRAR EL METODO crear_archivo_metadata(64,1000); EN INICIAR GAMECARD
cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=1024\nMAGIC_NUMBER=TALL_GRASS" >> Metadata.bin

cd 

#FIN BASE

#COPIO CARPETA TP

if test -d /home/utnso/workspace/tp-2020-1c-C-aprueba/; then
	sudo rm -r /home/utnso/workspace/tp-2020-1c-C-aprueba
fi

cd /home/utnso/tp-safe-zone/

cp -r tp-2020-1c-C-aprueba/ /home/utnso/workspace/tp-2020-1c-C-aprueba

#FIN DE COPIAR CARPETA

#INICIO DE GAMEBOY
cd /home/utnso/workspace/tp-2020-1c-C-aprueba/gameboy-v2/Debug/

make all

cd ../src/resources/

if test -e .gameBoy.config; then
	rm .gameBoy.config
fi
#REVISAR -> No encuentro los parametros del gamecard(config en el archivo)
echo -e "IP_BROKER=\nIP_TEAM=\nIP_GAMECARD=\nPUERTO_BROKER=\nPUERTO_TEAM=\nPUERTO_GAMECARD=\n" >> .gameBoy.config

#FIN DE GAMEBOY

#INICIO DE GAMECARD
cd /home/utnso/workspace/tp-2020-1c-C-aprueba/gamecard-v2/Debug/

make all

cd ../src/resources/

if test -e .gamecard.config; then
	rm .gamecard.config
fi
#REVISAR -> No encuentro los parametros del gamecard(config en el archivo)
echo -e "TIEMPO_DE_REINTENTO_CONEXION=10\nTIEMPO_DE_REINTENTO_OPERACION=5\nTIEMPO_RETARDO_OPERACION=1\nPUNTO_MONTAJE_TALLGRASS=/home/utnso/tallgrass-base/\nIP_BROKER=\nPUERTO_BROKER=\nID_MODULO=4\nIP_GAMECARD=\nPUERTO_GAMECARD=\n" >> .gamecard.config

#FIN DE GAMECARD

#CREO TEAMS

cd /home/utnso/workspace/tp-2020-1c-C-aprueba/team-v2/Debug/

make all

cd ../../

cp team-v2/ team-v2-RR
cp team-v2/ team-v2-SJF-SD
cp team-v2/ team-v2-SJF-CD

#CONFIGURO LOS TEAMS

#Configuro team con FIFO

cd /home/utnso/workspace/tp-2020-1c-C-aprueba/team-v2/

cd /src/resources/

if test -e .team.config; then
	rm .team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,2|2]\nPOKEMON_ENTRENADORES=[Pikachu]\nOBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\nTIEMPO_RECONEXION=30\nRETARDO_CICLO_CPU=5\nALGORITMO_PLANIFICACION=FIFO\nQUANTUM=0\nALPHA=0.5\nESTIMACION_INICIAL=5\nIP_BROKER=\nPUERTO_BROKER=\nLOG_FILE=" >> .team.config

#Configuro team con RR -> Quantum = 2

cd ../../../team-v2-RR/

cd /src/resources/

if test -e .team.config; then
	rm .team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,2|2]\nPOKEMON_ENTRENADORES=[Pikachu]\nOBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\nTIEMPO_RECONEXION=30\nRETARDO_CICLO_CPU=5\nALGORITMO_PLANIFICACION=RR\nQUANTUM=2\nALPHA=0.5\nESTIMACION_INICIAL=5\nIP_BROKER=\nPUERTO_BROKER=\nLOG_FILE=" >> .team.config

#Configuro team con SJF - SD

cd ../../../team-v2-SJF-SD/

cd /src/resources/

if test -e .team.config; then
	rm .team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,2|2]\nPOKEMON_ENTRENADORES=[Pikachu]\nOBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\nTIEMPO_RECONEXION=30\nRETARDO_CICLO_CPU=5\nALGORITMO_PLANIFICACION=SFJ-SD\nQUANTUM=0\nALPHA=0.5\nESTIMACION_INICIAL=5\nIP_BROKER=\nPUERTO_BROKER=\nLOG_FILE=" >> .team.config

#Configuro team con SJF - CD

cd ../../../team-v2-SJF-CD/

cd /src/resources/

if test -e .team.config; then
	rm .team.config
fi

echo -e "POSICIONES_ENTRENADORES=[1|3,2|3,2|2]\nPOKEMON_ENTRENADORES=[Pikachu]\nOBJETIVOS_ENTRENADORES=[Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix]\nTIEMPO_RECONEXION=30\nRETARDO_CICLO_CPU=5\nALGORITMO_PLANIFICACION=SFJ-CD\nQUANTUM=0\nALPHA=0.5\nESTIMACION_INICIAL=5\nIP_BROKER=\nPUERTO_BROKER=\nLOG_FILE=" >> .team.config
#FIN DE TEAM

#INICIO BROKER

#CREO LOS BROKERS

cd /home/utnso/workspace/tp-2020-1c-C-aprueba/broker-v2/Debug/

make all

cd ../../

cp broker-v2/ broker-v2-COMPACTACION-PD
cp broker-v2/ broker-v2-BUDDY-SYSTEM

#CONFIGURO BROKERS

#Configuro broker consolidacion Particiones dinamicas

cd /home/utnso/workspace/tp-2020-1c-C-aprueba/broker-v2/

cd /src/resources/

if test -e .broker.config; then
	rm .broker.config
fi

echo -e "TAMANO_MEMORIA=64\nTAMANO_MINIMO_PARTICION=4\nALGORITMO_MEMORIA=PARTICIONES\nALGORITMO_REEMPLAZO=FIFO\nALGORITMO_PARTICION_LIBRE=FF\nIP_BROKER=\nPUERTO_BROKER=\nFRECUENCIA_COMPACTACION=10\nLOG_FILE=\n">> .broker.config

#Configuro broker compactacion Particiones Dinamicas

cd ../../../broker-v2-COMPACTACION-PD/

cd /src/resources/

if test -e .broker.config; then
	rm .broker.config
fi

echo -e "TAMANO_MEMORIA=64\nTAMANO_MINIMO_PARTICION=4\nALGORITMO_MEMORIA=PARTICIONES\nALGORITMO_REEMPLAZO=FF\nALGORITMO_PARTICION_LIBRE=FF\nIP_BROKER=\nPUERTO_BROKER=\nFRECUENCIA_COMPACTACION=1\nLOG_FILE=\n">> .broker.config

#Configuro broker compactacion Particiones Dinamicas

cd ../../../broker-v2-BUDDY-SYSTEM/

cd /src/resources/

if test -e .broker.config; then
	rm .broker.config
fi

echo -e "TAMANO_MEMORIA=64\nTAMANO_MINIMO_PARTICION=4\nALGORITMO_MEMORIA=BS\nALGORITMO_REEMPLAZO=FIFO\nALGORITMO_PARTICION_LIBRE=FF\nIP_BROKER=\nPUERTO_BROKER=\nFRECUENCIA_COMPACTACION=1\nLOG_FILE=\n">> .broker.config

#FIN DE BROKER

cd