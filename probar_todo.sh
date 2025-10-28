#!/bin/bash
set -e

cd ~/proyecto-so
echo "[*] Compilando..."
make clean && make

echo "[*] Matando procesos previos..."
pkill -f "./bin/agente" 2>/dev/null || true
pkill -f "./bin/controlador" 2>/dev/null || true

echo "[*] Reiniciando FIFO..."
rm -f /tmp/pipe_principal

echo "[*] Levantando controlador..."
./bin/controlador &
PID_C=$!
sleep 0.5

echo "[*] Lanzando agentes..."
./bin/agente -s sensor1 -a data/sensor1.csv -p /tmp/pipe_principal &
./bin/agente -s sensor2 -a data/sensor2.csv -p /tmp/pipe_principal &
./bin/agente -s sensor3 -a data/sensor3.csv -p /tmp/pipe_principal &
./bin/agente -s sensor4 -a data/sensor4.csv -p /tmp/pipe_principal &

echo "[*] Mostrando logs del controlador (Ctrl+C para salir, no mata el proceso):"
sleep 0.5
lsof /tmp/pipe_principal || true
echo "PID controlador: $PID_C"
wait $PID_C
