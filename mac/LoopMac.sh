#!/bin/bash

for i in $(seq $1 $2)
do
	sed 's/Dimension/$i' mac/CfNeutron_Cube_BGO.mac > mac/_set_CfNeutron_Cube_BGO.mac
	sed 's/Dimension/$i' mac/CfNeutron_Cube_Pla.mac > mac/_set_CfNeutron_Cube_Pla.mac
	sed 's/Dimension/$i' mac/CfNeutron_Cyl_BGO.mac > mac/_set_CfNeutron_Cyl_BGO.mac
	sed 's/Dimension/$i' mac/CfNeutron_Cyl_BGO.mac > mac/_set_CfNeutron_Cyl_Pla.mac


	./batches/QbatchExclusion $i
	./batches/QbatchExclusionMix $i
	./batches/QbatchExclusionW $i
	./Batches/NuTauFlux $i
done
