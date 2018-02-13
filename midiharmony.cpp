#include <QtXml>
#include "midiharmony.h"

MidiHarmony::MidiHarmony(QString db_path, QString name) : _jack(_harmonies) {
  _presetDirectory=QDir::homePath(); 
  _learnFirstNote=-1;
  
  setUpGui();

  setCurrentHarmony(0);

  _jackClientName->setText(name);
  
  if (!loadHarmonyDB(db_path)) {
    QMessageBox::warning(this, tr("Error"), 
			 tr("Error loading harmony database..."));
  }

  _patternManager->setPresetFile("MidiHarmonyPatterns", QDir::homePath()+"/.midiharmony_patterns");
  _patternManager->loadPresets();
  _timePatternManager->setPresetFile("MidiHarmonyTimePatterns", QDir::homePath()+"/.midiharmony_time_patterns");
  _timePatternManager->loadPresets();

  jackCtrlClicked();

  _confFile=QDir::homePath()+"/.midiharmony";
  loadConf();
  if (!_currentFile.isNull() && !_currentFile.isEmpty())
    loadFile(_currentFile);
}
MidiHarmony::~MidiHarmony() {
}
void MidiHarmony::closeEvent(QCloseEvent*)
{
  //qDebug()<<"closing";
  saveConf();
}
void MidiHarmony::setUpGui()
{
  setWindowTitle("MidiHarmony");
  setWindowIcon(QIcon(QPixmap(":/logo")));

  _menuBar=new QMenuBar();
  QMenu *fileMenu=_menuBar->addMenu(tr("&File"));
  QAction *newAction=new QAction("&New", this);
  QAction *loadAction=new QAction("&Load ...", this);
  _saveAction=new QAction("&Save", this);
  QAction *saveAsAction=new QAction("Save &as ...", this);
  connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
  connect(loadAction, SIGNAL(triggered()), this, SLOT(loadFile()));
  connect(_saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
  connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));
  fileMenu->addAction(newAction);
  fileMenu->addAction(loadAction);
  fileMenu->addAction(_saveAction);
  fileMenu->addAction(saveAsAction);
  _saveAction->setEnabled(false);
     
  _jackClientName=new QLineEdit;
  _jackClientActivation=new QPushButton;

  QHBoxLayout *header=new QHBoxLayout;
  _staxaudio=new QLabel;
  _staxaudio->setPixmap(QPixmap(":/harmony_logo"));
  header->addWidget(_staxaudio);
  header->addStretch();
  _jackGB=new QGroupBox(tr("JackMidi client"));
  QVBoxLayout *out=new QVBoxLayout;
  QHBoxLayout *ch=new QHBoxLayout;
  ch->addWidget(_jackClientName);
  ch->addWidget(_jackClientActivation);
  out->addLayout(ch);
  _jackGB->setLayout(out);
  _jackGB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);//QSizePolicy::Minimum);
  header->addWidget(_jackGB);
  
  _harmonyGB=new QGroupBox("Harmony");
  QVBoxLayout *h=new QVBoxLayout;
  _harmonyList=new QComboBox;
  h->addWidget(_harmonyList);
  _modeBut=new QPushButton(tr("Mode Arpeggiator"));
  h->addWidget(_modeBut);
  QList<int> l;
  l << 0;
  _harmonyList->addItem(tr("[0] ByPass"), QVariant::fromValue<QList<int> >(l));

  _arpeggiatorGB=new QGroupBox("Arpeggiator");
  h->addWidget(_arpeggiatorGB);
  _harmonyGB->setLayout(h);


  QVBoxLayout *al=new QVBoxLayout(_arpeggiatorGB);

  _arpHarmonyGB=new QGroupBox("Harmony");
  al->addWidget(_arpHarmonyGB);
  _arpTimeGB=new QGroupBox("Time");
  al->addWidget(_arpTimeGB);
  _arpPlayModeGB=new QGroupBox("Play mode");
  al->addWidget(_arpPlayModeGB);

  
  QHBoxLayout *ai=new QHBoxLayout;
  _arpIntervals=new QComboBox;
  _arpIntervals->addItem(tr("Harmony"), ARPEG_INTERVALS_HARMONY);
  _arpIntervals->addItem(tr("Slide Up"), ARPEG_INTERVALS_SLIDEUP);
  _arpIntervals->addItem(tr("Slide Down."), ARPEG_INTERVALS_SLIDEDOWN);
  _arpIntervals->addItem(tr("Custom"), ARPEG_INTERVALS_CUSTOM);
  ai->addWidget(new QLabel("Intervals"));
  ai->addWidget(_arpIntervals);
  
  _arpSlideRange=new QSpinBox;
  _arpSlideRange->setRange(2, 32);
  _arpSlideRange->hide();
  _arpSlideRange->setValue(16);
  ai->addWidget(_arpSlideRange);
  
  QVBoxLayout *ai2=new QVBoxLayout;
  _arpCustomIntervals=new QLineEdit;
  QRegExp rex("(_|-?[0-9]{1,2}){1,1}(,_|,-?[0-9]{1,2})*");
  _arpCustomIntervals->setValidator(new QRegExpValidator(rex));
  _arpCustomIntervals->hide();
  ai2->addWidget(_arpCustomIntervals);
  _learnIntervals=new QPushButton("Learn");
  _learnIntervals->hide();
  ai2->addWidget(_learnIntervals);
  _patternManager=new PresetManager;
  _patternManager->hide();
  ai2->addWidget(_patternManager);  
  ai->addLayout(ai2);
  ai->addStretch();
  
  _arpHarmonyGB->setLayout(ai);
  
  QHBoxLayout *as=new QHBoxLayout;
  _arpBPM=new QSpinBox();
  _arpBPM->setRange(40, 360);

  _arpConstantSpeed=new QComboBox();
  _arpConstantSpeed->addItem(tr("Constant speed"));
  _arpConstantSpeed->addItem(tr("Variable speed"));

  _arpCustomTimes=new QLineEdit;
  QRegExp rex2("(-?[0-9]{1,2}){1,1}(,-?[0-9]{1,2})*");
  _arpCustomTimes->setValidator(new QRegExpValidator(rex2));
  _arpCustomTimes->hide();
  _timePatternManager=new PresetManager;
  _timePatternManager->hide();

  
  _arpNoteSpeed=new QComboBox();
  for (int i=-16; i <= 16; i++)
    if (i != -1 && i != 0)
      _arpNoteSpeed->addItem(QString("x %1").arg(i), i);
  
  QVBoxLayout *as2=new QVBoxLayout;
  QHBoxLayout *as3=new QHBoxLayout;

  as3->addWidget(_arpConstantSpeed);
  as3->addWidget(_arpNoteSpeed);
  as3->addStretch();

  as2->addLayout(as3);
  as2->addWidget(_arpCustomTimes);
  as2->addWidget(_timePatternManager);
  
  as->addWidget(new QLabel("BPM"));
  as->addWidget(_arpBPM);
  as->addWidget(new QLabel("Speed: "));
  as->addLayout(as2);
  
  _arpTimeGB->setLayout(as);
  
  QHBoxLayout *am=new QHBoxLayout;
  _arpTrigger=new QComboBox;
  _arpTrigger->addItem(tr("Full"), TRIGGER_FULL);
  _arpTrigger->addItem(tr("Until release"), TRIGGER_TILRELEASE);
  _arpTrigger->addItem(tr("Loop until release"), TRIGGER_LOOP_TILRELEASE);
  _arpTrigger->addItem(tr("Loop toggle"), TRIGGER_LOOP_TOGGLE);
  am->addWidget(new QLabel("Trigger"));
  am->addWidget(_arpTrigger);

  _arpMode=new QComboBox;
  _arpMode->addItem(tr("Asc."), ARPEG_MODE_ASC);
  _arpMode->addItem(tr("Desc."), ARPEG_MODE_DESC);
  _arpMode->addItem(tr("Loop Asc."), ARPEG_MODE_LOOP_ASC);
  _arpMode->addItem(tr("Loop Desc."), ARPEG_MODE_LOOP_DESC);
  _arpMode->addItem(tr("Random"), ARPEG_MODE_RANDOM);
  am->addWidget(new QLabel("Mode"));
  am->addWidget(_arpMode);
  am->addStretch();
  //as->addWidget(new QLabel("Speed"));
  //as->addWidget(_arpNoteSpeed);
  _arpPlayModeGB->setLayout(am);
  
  _arpeggiatorGB->setEnabled(false);
  


  QVBoxLayout *trl=new QVBoxLayout;
  _triggerList=new QListWidget;
  trl->addWidget(_triggerList);
  QHBoxLayout *tral=new QHBoxLayout;
  _addTrigger=new QPushButton(tr("Add Trigger"));
  _delTrigger=new QPushButton(tr("Delete Trigger"));
  _delTrigger->setEnabled(false);
  tral->addWidget(_addTrigger);
  tral->addWidget(_delTrigger);
  trl->addLayout(tral);
  
  
  QHBoxLayout *ml=new QHBoxLayout;
  ml->addLayout(trl);
  ml->addWidget(_harmonyGB);
  
  _layout=new QVBoxLayout;
  _layout->addLayout(header, 1);
  _layout->addWidget(_menuBar);
  _layout->addLayout(ml, 2);
  setLayout(_layout);

  
  connect(_jackClientActivation, SIGNAL(clicked()),
	  this, SLOT(jackCtrlClicked()));
  connect(_addTrigger, SIGNAL(clicked()), this, SLOT(addTrigger()));
  connect(_delTrigger, SIGNAL(clicked()), this, SLOT(delTrigger()));
  connect(_modeBut, SIGNAL(clicked()),
	  this, SLOT(toggleMode()));
  connect(_triggerList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
	  this, SLOT(triggerSelectionChanged(QListWidgetItem*, QListWidgetItem*)));
  connect(_harmonyList, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(harmonyChanged(int)));
  
  connect(_arpBPM, SIGNAL(valueChanged(int)),
	  this, SLOT(arpBPMChanged(int)));
  connect(_arpNoteSpeed, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(arpNoteSpeedChanged(int)));
  
  connect(_arpTrigger, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(arpTriggerChanged(int)));
  connect(_arpMode, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(arpModeChanged(int)));
  connect(_arpIntervals, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(arpIntervalsChanged(int)));
  connect(_arpConstantSpeed, SIGNAL(currentIndexChanged(int)),
	  this, SLOT(arpConstantSpeedChanged(int)));
  connect(_arpSlideRange, SIGNAL(valueChanged(int)),
	  this, SLOT(arpSlideRangeChanged(int)));
  connect(_arpCustomIntervals, SIGNAL(textChanged(QString)),
	  this, SLOT(arpCustomIntervalsChanged(QString)));
  connect(_arpCustomTimes, SIGNAL(textChanged(QString)),
	  this, SLOT(arpCustomTimesChanged(QString)));
  connect(_learnIntervals, SIGNAL(clicked()),
	  this, SLOT(learnIntervalsClicked()));
  connect(_patternManager, SIGNAL(savePresetRequest(int, QString)),
	  this, SLOT(savePatternRequested(int, QString)));
  connect(_patternManager, SIGNAL(presetChanged(QDomElement)),
	  this, SLOT(curPatternChanged(QDomElement)));
  connect(_timePatternManager, SIGNAL(savePresetRequest(int, QString)),
	  this, SLOT(saveTimePatternRequested(int, QString)));
  connect(_timePatternManager, SIGNAL(presetChanged(QDomElement)),
	  this, SLOT(curTimePatternChanged(QDomElement)));

}
void MidiHarmony::learnIntervalsClicked() {
  QString ints=_arpCustomIntervals->text();
  QList<int> intervals;
  intervals << 0;
  _currentHarmony->setIntervals(intervals);
  
  connect(&_jack, SIGNAL(receivedMidiEvent(MidiEvent)),
	  this, SLOT(learnMidiIn(MidiEvent)));
  _arpCustomIntervals->blockSignals(true);
  _arpCustomIntervals->setText("");
  
  int ret=QMessageBox::information(this, tr("Midi Learn"), 
				   tr("Play your midi sequence, then click OK ..."),
				   QMessageBox::Ok|QMessageBox::Cancel);

  disconnect(&_jack, SIGNAL(receivedMidiEvent(MidiEvent)),
	     this, SLOT(learnMidiIn(MidiEvent)));
  _arpCustomIntervals->blockSignals(false);
  _learnFirstNote=-1;

  if (ret == QMessageBox::Cancel) {
    _arpCustomIntervals->setText(ints);
    //intervals=Harmony::intervalsFromString(ints);
    //_currentHarmony->setIntervals(intervals);
    return;
  }

  ints=_arpCustomIntervals->text();
  intervals=Harmony::intervalsFromString(ints);
  _currentHarmony->setIntervals(intervals);
  _currentHarmony->setArpCustomIntervals(ints);
}
void MidiHarmony::learnMidiIn(MidiEvent ev) {
  int msg=ev.data[0] & 0xF0;
  //int chan=ev.data[0] & 0x0F;
    
  if (msg == MIDI_NOTE_ON) {
    if (_learnFirstNote == -1) {
      _learnFirstNote=ev.data[1];
      _arpCustomIntervals->setText("0");
    }
    else {
      QString str=QString("%1,%2").arg(_arpCustomIntervals->text())
	.arg((int)(ev.data[1] - _learnFirstNote));
      _arpCustomIntervals->setText(str);
    }
  }
}
void MidiHarmony::newFile() {
  QListWidgetItem *item;
  HarmonyTriggerWidget *ct;
  Harmony *h;
  
  for (int i=_triggerList->count() -1; i >= 0; i--) {
    item=_triggerList->item(i);
    ct=(HarmonyTriggerWidget*)_triggerList->itemWidget(item);
    h=_harmonies.at(i);
    
    h->stopNotes();
    _harmonies.removeAt(i);
    delete h;
    delete ct;
    delete item;
  }  
  _delTrigger->setEnabled(false);
  setCurrentHarmony(0);
  _saveAction->setEnabled(false);
  setWindowTitle("MidiHarmony");
}
void MidiHarmony::loadFile() {
  QString fileName=QFileDialog::getOpenFileName(this, tr("Open File"),
                                                _presetDirectory,
                                                tr("MidiHarmony Conf file (*.mhc)"));
  if (fileName.isNull())
    return;
  loadFile(fileName);
}
void MidiHarmony::saveFile() {
  if (_currentFile.isNull() || _currentFile.isEmpty())
    return;
  saveFile(_currentFile);
}
void MidiHarmony::saveFileAs() {
  QFileDialog fd;
  fd.setDefaultSuffix("mhc");
  QString fileName=fd.getSaveFileName(this, tr("Open File"),
				      _presetDirectory,
				      tr("MidiHarmony Conf file (*.mhc)"));
  if (fileName.isNull())
    return;
  QFileInfo fi(fileName);
  if (fi.suffix() != "mhc")
    fileName+=".mhc";
  saveFile(fileName);
}
void MidiHarmony::loadFile(QString path) {
  QDomDocument conf;
  QFile f(path);
  if (!f.open(QIODevice::ReadOnly)) {
    QMessageBox::warning(this, tr("Error"), 
			 tr("Error opening %1 file can't be read").arg(path));
    return;
  }
  QString errorStr;
  int errorLine, errorColumn;
  if (!conf.setContent(&f, true, &errorStr, &errorLine, &errorColumn)) {
    QMessageBox::warning(this, tr("Error"), 
			 tr("Parse error at line %1, column %2:%3")
			 .arg(errorLine).arg(errorColumn).arg(errorStr));
    f.close();
    return;
  }
  f.close();
  QDomElement root=conf.documentElement();
  if (root.tagName() != "MidiHarmony") {
    QMessageBox::warning(this, tr("Error"), 
			 tr("The file is not a conf file for this app."));
    return;
  }
  // clear harmonies
  newFile();
  
  QDomNodeList harmonies=conf.elementsByTagName("Harmony");
  if (!harmonies.length()) {
    QMessageBox::warning(this, tr("Error"), 
			 tr("The file is an empty conf !"));
    return;
  }
  for (int i=0; i < harmonies.size(); i++) {
    HarmonyTriggerWidget *nw=new HarmonyTriggerWidget;
    QListWidgetItem *item=new QListWidgetItem();
    nw->setListWidgetItem(item);
    item->setSizeHint(nw->sizeHint());
    _triggerList->addItem(item);
    _triggerList->setItemWidget(item, nw);
    Harmony *h=new Harmony(nw->getTrigger(), _jack);
    _harmonies.append(h);
    h->fromXML(harmonies.item(i).toElement());
    nw->updateUI();
  }
  _triggerList->setCurrentRow(0);
  //setCurrentHarmony(h);
  if (!_delTrigger->isEnabled())
    _delTrigger->setEnabled(true);
  _currentFile=path;
  _saveAction->setEnabled(true);
  setWindowTitle("MidiHarmony - "+_currentFile);
}
void MidiHarmony::saveFile(QString path) {
  QDomDocument conf;
  QDomProcessingInstruction instr;
  instr=conf.createProcessingInstruction("xml", "version=\"1.0\"  encoding=\"UTF-8\"");
  conf.appendChild(instr);
  QDomElement root, node, pnode;
  root=conf.createElement("MidiHarmony");
  root.setAttribute("xmlns", "http://staxaudio.free.fr/mime-info");
  conf.appendChild(root);
  for (int i=0; i < _harmonies.size(); i++)
    root.appendChild(_harmonies[i]->toXML());
  
  QFile f(path);
  if (!f.open(QIODevice::WriteOnly)) {
    QMessageBox::warning(this, tr("Error"), 
			 tr("Error saving %1\n can't create file...").arg(path));
    return;
  }
  QTextStream ts(&f);
  ts << conf.toString();
  f.close();

  _currentFile=path;
  _saveAction->setEnabled(true);
  setWindowTitle("MidiHarmony - "+_currentFile);
}
void MidiHarmony::addTrigger() {
  HarmonyTriggerWidget *nw=new HarmonyTriggerWidget;
  QListWidgetItem *item=new QListWidgetItem();
  nw->setListWidgetItem(item);
  item->setSizeHint(nw->sizeHint());
  _triggerList->addItem(item);
  _triggerList->setItemWidget(item, nw);
  _triggerList->setCurrentItem(item);
  Harmony *h=new Harmony(nw->getTrigger(), _jack);
  _harmonies.append(h);
  setCurrentHarmony(h);
  if (!_delTrigger->isEnabled())
    _delTrigger->setEnabled(true);
}
void MidiHarmony::delTrigger() {
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, tr("Delete trigger"), tr("Delete trigger and attached settings ?"),
                                QMessageBox::Yes|QMessageBox::No);
  if (reply != QMessageBox::Yes)
    return;
  
  QListWidgetItem *item=_triggerList->currentItem();
  HarmonyTriggerWidget *ct=(HarmonyTriggerWidget*)_triggerList->itemWidget(item);
  for (int i=0; i < _harmonies.count(); i++)
    if (_harmonies.at(i)->getTrigger() == ct->getTrigger()) {
      Harmony *h=_harmonies.at(i);
      h->stopNotes();
      _harmonies.removeAt(i);
      delete h;
      delete ct;
      delete item;
      if (_harmonies.size() == 0)
	_delTrigger->setEnabled(false);
      return;
    }  
}
void MidiHarmony::triggerSelectionChanged(QListWidgetItem *cur, QListWidgetItem *prev) {
  qWarning()<<"trigger sel changed";
  HarmonyTriggerWidget *ct=(HarmonyTriggerWidget*)_triggerList->itemWidget(cur);
  for (int i=0; i < _harmonies.count(); i++)
    if (_harmonies.at(i)->getTrigger() == ct->getTrigger()) {
      setCurrentHarmony(_harmonies.at(i));
      return;
    }
}
bool MidiHarmony::loadHarmonyDB(QString path)
{
  QDomDocument conf;
  QFile f(path);
  if (!f.open(QIODevice::ReadOnly))
    return false;
  QString errorStr;
  int errorLine, errorColumn;
  if (!conf.setContent(&f, true, &errorStr, &errorLine, &errorColumn)) {
    qDebug()<<"Parse error at line "<<errorLine<< ", column "<<errorColumn<<":"<<errorStr;
    f.close();
    return false;
  }
  f.close();
  QDomElement root=conf.documentElement();
  if (root.tagName() != "MidiHarmonyDB") {
    qDebug()<<"The file is not a conf file for this app.";
    return false;
  }  
  QDomNodeList chords=conf.elementsByTagName("chord");
  for (uint i=0; i < chords.length(); i++) {
    QDomElement elt=chords.item(i).toElement();
    QDomNodeList notes=elt.elementsByTagName("note");
    QList<int> intervals;
    for (uint j=0; j < notes.length(); j++)
      intervals.append(notes.item(j).toElement().attribute("interval").toInt());
    QVariant var=QVariant::fromValue<QList<int> >(intervals);
    QString label="["+QString::number(i+1)+"] "+elt.attribute("name");
    _harmonyList->addItem(label, var);
  }
  return true;
}
void MidiHarmony::jackCtrlClicked()
{
  if (_jack.isActive()) {
    _jack.closeClient();
    _jackClientName->setEnabled(true);
    _jackClientActivation->setText(tr("Start"));
  }
  else {
    if (_jackClientName->text().isEmpty()) {
      QMessageBox::warning(this, tr("Error"), 
			   tr("Please provide a name for the JackMidi client"));
      return;
    }
    if (_jack.initEngine(_jackClientName->text())) {
      _jackClientName->setEnabled(false);
      _jackClientActivation->setText(tr("Stop"));
    }
    else
      QMessageBox::warning(this, tr("Error"), 
			   tr("Jack failed to initialise..."));
  }

}
void MidiHarmony::setCurrentHarmony(Harmony *h) {
  _currentHarmony=h;
  if (!h) {
    _harmonyGB->setEnabled(false);
    return;
  }
  if (!_harmonyGB->isEnabled())
    _harmonyGB->setEnabled(true);
  
  harmony_params_t p=h->params();
  harmony_gui_state_t gs=h->guiState();

  _arpHarmonyGB->setTitle(QString("Harmony - %1 notes long").arg(p.intervals.size()));

  _mode=p.mode;
  if (p.mode == MODE_ARPEGGIATOR) {
    _arpeggiatorGB->setEnabled(true);
    _modeBut->setText("Mode Chord");
  }
  else {
    _arpeggiatorGB->setEnabled(false);
    _modeBut->setText("Mode Arpeggiator");
  }

  _arpBPM->blockSignals(true);
  _arpBPM->setValue(p.arpBPM);
  _arpBPM->blockSignals(false);
  
  _arpNoteSpeed->blockSignals(true);
  _arpNoteSpeed->setCurrentIndex(_arpNoteSpeed->findData(p.arpSpeed));
  _arpNoteSpeed->blockSignals(false);

  _arpConstantSpeed->blockSignals(true);
  _arpConstantSpeed->setCurrentIndex(p.arpConstantSpeed ? 0 : 1);
  _arpConstantSpeed->blockSignals(false);
  if (p.arpConstantSpeed) {
    _arpCustomTimes->hide();
    _timePatternManager->hide();
  }
  else {
    _arpCustomTimes->show();
    _timePatternManager->show();
  }
  _arpCustomTimes->blockSignals(true);
  _arpCustomTimes->setText(gs.arpCustomTimes);
  _arpCustomTimes->blockSignals(false);


  _arpTrigger->blockSignals(true);
  _arpTrigger->setCurrentIndex(_arpTrigger->findData(p.arpTrigger));
  _arpTrigger->blockSignals(false);

  _arpMode->blockSignals(true);
  _arpMode->setCurrentIndex(_arpMode->findData(p.arpMode));
  _arpMode->blockSignals(false);

  _harmonyList->blockSignals(true);
  _harmonyList->setCurrentIndex(gs.harmonyIndex);
  _harmonyList->blockSignals(false);
  
  _arpIntervals->blockSignals(true);
  _arpIntervals->setCurrentIndex(gs.arpIntervalsIndex);
  _arpIntervals->blockSignals(false);
  switch (_arpIntervals->itemData(gs.arpIntervalsIndex).toInt()) {
  case ARPEG_INTERVALS_HARMONY:
    _arpSlideRange->hide();
    _patternManager->hide();
    _learnIntervals->hide();
    _arpCustomIntervals->hide();
    break;
  case ARPEG_INTERVALS_SLIDEUP:
    _arpCustomIntervals->hide();
    _learnIntervals->hide();
    _patternManager->hide();
    _arpSlideRange->show();
    break;
  case ARPEG_INTERVALS_SLIDEDOWN:
    _arpCustomIntervals->hide();
    _learnIntervals->hide();
    _patternManager->hide();
    _arpSlideRange->show();
    break;
  case ARPEG_INTERVALS_CUSTOM:
    _arpCustomIntervals->show();
    _learnIntervals->show();
    _patternManager->show();
    _arpSlideRange->hide();
    break;
  }
  
  _arpSlideRange->blockSignals(true);
  _arpSlideRange->setValue(gs.arpSlideRange);
  _arpSlideRange->blockSignals(false);
  
  _arpCustomIntervals->blockSignals(true);
  _arpCustomIntervals->setText(gs.arpCustomIntervals);
  _arpCustomIntervals->blockSignals(false);
  
  _patternManager->blockSignals(true);
  _patternManager->setCurrentIndex(gs.arpPatternIndex);
  _patternManager->blockSignals(false);
  
  _timePatternManager->blockSignals(true);
  _timePatternManager->setCurrentIndex(gs.arpTimePatternIndex);
  _timePatternManager->blockSignals(false);
  
}
void MidiHarmony::toggleMode() {
  if (_mode == MODE_CHORD) {
    _mode=MODE_ARPEGGIATOR;
    _arpeggiatorGB->setEnabled(true);
    _modeBut->setText("Mode Chord");
    arpIntervalsChanged(_arpIntervals->currentIndex());
  }
  else {
    _mode=MODE_CHORD;
    _arpeggiatorGB->setEnabled(false);
    _modeBut->setText("Mode Arpeggiator");

    int sel=_harmonyList->currentIndex();
    if (_currentHarmony)
      _currentHarmony->setIntervals(_harmonyList->itemData(sel).value< QList<int> >());
  }
  if (_currentHarmony)
    _currentHarmony->setMode(_mode);
}
void MidiHarmony::selectHarmony(int sel) {
  if (sel <= _harmonyList->count()) {
    _harmonyList->setCurrentIndex(sel);
    if (_currentHarmony)
      _currentHarmony->setHarmonyIndex(sel);
  }
}
void MidiHarmony::harmonyChanged(int sel) {
  if ((_mode == MODE_ARPEGGIATOR) &&
      (_arpIntervals->itemData(_arpIntervals->currentIndex()).toInt() != ARPEG_INTERVALS_HARMONY))
    return;
  QList<int> intervals=_harmonyList->itemData(sel).value< QList<int> >();
  _arpHarmonyGB->setTitle(QString("Harmony - %1 notes long").arg(intervals.size()));
  if (_currentHarmony) {
    _currentHarmony->setIntervals(intervals);
    _currentHarmony->setHarmonyIndex(sel);
  }
  qDebug()<<"harmony int changed";
}
void MidiHarmony::arpTriggerChanged(int sel) {
  if (_currentHarmony)
    _currentHarmony->setArpTrigger(_arpTrigger->itemData(sel).toInt());
}
void MidiHarmony::arpModeChanged(int sel) {
  if (_currentHarmony)
    _currentHarmony->setArpMode(_arpMode->itemData(sel).toInt());
}
void MidiHarmony::arpBPMChanged(int bpm) {
  if (_currentHarmony)
    _currentHarmony->setArpBPM(bpm);
}
void MidiHarmony::arpNoteSpeedChanged(int sel) {
  if (_currentHarmony)
    _currentHarmony->setArpSpeed(_arpNoteSpeed->itemData(sel).toInt());
}
void MidiHarmony::arpConstantSpeedChanged(int sel) {
  if (_currentHarmony)
    _currentHarmony->setConstantSpeed(sel == 0 ? true : false);
  if (sel) {
    _arpCustomTimes->show();
    _timePatternManager->show();
  }
  else {
    _arpCustomTimes->hide();
    _timePatternManager->hide();
  }
}

