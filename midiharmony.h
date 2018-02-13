#ifndef MIDIHARMONY_H
#define MIDIHARMONY_H

#include <QtGui>
#include <libqjack/presetbox.h>

#include "Harmony.h"

#include "jack_engine.h"

Q_DECLARE_METATYPE(QList<int>)


class MidiHarmony : public QWidget
{
  Q_OBJECT
private:
  JackEngine _jack;
  HarmonyList _harmonies;

  Harmony *_currentHarmony;
  QString _currentFile;

  // GUI
  QLabel *_staxaudio;

  QMenuBar *_menuBar;
  QAction *_saveAction;
  
  QListWidget *_triggerList;
  QPushButton *_addTrigger;
  QPushButton *_delTrigger;
  
  QVBoxLayout *_layout;
  QGroupBox *_jackGB;
  QLineEdit *_jackClientName;
  QPushButton *_jackClientActivation;
  QGroupBox *_harmonyGB;
  QComboBox *_harmonyList;

  QPushButton *_modeBut;
  
  QGroupBox *_arpeggiatorGB;

  QGroupBox *_arpTimeGB;
  QSpinBox *_arpBPM;
  QComboBox *_arpConstantSpeed;
  QComboBox *_arpNoteSpeed;
  QLineEdit *_arpCustomTimes;
  PresetManager *_timePatternManager;
  
  QGroupBox *_arpHarmonyGB;
  QComboBox *_arpIntervals;
  QPushButton *_learnIntervals;
  PresetManager *_patternManager;
  QSpinBox *_arpSlideRange;
  QLineEdit *_arpCustomIntervals;

  QGroupBox *_arpPlayModeGB;
  QComboBox *_arpTrigger;
  QComboBox *_arpMode;


  QString _confFile;
  QString _presetDirectory;
  
  int _mode;
  int _learnFirstNote;
  
  void setUpGui();

  bool loadHarmonyDB(QString path);

  void setCurrentHarmony(Harmony *h);
  
  bool saveConf();
  bool loadConf();

  void closeEvent(QCloseEvent*);


private slots:
  void newFile();
  void loadFile();
  void loadFile(QString path);
  void saveFile();
  void saveFileAs();
  void saveFile(QString path);
  
  void jackCtrlClicked();
  void addTrigger();
  void delTrigger();
  void triggerSelectionChanged(QListWidgetItem*, QListWidgetItem*);
  void toggleMode();
  void selectHarmony(int);
  void harmonyChanged(int);
  void arpTriggerChanged(int);
  void arpModeChanged(int);
  void arpCustomIntervalsChanged(QString);
  void arpCustomTimesChanged(QString);
  void arpIntervalsChanged(int);
  void arpSlideRangeChanged(int);
  void arpBPMChanged(int);
  void arpNoteSpeedChanged(int);
  void arpConstantSpeedChanged(int);
  void savePatternRequested(int index, QString name);
  void curPatternChanged(QDomElement pr);
  void saveTimePatternRequested(int index, QString name);
  void curTimePatternChanged(QDomElement pr);
  void learnIntervalsClicked();
  void learnMidiIn(MidiEvent ev);

      
public:
  MidiHarmony(QString db_path, QString name);
  ~MidiHarmony();
};


#endif
