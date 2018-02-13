// HarmonyTrigger.cpp --- 
// 
// Filename: HarmonyTrigger.cpp
// Description: 
// Author: stax
// Maintainer: 
// Created: dim. déc. 10 19:42:01 2017 (+0100)
// Version: 
// Package-Requires: ()
// Last-Updated: jeu. déc. 14 21:56:19 2017 (+0100)
//           By: stax
//     Update #: 45
// URL: 
// Doc URL: 
// Keywords: 
// Compatibility: 
// 
// 

// Commentary: 
// 
// 
// 
// 

// Change Log:
// 
// 
// 
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
// 
// 

#include "HarmonyTrigger.h"

HarmonyTrigger::HarmonyTrigger() {
  _channelMode=AnyChannel;
  _noteMode=AnyNote;
  _channel=0;
  _noteMin=0;
  _noteMax=127;
  _note=64;
}

bool HarmonyTrigger::checkTrigger(MidiEvent ev) {
  int msg=ev.data[0] & 0xF0;
  int chan=ev.data[0] & 0x0F;

  if ((_channelMode == SingleChannel) && (chan != _channel))
    return false;
  if ((msg != MIDI_NOTE_ON) && (msg != MIDI_NOTE_OFF))
    return false;
  switch (_noteMode) {
  case AnyNote:
    return true;
    break;
  case NoteRange:
    if ((ev.data[1] >= _noteMin) && (ev.data[1] <= _noteMax))
      return true;
    return false;
    break;
  case SingleNote:
    if (ev.data[1] == _note)
      return true;
    return false;
    break;
  default:
    return false;
    break;
  }
}
QDomElement HarmonyTrigger::toXML() {
  QDomDocument conf;
  QDomElement root, chan, note;

  root=conf.createElement("HarmonyTrigger");

  chan=conf.createElement("channel");
  chan.setAttribute("mode", _channelMode);
  chan.setAttribute("number", _channel);
  root.appendChild(chan);
  
  note=conf.createElement("note");
  note.setAttribute("mode", _noteMode);
  note.setAttribute("min", _noteMin);
  note.setAttribute("max", _noteMax);
  note.setAttribute("value", _note);
  root.appendChild(note);
  
  return root;
}
void HarmonyTrigger::fromXML(QDomElement root) {
  if (root.tagName() != "HarmonyTrigger")
    return;
  QDomElement elt;
  QDomNodeList p;

  p=root.elementsByTagName("channel");
  if (!p.length())
    return;  
  elt=p.item(0).toElement();
  _channelMode=(HarmonyTrigger::ChannelMode)elt.attribute("mode").toInt();
  _channel=elt.attribute("number").toInt();
  
  p=root.elementsByTagName("note");
  if (!p.length())
    return;  
  elt=p.item(0).toElement();
  _noteMode=(HarmonyTrigger::NoteMode)elt.attribute("mode").toInt();
  _noteMin=elt.attribute("min").toInt();
  _noteMax=elt.attribute("max").toInt();
  qWarning()<<(int)_noteMax;
  _note=elt.attribute("value").toInt();
}
//
// HarmonyTriggerWidget
//
HarmonyTriggerWidget::HarmonyTriggerWidget() {
  _listWidgetItem=0;
  setupGui();
}
void HarmonyTriggerWidget::setupGui() {
  QVBoxLayout *layout=new QVBoxLayout;
  QHBoxLayout *hl=new QHBoxLayout;
  hl->addWidget(new QLabel("Channel"));
  _triggerChannel=new QComboBox;
  _triggerChannel->addItem("Any Channel", HarmonyTrigger::AnyChannel);
  for (int i=0; i < 16; i++)
    _triggerChannel->addItem(QString("Channel %1").arg(i+1), i);
  hl->addWidget(_triggerChannel);
  layout->addLayout(hl);
  
  hl=new QHBoxLayout;
  hl->addWidget(new QLabel("Note"));
  _triggerNoteMode=new QComboBox;
  _triggerNoteMode->addItem("Any Note", HarmonyTrigger::AnyNote);
  _triggerNoteMode->addItem("Note Range", HarmonyTrigger::NoteRange);
  _triggerNoteMode->addItem("Single Note", HarmonyTrigger::SingleNote);
  hl->addWidget(_triggerNoteMode);
  //layout->addLayout(hl);
  
  //hl=new QHBoxLayout;
  _note=new QSpinBox;
  _note->setRange(1, 127);
  _noteMin=new QSpinBox;
  _noteMin->setRange(1, 127);
  _noteMax=new QSpinBox;
  _noteMax->setRange(1, 127);
  hl->addWidget(_note);
  hl->addWidget(_noteMin);
  hl->addWidget(_noteMax);
  layout->addLayout(hl);
  _note->hide();
  _noteMin->hide();
  _noteMax->hide();
  
  setLayout(layout);

  connect(_triggerChannel, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(channelChanged(int)));
  connect(_triggerNoteMode, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(noteModeChanged(int)));
  connect(_note, SIGNAL(valueChanged(int)),
	  this, SLOT(noteChanged(int)));
  connect(_noteMin, SIGNAL(valueChanged(int)),
	  this, SLOT(noteMinChanged(int)));
  connect(_noteMax, SIGNAL(valueChanged(int)),
	  this, SLOT(noteMaxChanged(int)));
}
void HarmonyTriggerWidget::updateUI() {
  if (_trigger.channelMode() == HarmonyTrigger::AnyChannel)
    _triggerChannel->setCurrentIndex(0);
  else
    _triggerChannel->setCurrentIndex(_triggerChannel->findData(_trigger.channel()));

  _triggerNoteMode->setCurrentIndex(_triggerNoteMode->findData(_trigger.noteMode()));
  
  _note->blockSignals(true);
  _noteMin->blockSignals(true);
  _noteMax->blockSignals(true);

  _note->setValue(_trigger.note());

  _noteMin->setRange(0, 127);
  _noteMax->setRange(0, 127);

  _noteMin->setValue(_trigger.noteMin());
  _noteMax->setValue(_trigger.noteMax());

  _noteMin->setRange(0, _trigger.noteMax());
  _noteMax->setRange(_trigger.noteMin(), 127);

  _noteMin->blockSignals(false);
  _noteMax->blockSignals(false);
  _note->blockSignals(false);
}
void HarmonyTriggerWidget::channelChanged(int idx) {
  int cc=_triggerChannel->itemData(idx).toInt();
  if (cc == HarmonyTrigger::AnyChannel) {
    _trigger.setChannelMode(HarmonyTrigger::AnyChannel);
  }
  else {
    _trigger.setChannelMode(HarmonyTrigger::SingleChannel);
    _trigger.setChannel(cc);
  }
}
void HarmonyTriggerWidget::noteModeChanged(int idx) {
  HarmonyTrigger::NoteMode nm=(HarmonyTrigger::NoteMode)_triggerNoteMode->itemData(idx).toInt();
  _trigger.setNoteMode(nm);
  switch (nm) {
  case HarmonyTrigger::AnyNote:
    _note->hide();
    _noteMin->hide();
    _noteMax->hide();
    break;
  case HarmonyTrigger::NoteRange:
    _note->hide();
    _noteMin->show();
    _noteMax->show();
    break;
  case HarmonyTrigger::SingleNote:
    _note->show();
    _noteMin->hide();
    _noteMax->hide();
    break;
  }
  if (_listWidgetItem)
    _listWidgetItem->setSizeHint(sizeHint());
}
void HarmonyTriggerWidget::noteChanged(int val) {
  _trigger.setNote(val);
}
void HarmonyTriggerWidget::noteMinChanged(int val) {
  _trigger.setNoteMin(val);
  _noteMax->setRange(val, 127);
}
void HarmonyTriggerWidget::noteMaxChanged(int val) {
  _trigger.setNoteMax(val);
  _noteMin->setRange(0, val);
}

// 
// HarmonyTrigger.cpp ends here
