// jack_engine.h --- 
// 
// Filename: jack_engine.h
// Description: 
// Author: stax
// Maintainer: 
// Created: sam. janv. 23 18:23:25 2016 (+0100)
// Version: 
// Package-Requires: ()
// Last-Updated: jeu. déc. 21 09:27:09 2017 (+0100)
//           By: stax
//     Update #: 30
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

#ifndef JACK_ENGINE_H
#define JACK_ENGINE_H

#include <QtGui>
#include <libqjack/jack_client.h>
#include "Harmony.h"

class JackEngine : public JackClient
{
Q_OBJECT

private:
  HarmonyList &_harmonies;
  
  virtual void audio_process(jack_nframes_t);

  
public:
  JackEngine(HarmonyList &hl);
  ~JackEngine();
  
  bool initEngine(QString name);

						     
public slots:
  void handleMidiEvent(struct MidiEvent);

signals:
  void doRollArpeggios(jack_time_t);
};



#endif

// 
// jack_engine.h ends here
