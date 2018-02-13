#include <QtCore>
#include "midiharmony.h"

void usage(QString appname, QString error="") {
  QTextStream out(stdout);
  
  if (!error.isEmpty())
    out << "error: " << error << "\n";
  out << "Usage: " << appname << " [options]\n";
  out << "\t options:\n";
  out << "\t\t-j name (set the jack client name to name)\n";
  out << "\t\t-d harmony_db_path (set an alternative harmony database path)\n";
  out << "\t\t-h (show this help!)\n";
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Q_INIT_RESOURCE(resources_qjack);
  QTextStream out(stdout); 
  QString name="MidiHarmony";
  QString db=QDir::homePath()+"/harmonydb.xml";//"/usr/share/midiharmony/harmonydb.xml";
  QString arg;
  
  qRegisterMetaType<jack_time_t>("jack_time_t");
 
  for (int i=1; i < argc; i++) {
    arg=argv[i];
    if (arg == "-j") {
      if (i+1 < argc) {
	i++;
	name=argv[i];
      }
      else {
	usage(argv[0], "error: client name missing!");
	exit(1);
      }
    }
    if (arg == "-d") {
      if (i+1 < argc) {
	i++;
	db=argv[i];
      }
      else {
	usage(argv[0], "error: database path missing!");
	exit(1);
      }
    }
    if (arg == "-h") {
      usage(argv[0]);
      exit(0);
    }
  }
  MidiHarmony dlg(db, name);
  QFile qss(":/qss");
  qss.open(QFile::ReadOnly);
  QString st=qss.readAll();
  //qDebug()<<st;
  a.setStyleSheet(st);
  qss.close();

  dlg.show();
  return a.exec();
}
