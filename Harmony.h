// Harmony.h --- 
// 
// Filename: Harmony.h
// Description: 
// Author: stax
// Maintainer: 
// Created: lun. déc. 11 00:39:43 2017 (+0100)
// Version: 
// Package-Requires: ()
// Last-Updated: jeu. déc. 21 09:40:15 2017 (+0100)
//           By: stax
//     Update #: 34
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

#ifndef HARMONY_H
#define HARMONY_H

#include <QtGui>
#include <libqjack/jack_client.h>
#include "HarmonyTrigger.h"

#define MODE_CHORD		0
#define MODE_ARPEGGIATOR	1

#define MAX_NOTE	127

#define ARPEG_MODE_ASC		0
#define ARPEG_MODE_DESC		1
#define ARPEG_MODE_LOOP_ASC    	2
#define ARPEG_MODE_LOOP_DESC   	3
#define ARPEG_MODE_RANDOM   	4

#define ARPEG_INTERVALS_HARMONY		0
#define ARPEG_INTERVALS_SLIDEUP		1
#define ARPEG_INTERVALS_SLIDEDOWN    	2
#define ARPEG_INTERVALS_CUSTOM    	3

#define TRIGGER_FULL		0
#define TRIGGER_TILRELEASE     	1
#define TRIGGER_LOOP_TILRELEASE	2
#define TRIGGER_LOOP_TOGGLE    	3

Q_DECLARE_METATYPE(jack_time_t)

struct harmony_gui_state_t {
  int harmonyIndex;
  int arpIntervalsIndex;
  int arpSlideRange;
  QString arpCustomIntervals;
  QString arpCustomTimes;
  int arpPatternIndex;
  int arpTimePatternIndex;
};
struct harmony_params_t{
  int mode;// Chord or Arp
  QList<int> intervals;
  int arpBPM, arpSpeed;
  bool arpConstantSpeed;
  QList<int> arpCustomTimes;
  int arpMode;// asc, desc, loop[asc/desc], random
  int arpTrigger;// full, tilrelease, loop[tilrelease/toggle]
};
struct arpeggio_t{
  MidiEvent event;
  harmony_params_t params;
  int previousNote;
  int currentNoteIndex;
  int currentTimeIndex;
  int noteCount;
  jack_time_t duration;
  jack_time_t triggerTime;
};

class Harmony : public QObject {
  Q_OBJECT
private:
  JackClient &_jack;
  
  HarmonyTrigger *_trigger;
  harmony_gui_state_t _guiState;
  harmony_params_t _params;

  // used in chord mode
  QHash<int, uint16_t> _playedNotes;// key:playing note, value:trigger note
  // used in arpeggio mode
  QList<arpeggio_t> _arpeggios;

  // arpeggios funcs
  void addArpeggio(MidiEvent ev, harmony_params_t cp);
  void stopArpeggio(int chan, int note);
  void stopArpeggio(int index);
  void stopArpeggios();
  int getArpeggioIndex(int chan, int note);				       
  void arpeggioRoll(int i);

  // chords funcs
  void stopChords();

  /*
private slots:
  void manageArpeggio(int cmd, int index);
  */
public:
  Harmony(HarmonyTrigger *ht, JackClient &j);

  // getters
  HarmonyTrigger *getTrigger() {return _trigger;}
  harmony_gui_state_t guiState() {return _guiState;}
  harmony_params_t params() {return _params;}
  
  // params setters
  void setMode(int mode);
  void setIntervals(QList<int> i) {_params.intervals=i;}
  void setArpBPM(int bpm) {_params.arpBPM=bpm;}
  void setArpSpeed(int s) {_params.arpSpeed=s;}
  void setConstantSpeed(bool s) {_params.arpConstantSpeed=s;}
  void setArpCustomTimes(QList<int> i) {_params.arpCustomTimes=i;}
  void setArpMode(int m) {_params.arpMode=m;}
  void setArpTrigger(int t) {_params.arpTrigger=t;}

  // gui state setters
  void setHarmonyIndex(int hi) {_guiState.harmonyIndex=hi;}
  void setArpIntervalsIndex(int aii) {_guiState.arpIntervalsIndex=aii;}
  void setArpSlideRange(int asr) {_guiState.arpSlideRange=asr;}
  void setArpCustomIntervals(QString aci) {_guiState.arpCustomIntervals=aci;}
  void setArpCustomTimes(QString act) {_guiState.arpCustomTimes=act;}
  void setArpPatternIndex(int api) {_guiState.arpPatternIndex=api;}
  void setArpTimePatternIndex(int atpi) {_guiState.arpTimePatternIndex=atpi;}

					 
  void stopNotes() {if (_params.mode == MODE_CHORD) stopChords(); else stopArpeggios();}

  static QString intervalsToString(QList<int> intervals);
  static QList<int> intervalsFromString(QString str);

  QDomElement toXML();
  void fromXML(QDomElement);
			   
public slots:
  void handleMidiEvent(MidiEvent ev);
  void rollArpeggios(jack_time_t curtime);

};

typedef QList<Harmony*> HarmonyList;

#endif

// 
// Harmony.h ends here
