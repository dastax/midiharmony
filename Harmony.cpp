// Harmony.cpp --- 
// 
// Filename: Harmony.cpp
// Description: 
// Author: stax
// Maintainer: 
// Created: lun. déc. 11 19:02:04 2017 (+0100)
// Version: 
// Package-Requires: ()
// Last-Updated: jeu. déc. 21 09:29:29 2017 (+0100)
//           By: stax
//     Update #: 33
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

#include "Harmony.h"

Harmony::Harmony(HarmonyTrigger *ht, JackClient &j) : _jack(j) {
  _trigger=ht;

  _guiState.harmonyIndex=0;
  _guiState.arpIntervalsIndex=0;
  _guiState.arpSlideRange=8;
  _guiState.arpCustomIntervals="0";
  _guiState.arpCustomTimes="1";
  _guiState.arpPatternIndex=-1;
  _guiState.arpTimePatternIndex=-1;

  _params.mode=MODE_CHORD;
  _params.intervals<<0;
  _params.arpBPM=120;
  _params.arpSpeed=1;
  _params.arpConstantSpeed=true;
  _params.arpCustomTimes<<1;
  _params.arpTrigger=TRIGGER_FULL;
  _params.arpMode=ARPEG_MODE_ASC;

  
  connect(&_jack, SIGNAL(receivedMidiEvent(MidiEvent)),
	  this, SLOT(handleMidiEvent(MidiEvent)));
  connect(&_jack, SIGNAL(doRollArpeggios(jack_time_t)),
	  this, SLOT(rollArpeggios(jack_time_t)));
}
void Harmony::setMode(int mode) {
  _params.mode=mode;
  if (mode == MODE_CHORD)
    stopArpeggios();
  else
    stopChords();
}
void Harmony::handleMidiEvent(MidiEvent ev) {
  if (_trigger->checkTrigger(ev)) {
    unsigned char msg=ev.data[0] & 0xF0;
    unsigned char chan=ev.data[0] & 0x0F;

    // BYPASS
    if (_params.intervals.size() <= 1) {
      _jack.sendMidiEvent("midi_out", ev);
      return;
    }
    
    unsigned char fondamental=ev.data[1];
    //apply harmony = )
    if (_params.mode == MODE_CHORD) { // Chord
      if (msg == MIDI_NOTE_ON) { // NOTE ON
	foreach (int gap, _params.intervals) {
	  int note=fondamental + gap;
	  if (note < MAX_NOTE) {
	    ev.data[1]=note;
	    _jack.sendMidiEvent("midi_out", ev);
	    _playedNotes.insert(note, (uint16_t)(fondamental << 8) + chan);
	  }
	}
      }
      else { // NOTE OFF
	uint16_t  initier=(uint16_t)(fondamental << 8) + chan;
	QMutableHashIterator<int, uint16_t> i(_playedNotes);
	while (i.findNext(initier)) {
	  ev.data[1]=i.key();
	  _jack.sendMidiEvent("midi_out", ev);
	  i.remove();
	}
      }	
    }
    else{// arpeggiator
      if (msg == MIDI_NOTE_ON) { // NOTE ON
	
	if (_params.arpTrigger == TRIGGER_LOOP_TOGGLE) {
	  int idx=getArpeggioIndex(chan, fondamental);
	  if (idx >= 0)
	    stopArpeggio(idx);
	  else
	    addArpeggio(ev, _params);
	}
	else
	  addArpeggio(ev, _params);
	
      }
      else { // NOTE OFF
	int ai=getArpeggioIndex(chan, fondamental);
	if (ai >= 0)
	  if ((_arpeggios.at(ai).params.arpTrigger == TRIGGER_TILRELEASE) ||
	      (_arpeggios.at(ai).params.arpTrigger == TRIGGER_LOOP_TILRELEASE))
	    stopArpeggio(ai);
	
      }
    }	
  }
}
QString Harmony::intervalsToString(QList<int> intervals) {
  QString intstr="";
  for (int j=0; j < intervals.count(); j++) {
    if (j > 0)
      intstr+=",";
    if (intervals.at(j) == 1000)
      intstr+="_";
    else
      intstr+=QString::number(intervals.at(j));
  }
  return intstr;
}
QList<int> Harmony::intervalsFromString(QString str){
  QList<int> intervals;
  QStringList svals=str.split(',');
  for (int i=0; i < svals.count(); i ++) {
    if (svals[i] != "_")
      intervals.append(svals[i].toInt());
    else
      intervals.append(1000);
  }
  return intervals;
}

