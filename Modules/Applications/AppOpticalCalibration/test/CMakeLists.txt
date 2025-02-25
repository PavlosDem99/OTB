#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()
#----------- OpticalCalibration TESTS ----------------
otb_test_application(NAME apTvRaOpticalCalibration_QuickbirdXS
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             -level toa
                             -clamp false
                             -out ${TEMP}/apTvRaOpticalCalibration_QuickbirdXS.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdXS.tif
                             ${TEMP}/apTvRaOpticalCalibration_QuickbirdXS.tif )

otb_test_application(NAME apTvRaOpticalCalibration_SolarDistance
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             -level toa
                             -clamp false
                             -acqui.solardistance 0.9987593
                             -out ${TEMP}/apTvRaOpticalCalibration_SolarDistance.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdXS.tif
                             ${TEMP}/apTvRaOpticalCalibration_SolarDistance.tif )

otb_test_application(NAME apTvRaOpticalCalibration_WV2PAN
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/WV2_PAN_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_WV2PAN.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoWV2PAN.tif
                             ${TEMP}/apTvRaOpticalCalibration_WV2PAN.tif )

otb_test_application(NAME apTvRaOpticalCalibration_WV2Multi
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/WV2_MUL_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_WV2Multi.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoWV2Multi.tif
                             ${TEMP}/apTvRaOpticalCalibration_WV2Multi.tif )

otb_test_application(NAME apTvRaOpticalCalibration_Formosat
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/FORMOSAT_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_Formosat.img
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoFormosat.tif
                             ${TEMP}/apTvRaOpticalCalibration_Formosat.img )

otb_test_application(NAME apTvRaOpticalCalibration_QuickbirdPAN
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/QB_PAN_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_QuickbirdPAN.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdPAN.tif
                             ${TEMP}/apTvRaOpticalCalibration_QuickbirdPAN.tif )

otb_test_application(NAME apTvRaOpticalCalibration_Spot5
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/SPOT5_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_Spot5.img
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoSpot5.tif
                             ${TEMP}/apTvRaOpticalCalibration_Spot5.img )

otb_test_application(NAME apTvRaOpticalCalibration_UnknownSensor
                     APP  OpticalCalibration
                     OPTIONS 
           -in ${INPUTDATA}/Romania_Extract.tif
           -out ${TEMP}/apTvRaOpticalCalibration_Spot4_UnknownSensor_test.tif
           -level toc
           -acqui.gainbias ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorGainsBiases2.txt
           -acqui.day 4
           -acqui.month 12
           -acqui.sun.elev 62.7
           -acqui.sun.azim 152.7
           -acqui.view.elev 87.5
           -acqui.view.azim 283
           -acqui.solarilluminations ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorSolarIllumations2.txt
           -atmo.rsr ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorRSR.txt
           -atmo.pressure 1013.0
           -atmo.wa 2.48134
           -atmo.oz 0.34400
           -atmo.aerosol continental
           -atmo.opt 0.199854
           -atmo.radius 2
           -atmo.pixsize 0.02
           -milli false
           -clamp false
                     VALID   --compare-image ${EPSILON_6}
                             ${OTB_DATA_ROOT}/Baseline/Examples/Radiometry/Example_RomaniaAtmosphericCorrectionSequencement.tif
                             ${TEMP}/apTvRaOpticalCalibration_Spot4_UnknownSensor_test.tif )

otb_test_application(NAME apTvRaOpticalCalibration_Reverse_UnknownSensor
                     APP  OpticalCalibration
                     OPTIONS 
			     -in ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdXS.tif
			     -out ${TEMP}/apTvRaOpticalCalibration_Rev_QB-XS_UnknownSensor_test.tif
			     -level toatoim 
			     -acqui.gainbias ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorGainsBiases.txt
			     -acqui.day 1
			     -acqui.month 4
			     -acqui.sun.elev 48.6
			     -acqui.solarilluminations ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorSolarIllumations.txt
		       -milli false
			     -clamp false
                     VALID   --compare-image ${EPSILON_6}
                             ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             ${TEMP}/apTvRaOpticalCalibration_Rev_QB-XS_UnknownSensor_test.tif )

otb_test_application(NAME apTvRaOpticalCalibration_Ikonos
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/IKONOS_BLOSSEVILLE_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_Ikonos.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoIkonos.tif
                             ${TEMP}/apTvRaOpticalCalibration_Ikonos.tif )