void MidiHarmony::arpIntervalsChanged(int sel) {
  QList<int> intervals;
  int i;
  switch (_arpIntervals->itemData(sel).toInt()) {
  case ARPEG_INTERVALS_HARMONY:
    _arpSlideRange->hide();
    _patternManager->hide();
    _arpCustomIntervals->hide();
    _learnIntervals->hide();
    intervals=_harmonyList->itemData(_harmonyList->currentIndex()).value< QList<int> >();
    break;
  case ARPEG_INTERVALS_SLIDEUP:
    _arpCustomIntervals->hide();
    _learnIntervals->hide();
    _patternManager->hide();
    _arpSlideRange->show();
    for (i=0; i < _arpSlideRange->value(); i++)
      intervals << i;
    break;
  case ARPEG_INTERVALS_SLIDEDOWN:
    _arpCustomIntervals->hide();
    _learnIntervals->hide();
    _patternManager->hide();
    _arpSlideRange->show();
    for (i=0; i < _arpSlideRange->value(); i++)
      intervals << -i;
    break;
  case ARPEG_INTERVALS_CUSTOM: {
    _arpCustomIntervals->show();
    _learnIntervals->show();
    _patternManager->show();
    _arpSlideRange->hide();
    intervals=Harmony::intervalsFromString(_arpCustomIntervals->text());
    break;
  }
  }
  _arpHarmonyGB->setTitle(QString("Harmony - %1 notes long").arg(intervals.size()));
  if (_currentHarmony) {
    _currentHarmony->setIntervals(intervals);
    _currentHarmony->setArpIntervalsIndex(sel);
  }
  qDebug()<<"arp int changed ";
}

