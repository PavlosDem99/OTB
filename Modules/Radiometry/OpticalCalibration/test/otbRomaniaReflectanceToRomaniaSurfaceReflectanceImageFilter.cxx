/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include <fstream>
#include <iostream>

int otbRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char*              inputFileName  = argv[1];
  const char*              outputFileName = argv[2];
  const char*              paramFile      = argv[3];
  std::vector<const char*> wavelenghFiles;

  const unsigned int Dimension = 2;
  typedef double     PixelType;
  typedef otb::VectorImage<PixelType, Dimension> InputImageType;
  typedef otb::VectorImage<PixelType, Dimension> OutputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::ReflectanceToSurfaceReflectanceImageFilter<InputImageType, OutputImageType> ReflectanceToSurfaceReflectanceImageFilterType;

  typedef otb::RadiometryCorrectionParametersToAtmosphericRadiativeTerms CorrectionParametersToRadiativeTermsType;

  typedef otb::AtmosphericCorrectionParameters           AtmoCorrectionParametersType;
  typedef AtmoCorrectionParametersType::Pointer          AtmoCorrectionParametersPointerType;
  typedef AtmoCorrectionParametersType::AerosolModelType AerosolModelType;

  typedef otb::ImageMetadataCorrectionParameters AcquiCorrectionParametersType;
  typedef AcquiCorrectionParametersType::Pointer AcquiCorrectionParametersPointerType;

  typedef otb::AtmosphericRadiativeTerms         AtmosphericRadiativeTermsType;
  typedef AtmosphericRadiativeTermsType::Pointer AtmosphericRadiativeTermsPointerType;

  typedef otb::FilterFunctionValues                            FilterFunctionValuesType;
  typedef FilterFunctionValuesType::WavelengthSpectralBandType ValueType;        // float
  typedef FilterFunctionValuesType::ValuesVectorType           ValuesVectorType; // std::vector<float>

  AcquiCorrectionParametersPointerType paramAcqui = AcquiCorrectionParametersType::New();
  AtmoCorrectionParametersPointerType  paramAtmo  = AtmoCorrectionParametersType::New();
  FilterFunctionValuesType::Pointer    functionValues;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  reader->UpdateOutputInformation();
  unsigned int nbChannel = reader->GetOutput()->GetNumberOfComponentsPerPixel();
  for (unsigned int i = 0; i < nbChannel; ++i)
  {
    wavelenghFiles.push_back(argv[i + 4]);
  }

  ValueType val = 0.0025;

  // Correction parameters initialization
  double       solarZenithalAngle(0.);
  double       solarAzimutalAngle(0.);
  double       viewingZenithalAngle(0.);
  double       viewingAzimutalAngle(0.);
  unsigned int month(0);
  unsigned int day(0);
  double       atmosphericPressure(0.);
  double       waterVaporAmount(0.);
  double       ozoneAmount(0.);
  double       aerosolOptical(0.);

  std::ifstream fin;
  fin.open(paramFile);
  // Read input file parameters
  fin >> solarZenithalAngle;   // asol;
  fin >> solarAzimutalAngle;   // phi0;
  fin >> viewingZenithalAngle; // avis;
  fin >> viewingAzimutalAngle; // phiv;
  fin >> month;                // month;
  fin >> day;                  // jday;
  fin >> atmosphericPressure;  // pressure;
  fin >> waterVaporAmount;     // uw;
  fin >> ozoneAmount;          // uo3;
  unsigned int aer(0);
  fin >> aer; // iaer;
  AerosolModelType aerosolModel = static_cast<AerosolModelType>(aer);
  fin >> aerosolOptical; // taer55;
  fin.close();
  // Set atmospheric parameters
  /*param->SetSolarZenithalAngle(static_cast<double>(solarZenithalAngle));
  param->SetSolarAzimutalAngle(static_cast<double>(solarAzimutalAngle));
  param->SetViewingZenithalAngle(static_cast<double>(viewingZenithalAngle));
  param->SetViewingAzimutalAngle(static_cast<double>(viewingAzimutalAngle));
  param->SetMonth(month);
  param->SetDay(day);
  param->SetAtmosphericPressure(static_cast<double>(atmosphericPressure));
  param->SetWaterVaporAmount(static_cast<double>(waterVaporAmount));
  param->SetOzoneAmount(static_cast<double>(ozoneAmount));
  param->SetAerosolModel(aerosolModel);
  param->SetAerosolOptical(static_cast<double>(aerosolOptical)); */

  paramAcqui->SetSolarZenithalAngle(solarZenithalAngle);
  paramAcqui->SetSolarAzimutalAngle(solarAzimutalAngle);
  paramAcqui->SetViewingZenithalAngle(viewingZenithalAngle);
  paramAcqui->SetViewingAzimutalAngle(viewingAzimutalAngle);
  paramAcqui->SetMonth(month);
  paramAcqui->SetDay(day);
  paramAtmo->SetAtmosphericPressure(atmosphericPressure);
  paramAtmo->SetWaterVaporAmount(waterVaporAmount);
  paramAtmo->SetOzoneAmount(ozoneAmount);
  paramAtmo->SetAerosolModel(aerosolModel);
  paramAtmo->SetAerosolOptical(aerosolOptical);


  ValuesVectorType vect;
  for (unsigned int j = 0; j < nbChannel; ++j)
  {
    functionValues = FilterFunctionValuesType::New();
    vect.clear();

    // Filter function values initialization
    float minSpectralValue(0.);
    float maxSpectralValue(0.);
    float value(0.);

    std::ifstream fin2;
    // Read input file parameters
    fin2.open(wavelenghFiles[j]);
    fin2 >> minSpectralValue; // wlinf;
    fin2 >> maxSpectralValue; // wlsup;

    while (fin2.good())
    {
      std::string line;
      std::getline(fin2, line);
      if (!line.empty())
      {
        value = atof(line.c_str());
        vect.push_back(value);
      }
    }

    fin2.close();

    functionValues->SetFilterFunctionValues(vect);
    functionValues->SetMinSpectralValue(minSpectralValue);
    functionValues->SetMaxSpectralValue(maxSpectralValue);
    functionValues->SetUserStep(val);

    paramAcqui->SetWavelengthSpectralBandWithIndex(j, functionValues);
  }

  AtmosphericRadiativeTermsPointerType radiative = CorrectionParametersToRadiativeTermsType::Compute(paramAtmo, paramAcqui);

  // Instantiating object
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer filter = ReflectanceToSurfaceReflectanceImageFilterType::New();
  filter->SetAtmosphericRadiativeTerms(radiative);
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
