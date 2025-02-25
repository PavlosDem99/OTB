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

#ifndef otbQuickBirdImageMetadataInterfaceFactory_h
#define otbQuickBirdImageMetadataInterfaceFactory_h

#include "OTBMetadataExport.h"
#include "itkObjectFactoryBase.h"

namespace otb
{
/** \class QuickBirdImageMetadataInterfaceFactory
 * \brief Creating an instance of a ImageMetadataInterface object using object factory.
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT QuickBirdImageMetadataInterfaceFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef QuickBirdImageMetadataInterfaceFactory Self;
  typedef itk::ObjectFactoryBase                 Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const override;
  const char* GetDescription(void) const override;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(QuickBirdImageMetadataInterfaceFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    QuickBirdImageMetadataInterfaceFactory::Pointer quickBirdIMIFactory = QuickBirdImageMetadataInterfaceFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(quickBirdIMIFactory);
  }

protected:
  QuickBirdImageMetadataInterfaceFactory();
  ~QuickBirdImageMetadataInterfaceFactory() override;

private:
  QuickBirdImageMetadataInterfaceFactory(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
