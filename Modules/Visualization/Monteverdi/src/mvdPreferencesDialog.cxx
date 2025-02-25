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

#include "mvdPreferencesDialog.h"
#include "ui_mvdPreferencesDialog.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include <otbQtAdapters.h>

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdCore.h"
#include "mvdGui.h"
#include "mvdI18nApplication.h"
#include "mvdI18nMainWindow.h"

namespace mvd
{
/*
  TRANSLATOR mvd::PreferencesDialog

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

bool IsPathModified(I18nCoreApplication::SettingsKey, bool, I18nCoreApplication::SettingsKey, const QString&);

/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */
/*****************************************************************************/
bool IsPathModified(I18nCoreApplication::SettingsKey enabledKey, bool isEnabled, I18nCoreApplication::SettingsKey pathKey, const QString& pathname)
{
  // qDebug()
  //   << "::IsPathModified("
  //   << I18nApplication::Instance()->RetrieveSettingsKey( enabledKey ).toBool()
  //   << ","
  //   << isEnabled
  //   << ","
  //   << I18nApplication::Instance()->RetrieveSettingsKey(
  //        pathKey
  //      ).toString()
  //   << ","
  //   << pathname
  //   << ")";

  return I18nApplication::Instance()->RetrieveSettingsKey(enabledKey).toBool() != isEnabled ||
         QDir(I18nApplication::Instance()->RetrieveSettingsKey(pathKey).toString()) != QDir(pathname);
}

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
PreferencesDialog::PreferencesDialog(QWidget* p, Qt::WindowFlags flags)
  : QDialog(p, flags), m_UI(new mvd::Ui::PreferencesDialog()), m_ResultsDirModified(false), m_GeoidFileModified(false), m_SrtmDirModified(false)
{
  assert(m_UI != NULL);
  assert(I18nApplication::Instance() != NULL);

  m_UI->setupUi(this);

  //
  // Runtime UI initialization.
  m_UI->settingsGroupBox->setVisible(false);

  AddItemsInto(m_UI->resolutionComboBox, "mvd", RESOLUTION_NAMES, RESOLUTION_COUNT);

  AddItemsInto(m_UI->tileSizeComboBox, "mvd", TILE_SIZE_NAMES, TILE_SIZE_COUNT);

  //
  // General settings.
  m_UI->resultDirPathLineEdit->setText(I18nApplication::Instance()->RetrieveSettingsKey(I18nCoreApplication::SETTINGS_KEY_RESULTS_DIR).toString());

  //
  // GDAL overviews.
  {
    QVariant value(I18nApplication::Instance()->RetrieveSettingsKey(I18nCoreApplication::SETTINGS_KEY_OVERVIEWS_ENABLED));

    m_UI->overviewsCheckBox->setChecked(!value.isValid() ? OVERVIEWS_ENABLED_DEFAULT : value.toBool());
  }
  {
    QVariant value(I18nApplication::Instance()->RetrieveSettingsKey(I18nCoreApplication::SETTINGS_KEY_OVERVIEWS_SIZE));

    m_UI->overviewsSpinBox->setValue(!value.isValid() ? OVERVIEWS_SIZE_DEFAULT : value.toInt());
  }

  //
  // Rendering settings.
  {
    QVariant value(I18nApplication::Instance()->RetrieveSettingsKey(I18nCoreApplication::SETTINGS_KEY_RESOLUTION));

    m_UI->resolutionComboBox->setCurrentIndex(!value.isValid() ? RESOLUTION_DEFAULT : value.toInt());
  }

  {
    QVariant value(I18nApplication::Instance()->RetrieveSettingsKey(I18nCoreApplication::SETTINGS_KEY_TILE_SIZE));

    m_UI->tileSizeComboBox->setCurrentIndex(TILE_SIZE_DEFAULT);

    int val = value.toInt();

    for (int i = 0; i < TILE_SIZE_COUNT; ++i)
      if (val == TILE_SIZE_VALUE[i])
      {
        m_UI->tileSizeComboBox->setCurrentIndex(i);

        break;
      }
  }

  //
  // Elevation management settings.
  m_UI->srtmLineEdit->setText(I18nApplication::Instance()->RetrieveSettingsKey(I18nCoreApplication::SETTINGS_KEY_SRTM_DIR).toString());

  m_UI->srtmCheckbox->setChecked(I18nApplication::Instance()->RetrieveSettingsKey(I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE).toBool());
  m_UI->srtmLineEdit->setEnabled(m_UI->srtmCheckbox->isChecked());
  m_UI->srtmButton->setEnabled(m_UI->srtmCheckbox->isChecked());

  m_UI->geoidLineEdit->setText(I18nApplication::Instance()->RetrieveSettingsKey(I18nCoreApplication::SETTINGS_KEY_GEOID_PATH).toString());

  m_UI->geoidCheckbox->setChecked(I18nApplication::Instance()->RetrieveSettingsKey(I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE).toBool());

  m_UI->geoidLineEdit->setEnabled(m_UI->geoidCheckbox->isChecked());
  m_UI->geoidButton->setEnabled(m_UI->geoidCheckbox->isChecked());

  // Connect centralWidget manipulator to Ql renderer when viewportRegionChanged
  QObject::connect(m_UI->buttonBox, SIGNAL(rejected()), this, SLOT(close()));

  assert(!m_ResultsDirModified);
  assert(!m_GeoidFileModified);
  assert(!m_SrtmDirModified);

  // qDebug() << "results-dir:" << m_ResultsDirModified;
  // qDebug() << "geoid-file:" << m_GeoidFileModified;
  // qDebug() << "srtm-dir:" << m_SrtmDirModified;
}

/*****************************************************************************/
PreferencesDialog::~PreferencesDialog()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void PreferencesDialog::on_buttonBox_accepted()
{
  //
  // General settings.
  if (m_ResultsDirModified)
  {
    // Set the result dir
    I18nApplication::Instance()->StoreSettingsKey(I18nCoreApplication::SETTINGS_KEY_RESULTS_DIR, QDir::cleanPath(m_UI->resultDirPathLineEdit->text()));
  }

  I18nApplication::Instance()->StoreSettingsKey(I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE, m_UI->srtmCheckbox->isChecked());
  I18nApplication::Instance()->StoreSettingsKey(I18nCoreApplication::SETTINGS_KEY_SRTM_DIR, QDir::cleanPath(m_UI->srtmLineEdit->text()));

  I18nApplication::Instance()->StoreSettingsKey(I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE, m_UI->geoidCheckbox->isChecked());
  I18nApplication::Instance()->StoreSettingsKey(I18nCoreApplication::SETTINGS_KEY_GEOID_PATH, m_UI->geoidLineEdit->text());

  //
  // Overview settings.
  I18nApplication::Instance()->StoreSettingsKey(I18nCoreApplication::SETTINGS_KEY_OVERVIEWS_ENABLED, m_UI->overviewsCheckBox->isChecked());
  I18nApplication::Instance()->StoreSettingsKey(I18nCoreApplication::SETTINGS_KEY_OVERVIEWS_SIZE, m_UI->overviewsSpinBox->value());

  //
  // Rendering settings.
  I18nApplication::Instance()->StoreSettingsKey(I18nCoreApplication::SETTINGS_KEY_RESOLUTION, m_UI->resolutionComboBox->currentIndex());

  {
    bool isOk = true;

    int value = m_UI->tileSizeComboBox->currentText().toInt(&isOk);

    assert(isOk);

    if (!isOk)
      value = TILE_SIZE_VALUE[TILE_SIZE_DEFAULT];

    I18nApplication::Instance()->StoreSettingsKey(I18nCoreApplication::SETTINGS_KEY_TILE_SIZE, value);
  }

  //
  // Elevation management settings.
  if (m_GeoidFileModified)
  {
    try
    {
      if (I18nApplication::Instance()->ElevationSetup())
      {
        QMessageBox::warning(
            this, "Warning!",
            tr("Geoid file has changed since application has been started. This setting needs the application to be restarted to be taken into account."));
      }
    }
    catch (const std::exception& exception)
    {
      QMessageBox::warning(this, "Warning!", tr("The following exception has been caught when setting up Elevation Settings:\n\n%1").arg(exception.what()));
    }
  }

  m_ResultsDirModified = false;
  m_GeoidFileModified  = false;
  m_SrtmDirModified    = false;

  close();
}

/*****************************************************************************/
void PreferencesDialog::on_srtmCheckbox_toggled(bool checked)
{
  assert(m_UI != NULL);

  m_UI->srtmLineEdit->setEnabled(checked);
  m_UI->srtmButton->setEnabled(checked);

  m_SrtmDirModified =
      IsPathModified(I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE, checked, I18nCoreApplication::SETTINGS_KEY_SRTM_DIR, m_UI->srtmLineEdit->text());
}

/***************************************************************************/
void PreferencesDialog::on_srtmLineEdit_textChanged(const QString& text)
{
  assert(m_UI != NULL);

  m_SrtmDirModified =
      IsPathModified(I18nCoreApplication::SETTINGS_KEY_SRTM_DIR_ACTIVE, m_UI->srtmCheckbox->isChecked(), I18nCoreApplication::SETTINGS_KEY_SRTM_DIR, text);
}

/***************************************************************************/
void PreferencesDialog::on_geoidCheckbox_toggled(bool checked)
{
  m_UI->geoidLineEdit->setEnabled(checked);
  m_UI->geoidButton->setEnabled(checked);

  m_GeoidFileModified =
      IsPathModified(I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE, checked, I18nCoreApplication::SETTINGS_KEY_GEOID_PATH, m_UI->geoidLineEdit->text());
}

/***************************************************************************/
void PreferencesDialog::on_geoidLineEdit_textChanged(const QString& text)
{
  m_GeoidFileModified =
      IsPathModified(I18nCoreApplication::SETTINGS_KEY_GEOID_PATH_ACTIVE, m_UI->geoidCheckbox->isChecked(), I18nCoreApplication::SETTINGS_KEY_GEOID_PATH, text);
}

/***************************************************************************/
void PreferencesDialog::on_srtmButton_clicked()
{
  QString srtmDir(otb::GetExistingDirectory(this, tr("Select the directory containing DEM files.")));

  if (srtmDir.isEmpty())
    return;

  m_UI->srtmLineEdit->setText(QDir(srtmDir).absolutePath());
}

/***************************************************************************/
void PreferencesDialog::on_geoidButton_clicked()
{
  QString geoidFile(otb::GetOpenFilename(this, tr("Select a geoid file.")));

  if (geoidFile.isEmpty())
    return;

  m_UI->geoidLineEdit->setText(QDir(geoidFile).absolutePath());
}

/***************************************************************************/
void PreferencesDialog::on_resultDirButton_clicked()
{
  QString resultsDir(otb::GetExistingDirectory(this, tr("Select the default directory for results data: ")));

  if (resultsDir.isEmpty())
    return;

  m_UI->resultDirPathLineEdit->setText(QDir(resultsDir).absolutePath());
}

} // end namespace 'mvd'
