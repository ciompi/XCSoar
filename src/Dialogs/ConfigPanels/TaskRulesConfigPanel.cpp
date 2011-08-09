/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2011 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "TaskRulesConfigPanel.hpp"
#include "Profile/ProfileKeys.hpp"
#include "Profile/Profile.hpp"
#include "Form/Edit.hpp"
#include "Form/Util.hpp"
#include "DataField/Enum.hpp"
#include "Interface.hpp"
#include "Language/Language.hpp"

static WndForm* wf = NULL;


void
TaskRulesConfigPanel::Init(WndForm *_wf)
{
  assert(_wf != NULL);
  wf = _wf;
  WndProperty *wp;
  const SETTINGS_COMPUTER &settings_computer = XCSoarInterface::SettingsComputer();

  LoadFormProperty(*wf, _T("prpStartMaxSpeed"), ugHorizontalSpeed,
                   settings_computer.ordered_defaults.start_max_speed);

  LoadFormProperty(*wf, _T("prpStartMaxSpeedMargin"), ugHorizontalSpeed,
                   settings_computer.start_max_speed_margin);

  static const StaticEnumChoice start_max_height_ref_list[] = {
    { hrAGL, N_("AGL"), N_("Reference AGL for start maximum height rule (above start point)") },
    { hrMSL, N_("MSL"), N_("Reference MSL for start maximum height rule (above sea level)") },
    { 0 }
  };
  LoadFormProperty(*wf, _T("prpStartHeightRef"), start_max_height_ref_list,
                   settings_computer.ordered_defaults.start_max_height_ref);

  LoadFormProperty(*wf, _T("prpStartMaxHeight"), ugAltitude,
                   settings_computer.ordered_defaults.start_max_height);

  LoadFormProperty(*wf, _T("prpStartMaxHeightMargin"), ugAltitude,
                   settings_computer.start_max_height_margin);

  static const StaticEnumChoice finish_min_height_ref_list[] = {
    { hrAGL, N_("AGL"), N_("Reference AGL for finish minimum height rule (above finish point)") },
    { hrMSL, N_("MSL"), N_("Reference MSL for finish minimum height rule (above sea level)") },
    { 0 }
  };
  LoadFormProperty(*wf, _T("prpFinishHeightRef"), finish_min_height_ref_list,
                   settings_computer.ordered_defaults.finish_min_height_ref);

  LoadFormProperty(*wf, _T("prpFinishMinHeight"), ugAltitude,
                   settings_computer.ordered_defaults.finish_min_height);

  wp = (WndProperty*)wf->FindByName(_T("prpContests"));
  if (wp) {
    DataFieldEnum* dfe;
    dfe = (DataFieldEnum*)wp->GetDataField();
    dfe->addEnumText(ContestToString(OLC_FAI), OLC_FAI);
    dfe->addEnumText(ContestToString(OLC_Classic), OLC_Classic);
    dfe->addEnumText(ContestToString(OLC_League), OLC_League);
    dfe->addEnumText(ContestToString(OLC_Plus), OLC_Plus);
    dfe->addEnumText(ContestToString(OLC_XContest), OLC_XContest);
    dfe->addEnumText(ContestToString(OLC_DHVXC), OLC_DHVXC);
    dfe->addEnumText(ContestToString(OLC_SISAT), OLC_SISAT);
    dfe->Set(settings_computer.contest);
    wp->RefreshDisplay();
  }
}


bool
TaskRulesConfigPanel::Save()
{
  bool changed = false;
  SETTINGS_COMPUTER &settings_computer = XCSoarInterface::SetSettingsComputer();

  changed |= SaveFormProperty(*wf, _T("prpStartMaxSpeed"),
                                  ugHorizontalSpeed,
                                  settings_computer.ordered_defaults.start_max_speed,
                                  szProfileStartMaxSpeed);

  changed |= SaveFormProperty(*wf, _T("prpStartMaxSpeedMargin"),
                                  ugHorizontalSpeed,
                                  settings_computer.start_max_speed_margin,
                                  szProfileStartMaxSpeedMargin);

  changed |= SaveFormProperty(*wf, _T("prpStartMaxHeight"), ugAltitude,
                                  settings_computer.ordered_defaults.start_max_height,
                                  szProfileStartMaxHeight);

  changed |= SaveFormProperty(*wf, _T("prpStartMaxHeightMargin"), ugAltitude,
                                  settings_computer.start_max_height_margin,
                                  szProfileStartMaxHeightMargin);

  changed |= SaveFormPropertyEnum(*wf, _T("prpStartHeightRef"),
                              szProfileStartHeightRef,
                              settings_computer.ordered_defaults.start_max_height_ref);

  changed |= SaveFormProperty(*wf, _T("prpFinishMinHeight"), ugAltitude,
                                  settings_computer.ordered_defaults.finish_min_height,
                                  szProfileFinishMinHeight);

  changed |= SaveFormPropertyEnum(*wf, _T("prpFinishHeightRef"),
                              szProfileFinishHeightRef,
                              settings_computer.ordered_defaults.finish_min_height_ref);

  unsigned t = settings_computer.contest;
  changed |= SaveFormProperty(*wf, _T("prpContests"), szProfileOLCRules, t);
  settings_computer.contest = (Contests) t;

  return changed;
}
