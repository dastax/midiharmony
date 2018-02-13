// jack_engine.cpp --- 
// 
// Filename: jack_engine.cpp
// Description: 
// Author: stax
// Maintainer: 
// Created: sam. janv. 23 18:26:34 2016 (+0100)
// Version: 
// Package-Requires: ()
// Last-Updated: jeu. déc. 21 09:26:35 2017 (+0100)
//           By: stax
//     Update #: 75
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

#include <math.h>
#include "jack_engine.h"

JackEngine::JackEngine(HarmonyList &hl) : _harmonies(hl) {
  connect(this, SIGNAL(receivedMidiEvent(MidiEvent)),
  	  this, SLOT(handleMidiEvent(MidiEvent)));
}
JackEngine::~JackEngine() {
}

bool JackEngine::initEngine(QString name)
{
  if (initClient(name)) {
    createMidiIn("midi_in");
    addMidiOutput("midi_out");
    return true;
  }

  return false;
}
void JackEngine::handleMidiEvent(MidiEvent ev)
{
  int msg=ev.data[0] & 0xF0;
  //int chan=ev.data[0] & 0x0F;
    
  if((msg != MIDI_NOTE_ON) && (msg != MIDI_NOTE_OFF))// jus' route
    sendMidiEvent("midi_out", ev);
}
void JackEngine::audio_process(jack_nframes_t nframes) {
  // check tiuming for note throw
  jack_time_t curtime=jack_get_time();

  //for (int i=0; i < _harmonies.count(); i++)
    emit doRollArpeggios(curtime);
  //_harmonies.at(i)->rollArpeggios(curtime);
  
  //qWarning("nframes %d", pos);
}
/*
void JackEngine::midi_out_process(jack_nframes_t nframes)
{
  portmap_it_t i=_midiOutputs.constBegin();
  while (i != _midiOutputs.constEnd()) {
    struct MidiEvent ev;
    int read, t;
    unsigned char* buffer ;
    jack_nframes_t last_frame_time=jack_last_frame_time(_client);
    void *port_buf=jack_port_get_buffer(i.value(), nframes);
    jack_ringbuffer_t *rb=_ringBuffers[i.key()];

    jack_midi_clear_buffer(port_buf);
    while (jack_ringbuffer_read_space(rb)) {
      read=jack_ringbuffer_peek(rb, (char *)&ev, sizeof(ev));
      if (read != sizeof(ev)) {
	qWarning()<<" Short read from the ringbuffer, possible note loss.";
	jack_ringbuffer_read_advance(rb, read);
	continue;
      }
      else {
	t=ev.time + nframes - last_frame_time;
	if (t >= (int)nframes)
	  break;
	if (t < 0)
	  t=0;
	jack_ringbuffer_read_advance(rb, sizeof(ev));
	buffer=jack_midi_event_reserve(port_buf, t, ev.size);
	if (buffer != NULL)
	  memcpy(buffer, ev.data, ev.size);
      }
    }
    i++;
  }
}
*/
// 
// jack_engine.cpp ends here