QDomElement Harmony::toXML() {
  QDomDocument conf;
  QDomElement root, params, guistate;

  root=conf.createElement("Harmony");

  root.appendChild(_trigger->toXML());

  params=conf.createElement("params");
  params.setAttribute("mode", _params.mode);
  params.setAttribute("intervals", intervalsToString(_params.intervals));
  params.setAttribute("arpBPM", _params.arpBPM);
  params.setAttribute("arpSpeed", _params.arpSpeed);
  params.setAttribute("arpConstantSpeed", _params.arpConstantSpeed);
  params.setAttribute("arpMode", _params.arpMode);
  params.setAttribute("arpTrigger", _params.arpTrigger);
  root.appendChild(params);

  guistate=conf.createElement("guiState");
  guistate.setAttribute("harmonyIndex", _guiState.harmonyIndex);
  guistate.setAttribute("arpIntervalsIndex", _guiState.arpIntervalsIndex);
  guistate.setAttribute("arpSlideRange", _guiState.arpIntervalsIndex);
  guistate.setAttribute("arpCustomIntervals", _guiState.arpCustomIntervals);
  guistate.setAttribute("arpCustomTimes", _guiState.arpCustomTimes);
  guistate.setAttribute("arpPatternIndex", _guiState.arpPatternIndex);
  guistate.setAttribute("arpTimePatternIndex", _guiState.arpTimePatternIndex);
  root.appendChild(guistate);

  return root;
}
void Harmony::fromXML(QDomElement root) {
  if (root.tagName() != "Harmony")
    return;
  QDomElement elt;
  QDomNodeList p;

  p=root.elementsByTagName("HarmonyTrigger");
  if (!p.length())
    return;  
  elt=p.item(0).toElement();
  _trigger->fromXML(elt);
  
  p=root.elementsByTagName("params");
  if (!p.length())
    return;  
  elt=p.item(0).toElement();
  _params.mode=elt.attribute("mode").toInt();
  _params.intervals=intervalsFromString(elt.attribute("intervals"));
  _params.arpBPM=elt.attribute("arpBPM").toInt();
  _params.arpSpeed=elt.attribute("arpSpeed").toInt();
  _params.arpConstantSpeed=elt.attribute("arpConstantSpeed").toInt();
  _params.arpMode=elt.attribute("arpMode").toInt();
  _params.arpTrigger=elt.attribute("arpTrigger").toInt();

  p=root.elementsByTagName("guiState");
  if (!p.length())
    return;  
  elt=p.item(0).toElement();
  _guiState.harmonyIndex=elt.attribute("harmonyIndex").toInt();
  _guiState.arpIntervalsIndex=elt.attribute("arpIntervalsIndex").toInt();
  _guiState.arpIntervalsIndex=elt.attribute("arpSlideRange").toInt();
  _guiState.arpCustomIntervals=elt.attribute("arpCustomIntervals");
  _guiState.arpCustomTimes=elt.attribute("arpCustomTimes");
  _guiState.arpPatternIndex=elt.attribute("arpPatternIndex").toInt();
  _guiState.arpTimePatternIndex=elt.attribute("arpTimePatternIndex").toInt();

  _params.arpCustomTimes=intervalsFromString(_guiState.arpCustomTimes);

}
void Harmony::addArpeggio(MidiEvent ev, harmony_params_t cp) {
  if (cp.intervals.size() == 0)
    return;

  arpeggio_t arp;

  arp.event=ev;
  arp.params=cp;

  arp.currentNoteIndex=0;
  arp.currentTimeIndex=0;
  arp.noteCount=0;
  arp.triggerTime=jack_get_time();
  
  arp.duration=60000000 / cp.arpBPM;
  if (cp.arpSpeed > 0)
    arp.duration/=cp.arpSpeed;
  else if (cp.arpSpeed < 0)
    arp.duration*=(-cp.arpSpeed);
  if (cp.arpConstantSpeed && cp.arpCustomTimes.size() == 0)
    return;
  
  switch (cp.arpMode) {
  case ARPEG_MODE_ASC:
  case ARPEG_MODE_LOOP_ASC:
    arp.currentNoteIndex=0;
    break;
  case ARPEG_MODE_DESC:
  case ARPEG_MODE_LOOP_DESC:
    arp.currentNoteIndex=cp.intervals.size() -1;
    break;
  case ARPEG_MODE_RANDOM:
    arp.noteCount=0;
    qsrand(QDateTime::currentDateTime().toTime_t());
    arp.currentNoteIndex=qrand() % (cp.intervals.size());
    break;
  }
  arp.previousNote=-1;

  _arpeggios.append(arp);
}

