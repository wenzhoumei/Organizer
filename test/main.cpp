#include <QScreen>
#include <QApplication>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QTextOption>
#include <QPalette>

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <iomanip>

QString generateRandomColor() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 255);

  std::stringstream ss;
  ss << "#";
  for (int i = 0; i < 3; i++) {
    int num = dis(gen);
    ss << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << num;
  }

  return QString::fromStdString(ss.str());
}

void setLineEditStyle(QLineEdit *lineEdit, const QString &backgroundColor, const QString &textColor, const QString &borderColor, const QString &borderStyle, const QString &borderWidth)
{
    lineEdit->setStyleSheet(QString("QLineEdit { background-color: %1; color: %2; border-color: %3; border-style: %4; border-width: %5; border-bottom: none; border-top: none; border-left: none }").arg(backgroundColor, textColor, borderColor, borderStyle, borderWidth));
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Create a QWidget as the container for the layout
    QWidget *window = new QWidget;

    window->setWindowFlag(Qt::FramelessWindowHint);

    // Get size of screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screen_size = screen->size();
    int screen_width = screen_size.width();
    int screen_height = screen_size.height();

    int num_columns = 10;
    int column_height = 33;

    int window_width = 300;
    int window_height = column_height * num_columns;

    window->setFixedHeight(window_height);
    window->setFixedWidth(window_width);

    window->move((screen_width - window_width) / 2, (screen_height - window_height) / 2);

    // Create a QVBoxLayout and set it as the layout for the container widget
    QVBoxLayout *layout = new QVBoxLayout(window);

    layout->setMargin(0);

    // Set the layout's spacing to 0 pixels
    layout->setSpacing(0);

    // Create a list of QLineEdit widgets
    QList<QLineEdit*> lineEdits;
    for (int i = 0; i < 10; ++i) {
	QLineEdit *lineEdit = new QLineEdit;
	lineEdit->setFixedHeight(column_height);
	lineEdit->setAlignment(Qt::AlignCenter);
	lineEdit->setReadOnly(true);

	QString color_foreground = ((i % 2) == 0) ? "#FFFFFF": "#FCFCFC";
	setLineEditStyle(lineEdit, color_foreground, "#222222", generateRandomColor(), "solid", "7px");

	lineEdits.append(lineEdit);
	layout->addWidget(lineEdit);
    }

    QLineEdit*& selected = lineEdits[2];
    setLineEditStyle(selected, "#777777", "#222222", "#FFFFFF", "solid", "7px");

    QLineEdit*& search = lineEdits[1];
    search->setReadOnly(false);
    search->setFocus();
    setLineEditStyle(search, "#EEEEEE", "#222222", "#000000", "solid", "7px");

    QLineEdit*& title_bar = lineEdits[0];
    title_bar->setText("program.program");
    setLineEditStyle(title_bar, "#222222", "#FFFFFF", "#FF0000", "solid", "7px");

    window->show();

    return app.exec();
}

