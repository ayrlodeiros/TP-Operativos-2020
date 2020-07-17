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

#INICIO DIRECTORIO DE GAMEBOY
if test -d /home/utnso/tallgrass-prueba-gameboy/; then
	sudo rm -r /home/utnso/tallgrass-prueba-gameboy
fi

cd
mkdir -p /home/utnso/tallgrass-prueba-gameboy
cd /home/utnso/tallgrass-prueba-gameboy/
mkdir Metadata
#mkdir Blocks
#mkdir Tables

cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=1024\nMAGIC_NUMBER=TALL_GRASS" >> Metadata.bin

#FIN DE GAMEBOY

cd

#INICIO DIRECTORIO DE GAMECARD
if test -d /home/utnso/tallgrass-prueba-gamecard/; then
	sudo rm -r /home/utnso/tallgrass-prueba-gamecard
fi

cd
mkdir -p /home/utnso/tallgrass-prueba-gamecard
cd /home/utnso/tallgrass-prueba-gamecard/
mkdir Metadata
#mkdir Blocks
#mkdir Tables

cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=1024\nMAGIC_NUMBER=TALL_GRASS" >> Metadata.bin

#FIN DE GAMECARD

cd

#INICIO DIRECTORIO DE BROKER
if test -d /home/utnso/tallgrass-prueba-broker/; then
	sudo rm -r /home/utnso/tallgrass-prueba-broker
fi

cd
mkdir -p /home/utnso/tallgrass-prueba-broker
cd /home/utnso/tallgrass-prueba-broker/
mkdir Metadata
#mkdir Blocks
#mkdir Tables

cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=1024\nMAGIC_NUMBER=TALL_GRASS" >> Metadata.bin

#FIN DE BROKER

cd

#CREO DIRECTORIO TEAMS

#CONFIGURO LOS TEAMS

#Configuro team con FIFO
if test -d /home/utnso/tallgrass-prueba-team-fifo/; then
	sudo rm -r /home/utnso/tallgrass-prueba-team-fifo
fi

cd
mkdir -p /home/utnso/tallgrass-prueba-team-fifo
cd /home/utnso/tallgrass-prueba-team-fifo/
mkdir Metadata
#mkdir Blocks
#mkdir Tables

cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=1024\nMAGIC_NUMBER=TALL_GRASS" >> Metadata.bin

#Fin configuracion Team FIFO

cd

#Configuro team con RR -> Quantum = 2

if test -d /home/utnso/tallgrass-prueba-team-RR/; then
	sudo rm -r /home/utnso/tallgrass-prueba-team-RR
fi

cd
mkdir -p /home/utnso/tallgrass-prueba-team-RR
cd /home/utnso/tallgrass-prueba-team-RR/
mkdir Metadata
#mkdir Blocks
#mkdir Tables

cd Metadata/
echo -e "BLOCK_SIZE=64\nBLOCKS=1024\nMAGIC_NUMBER=TALL_GRASS" >> Metadata.bin

#Fin configuracion Team RR

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
