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

set(OTBIOMSTARTests
otbIOMSTARTestDriver.cxx
otbMSTARImageIOTestCanRead.cxx
)

add_executable(otbIOMSTARTestDriver ${OTBIOMSTARTests})
target_link_libraries(otbIOMSTARTestDriver ${OTBIOMSTAR-Test_LIBRARIES})
otb_module_target_label(otbIOMSTARTestDriver)

# Tests Declaration

otb_add_test(NAME ioTuMSTARImageIOCanRead2 COMMAND otbIOMSTARTestDriver otbMSTARImageIOTestCanRead
  ${INPUTDATA}/MSTAR/HB20010.000)

otb_add_test(NAME ioTuMSTARImageIOCanRead1 COMMAND otbIOMSTARTestDriver otbMSTARImageIOTestCanRead
  ${INPUTDATA}/MSTAR/HB19896.016)

