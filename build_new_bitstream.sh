#!/bin/bash

cd /root/MyFirstM2M

cd M2M/QNICE/tools
./make-toolchain.sh
ls -l ../monitor/monitor.rom

cd ../../../CORE/m2m-rom
./make_rom.sh

cd ..
/root/Xilinx/Vivado/2022.2/bin/vivado

./bit2core mega65r3 CORE-R3.runs/impl_1/CORE_R3.bit MyFirstM2M V1 mfm2m.cor

cd /root/MyFirstM2M