void MidiHarmony::arpCustomIntervalsChanged(QString ints) {
  if (!_arpCustomIntervals->hasAcceptableInput())
    return;
  
  QList<int> intervals=Harmony::intervalsFromString(ints);
  _arpHarmonyGB->setTitle(QString("Harmony - %1 notes long").arg(intervals.size()));
  if (_currentHarmony) {
    _currentHarmony->setIntervals(intervals);
    _currentHarmony->setArpCustomIntervals(ints);
  }
  qDebug()<<"cust intervals changed "<<ints;
}
void MidiHarmony::arpCustomTimesChanged(QString ints) {
  if (!_arpCustomTimes->hasAcceptableInput())
    return;
  
  QList<int> times=Harmony::intervalsFromString(ints);
  if (_currentHarmony) {
    _currentHarmony->setArpCustomTimes(times);
    _currentHarmony->setArpCustomTimes(ints);
  }
  qDebug()<<"cust times changed "<<ints;
}

void MidiHarmony::arpSlideRangeChanged(int val) {
  arpIntervalsChanged(_arpIntervals->currentIndex());
  if (_currentHarmony)
    _currentHarmony->setArpSlideRange(val);
}
void MidiHarmony::savePatternRequested(int index, QString name)
{
  QDomDocument conf;
  QDomElement pr=conf.createElement("preset");
  pr.setAttribute("pattern", _arpCustomIntervals->text());
  _patternManager->setPreset(index, name, pr);
}
void MidiHarmony::curPatternChanged(QDomElement pr)
{
  _arpCustomIntervals->setText(pr.attribute("pattern"));
  if (_currentHarmony)
    _currentHarmony->setArpPatternIndex(_patternManager->currentIndex());
  //updatePattern();
}
void MidiHarmony::saveTimePatternRequested(int index, QString name)
{
  QDomDocument conf;
  QDomElement pr=conf.createElement("preset");
  pr.setAttribute("time_pattern", _arpCustomTimes->text());
  _timePatternManager->setPreset(index, name, pr);
}
void MidiHarmony::curTimePatternChanged(QDomElement pr)
{
  _arpCustomTimes->setText(pr.attribute("time_pattern"));
  if (_currentHarmony)
    _currentHarmony->setArpTimePatternIndex(_timePatternManager->currentIndex());
  //updatePattern();
}

