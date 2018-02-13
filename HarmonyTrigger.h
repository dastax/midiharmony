// HarmonyTrigger.h --- 
// 
// Filename: HarmonyTrigger.h
// Description: 
// Author: stax
// Maintainer: 
// Created: dim. déc. 10 19:05:42 2017 (+0100)
// Version: 
// Package-Requires: ()
// Last-Updated: jeu. déc. 14 21:42:20 2017 (+0100)
//           By: stax
//     Update #: 19
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

#ifndef HARMONY_TRIGGER_H
#define HARMONY_TRIGGER_H

#include <QtGui>
#include <libqjack/jack_client.h>

class HarmonyTrigger : public QObject {
  Q_OBJECT
  Q_ENUMS(TriggerChannel)
  Q_ENUMS(TriggerNoteMode)
  
public:
  enum ChannelMode {AnyChannel=17, SingleChannel};
  enum NoteMode {AnyNote, NoteRange, SingleNote};

private:
  ChannelMode _channelMode;
  NoteMode _noteMode;

  char _channel;
  char _noteMin, _noteMax;
  char _note;

public:
  HarmonyTrigger();

  void setChannelMode(ChannelMode tcm) {_channelMode=tcm;}
  void setNoteMode(NoteMode tnm) {_noteMode=tnm;}

  void setChannel(char chan) {_channel=chan;}
  void setNoteMin(char nm) {_noteMin=nm;}
  void setNoteMax(char nm) {_noteMax=nm;}
  void setNote(char n) {_note=n;}

  ChannelMode channelMode() {return _channelMode;}
  NoteMode noteMode() {return _noteMode;}
  char channel() {return _channel;}
  char noteMin() {return _noteMin;}
  char noteMax() {return _noteMax;}
  char note() {return _note;}
  
  bool checkTrigger(MidiEvent me);

  QDomElement toXML();
  void fromXML(QDomElement);
};

//
// HarmonyTriggerWidget
//
class HarmonyTriggerWidget : public QWidget {
  Q_OBJECT
  
private:
  QComboBox *_triggerChannel;
  QComboBox *_triggerNoteMode;
  QSpinBox *_noteMin, *_noteMax, *_note;

  HarmonyTrigger _trigger;

  QListWidgetItem *_listWidgetItem;

  void setupGui();

private slots:
  void channelChanged(int);
  void noteModeChanged(int);
  void noteMinChanged(int);
  void noteMaxChanged(int);
  void noteChanged(int);
  
public:
  HarmonyTriggerWidget();

  void setListWidgetItem(QListWidgetItem *lwi) {_listWidgetItem=lwi;}
  HarmonyTrigger *getTrigger() {return &_trigger;}

  void updateUI();
};

#endif

// 
// HarmonyTrigger.h ends here