int Harmony::getArpeggioIndex(int chan, int note) {
  int evt=MIDI_NOTE_ON | chan;
  for (int i=0; i < _arpeggios.count(); i++) {
    arpeggio_t arp=_arpeggios.at(i);
    if ((arp.event.data[0] == evt) &&
	(arp.event.data[1] == note))
      return i;
  }
  return -1;
}

void Harmony::stopArpeggio(int chan, int note) {
  int i=getArpeggioIndex(chan, note);

  if (i < 0)
    return;
  stopArpeggio(i);
}

void Harmony::stopArpeggio(int index) {
  if ((index < 0) || (index >= _arpeggios.count()))
    return;
  
  arpeggio_t arp=_arpeggios.at(index);
  if (arp.previousNote >= 0) {
    MidiEvent ev;
    ev.size=3;
    ev.data[0]=MIDI_NOTE_OFF | (arp.event.data[0]  & 0x0F);
    ev.data[1]=arp.previousNote;
    ev.data[2]=0;
    _jack.sendMidiEvent("midi_out", ev);
  }
  _arpeggios.removeAt(index);
}

void Harmony::stopArpeggios() {
  QMutableListIterator<arpeggio_t> i(_arpeggios);
  while (i.hasNext()) {
    arpeggio_t cur=i.next();
    if (cur.previousNote >= 0) {
      MidiEvent ev;
      ev.size=3;
      ev.data[0]=MIDI_NOTE_OFF | (cur.event.data[0]  & 0x0F);
      ev.data[1]=cur.previousNote;
      ev.data[2]=0;
      _jack.sendMidiEvent("midi_out", ev);
    }
    i.remove();
  }
}
void Harmony::stopChords() {
  QMutableHashIterator<int, uint16_t> i(_playedNotes);
  while (i.hasNext()) {
    i.next();
    MidiEvent ev;
    ev.data[0]=MIDI_NOTE_OFF | (i.value() & 0xFF);
    ev.data[1]=i.key();
    ev.data[2]=0;
    ev.size=3;
    _jack.sendMidiEvent("midi_out", ev);
    i.remove();
  }
}
void Harmony::rollArpeggios(jack_time_t curtime) {
  /*
  QList<arpeggio_t>::iterator i;
  for (i=list.begin(); i != list.end(); i++) {
    if ((*i).triggerTime <= curtime) {
      //qWarning("precision %d usec", curtime - _arpeggios.at(i).triggerTime);
      arpeggioRoll(i);
    }
  */
  for (int i=0; i < _arpeggios.count(); i++) {
    if (_arpeggios.at(i).triggerTime <= curtime) {
      //qWarning("precision %d usec", curtime - _arpeggios.at(i).triggerTime);
      arpeggioRoll(i);
    }
  }
}
void Harmony::arpeggioRoll(int i) {
  arpeggio_t arp=_arpeggios.at(i);

  int note=arp.event.data[1];
  MidiEvent ev;
  ev.size=3;
  if (arp.previousNote >= 0) {
    ev.data[0]=MIDI_NOTE_OFF | (arp.event.data[0]  & 0x0F);
    ev.data[1]=arp.previousNote;
    ev.data[2]=0;
    _jack.sendMidiEvent("midi_out", ev);
  }
  
  int trigger=arp.params.arpTrigger;

  switch (arp.params.arpMode) {
  case ARPEG_MODE_ASC:
    if (arp.currentNoteIndex == arp.params.intervals.size()){
      if ((trigger == TRIGGER_FULL) || (trigger == TRIGGER_TILRELEASE)) {
	_arpeggios.removeAt(i);
	return;
      }
      arp.currentNoteIndex=0;
    }
    note+=arp.params.intervals.at(arp.currentNoteIndex);
    arp.currentNoteIndex++;
    break;
  case ARPEG_MODE_DESC:
    if (arp.currentNoteIndex == -1){
      if ((trigger == TRIGGER_FULL) || (trigger == TRIGGER_TILRELEASE)) {
	_arpeggios.removeAt(i);
	return;
      }
      arp.currentNoteIndex=arp.params.intervals.size() -1;
    }
    note+=arp.params.intervals.at(arp.currentNoteIndex);
    arp.currentNoteIndex--;
    break;
  case ARPEG_MODE_LOOP_ASC:
    if (arp.currentNoteIndex == arp.params.intervals.size() - 2){
      if ((trigger == TRIGGER_FULL) || (trigger == TRIGGER_TILRELEASE))
	arp.params.arpMode=ARPEG_MODE_DESC;
      else
	arp.params.arpMode=ARPEG_MODE_LOOP_DESC;
    }
    note+=arp.params.intervals.at(arp.currentNoteIndex);
    arp.currentNoteIndex++;
    break;
  case ARPEG_MODE_LOOP_DESC:
    if (arp.currentNoteIndex == 1){
      if ((trigger == TRIGGER_FULL) || (trigger == TRIGGER_TILRELEASE))
	arp.params.arpMode=ARPEG_MODE_ASC;
      else
	arp.params.arpMode=ARPEG_MODE_LOOP_ASC;	
    }
    note+=arp.params.intervals.at(arp.currentNoteIndex);
    arp.currentNoteIndex--;
    break;
  case ARPEG_MODE_RANDOM:
    if ((arp.noteCount == arp.params.intervals.size()) &&
	((trigger == TRIGGER_FULL) || (trigger == TRIGGER_TILRELEASE))) {
      _arpeggios.removeAt(i);
      return;
    }
    note+=arp.params.intervals.at(qrand() % (arp.params.intervals.size()));
    arp.noteCount++;
    break;
  }

  if (note < 0 || note > 127)// silence
    arp.previousNote=-1;
  else {
    ev.data[0]=arp.event.data[0];
    ev.data[1]=note;
    ev.data[2]=arp.event.data[2];
    _jack.sendMidiEvent("midi_out", ev);
    arp.previousNote=note;
  }
  if (arp.params.arpConstantSpeed == false) {
    int speed=arp.params.arpCustomTimes.at(arp.currentTimeIndex);
    if (speed > 0)
      arp.triggerTime+=arp.duration / speed;
    else if (speed < 0)
      arp.triggerTime+=arp.duration * (-speed);
    
    arp.currentTimeIndex=(arp.currentTimeIndex + 1) % arp.params.arpCustomTimes.size();
  }
  else
    arp.triggerTime+=arp.duration;
  
  _arpeggios.replace(i, arp);     
}


// 
// Harmony.cpp ends here