bool MidiHarmony::loadConf()
{
  QDomDocument conf;
  QFile f(_confFile);
  if (!f.open(QIODevice::ReadOnly))
    return false;
  QString errorStr;
  int errorLine, errorColumn;
  if (!conf.setContent(&f, true, &errorStr, &errorLine, &errorColumn)) {
    qDebug()<<"Parse error at line "<<errorLine<< ", column "<<errorColumn<<":"<<errorStr;
    f.close();
    return false;
  }
  f.close();
  QDomElement root=conf.documentElement();
  if (root.tagName() != "MidiHarmony") {
    qDebug()<<"The file is not a conf file for this app.";
    return false;
  }
  _jack.connectionsFromXML(root);

  QDomNodeList param=conf.elementsByTagName("currentFile");
  QDomElement elt;

  if (param.length()) {
    elt=param.item(0).toElement();
    _currentFile=elt.attribute("path");
  }

  /*
  QDomNodeList input=conf.elementsByTagName("input_device");
  QDomElement elt;

  if (input.length()) {
    elt=input.item(0).toElement();
    QString device=elt.attribute("path");
    bool ok=false;
    int n=0;
    while(n < _inputList.count() || !ok) {
      if (_inputList.itemText(n) == device) {
	_inputList.setCurrentIndex(n);
	inputCtrlClicked();
	ok=true;
      }
      n++;
    }
  }
  QDomNodeList output=conf.elementsByTagName("midi_output");
  if (output.length()) {
    elt=output.item(0).toElement();
    _jackClientName.setText(elt.attribute("client_name"));
  }

  output=conf.elementsByTagName("global_channel");
  if (output.length()) {
    elt=output.item(0).toElement();
    _channel.setValue(elt.attribute("value").toInt());
    _globalChannel.setCheckState((Qt::CheckState)elt.attribute("state").toInt());
    globalChannelClicked();
  }

  QDomNodeList cmds=conf.elementsByTagName("midi_command");
  for (uint i=0; i < cmds.length(); i++) {
    elt=cmds.item(i).toElement();
    addMidiCommand(elt.attribute("type").toInt(), elt.attribute("channel").toInt(),
		   elt.attribute("input1").toInt(), elt.attribute("input2").toInt(),
		   elt.attribute("param1").toInt(), elt.attribute("param2").toInt());
  }
  */
  return true;
}
bool MidiHarmony::saveConf()
{
  QDomDocument conf;
  QDomProcessingInstruction instr;
  instr=conf.createProcessingInstruction("xml", "version=\"1.0\"  encoding=\"UTF-8\"");
  conf.appendChild(instr);
  QDomElement root, params;//midi, params;
  root=conf.createElement("MidiHarmony");
  root.setAttribute("xmlns", "http://staxaudio.free.fr/mime-info");
  conf.appendChild(root);

  root.appendChild(_jack.connectionsToXML());

  params=conf.createElement("currentFile");
  params.setAttribute("path", _currentFile);
  root.appendChild(params);

  /*
  QString device=_inputList.itemText(_inputList.currentIndex());
  params=conf.createElement("input_device");
  params.setAttribute("path", device);
  root.appendChild(params);

  params=conf.createElement("midi_output");
  params.setAttribute("client_name", _jackClientName.text());
  root.appendChild(params);

  params=conf.createElement("global_channel");
  params.setAttribute("value", _channel.value());
  params.setAttribute("state", _globalChannel.checkState());
  root.appendChild(params);
 
  for (int i=0; i < _cmdTable.rowCount(); i++) {
    MidiComponent *w=(MidiComponent*)_cmdTable.cellWidget(i, 0);
    midi=conf.createElement("midi_command");
    midi.setAttribute("type", w->itemData(w->currentIndex()).toInt());
    midi.setAttribute("channel", w->_channel.value());
    midi.setAttribute("input1", w->_input1.getValue());
    midi.setAttribute("input2", w->_input2.getValue());
    midi.setAttribute("param1", w->_param1.value());
    midi.setAttribute("param2", w->_param2.value());
    root.appendChild(midi);
  }
  */
  QFile f(_confFile);
  if (!f.open(QIODevice::WriteOnly))
    return false;
  QTextStream ts(&f);
  ts << conf.toString();
  f.close();

  return true;
}
