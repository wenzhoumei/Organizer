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

class Column : public QLineEdit
{
public:
};

class SelectionColumn : public Column {
private:
    QString stylesheet_template = "QLineEdit { color: %1; border-color: %2; background-color: %3; border-style: solid; border-width: %4; border-bottom: none; border-top: none; border-left: none; font-family: %5; }";
    inline static QString text_color = "#222222";
    inline static QString background_color_odd = "#FFFFFF";
    inline static QString background_color_even = "#FCFCFC";
    inline static QString background_color;
    inline static QString border_width = "15px";
    inline static QString font_family = "DejaVu Sans Mono";
    QString border_color;

public:
    SelectionColumn(const int& column_height, const QString& border_color, const bool& odd, const QString& text)
    {
	background_color = odd ? background_color_even: background_color_odd;
        setFixedHeight(column_height);
        setReadOnly(true);
	SetColor(border_color);
        setAlignment(Qt::AlignCenter);
	setText(text);
    }

    void SetColor(const QString &color)
    {
	border_color = color;
	setStyleSheet(stylesheet_template.arg(text_color, border_color, background_color, border_width, font_family));
    }
};

class SearchColumn : public Column
{
    QString stylesheet_template = "QLineEdit { color: %1; border-color: %2; background-color: %3; border-style: solid; border-width: %4; border-bottom: none; border-top: none; border-left: none; font-family: %5; }";
    inline static QString text_color = "#222222";
    inline static QString background_color = "#F5F5F5";
    inline static QString border_width = "15px";
    inline static QString font_family = "DejaVu Sans Mono";
    inline static QString border_color = "#000000";

public:
    SearchColumn(const int& column_height)
    {
        setAlignment(Qt::AlignCenter);
        setFixedHeight(column_height);
	setStyleSheet(stylesheet_template.arg(text_color, border_color, background_color, border_width, font_family));
    }
};

class TitleColumn : public Column
{
    QString stylesheet_template = "QLineEdit { color: %1; border-color: %2; background-color: %3; border-style: solid; border-width: %4; border-bottom: none; border-top: none; border-left: none; font-family: %5; }";
    inline static QString text_color = "#FFFFFF";
    inline static QString background_color = "#222222";
    inline static QString border_width = "15px";
    inline static QString font_family = "DejaVu Sans Mono";
    inline static QString border_color = "#000000";

public:
    TitleColumn(const int& column_height, const QString& text)
    {
        setFixedHeight(column_height);
        setAlignment(Qt::AlignCenter);
	setText(text);
        setReadOnly(true);
	setStyleSheet(stylesheet_template.arg(text_color, border_color, background_color, border_width, font_family));
    }
};


class MenuWindow : public QWidget
{
private:
    int num_columns;
    int column_height;
    int column_width;

    QVBoxLayout *layout;

    QList<Column*> columns;

public:
    MenuWindow(int num_columns, int column_height, int column_width)
	:num_columns(num_columns), column_height(column_height), column_width(column_width)
    {
	setWindowFlag(Qt::FramelessWindowHint);

	Centre();

	setFixedHeight(column_height * num_columns);
	setFixedWidth(column_width);

	layout = new QVBoxLayout(this);
        layout->setMargin(0);
        layout->setSpacing(0);

	// Add search and title column
	layout->addWidget(new TitleColumn(column_height, "Hello"));
	layout->addWidget(new SearchColumn(column_height));

	std::vector<QString> actions = { "google.link", "chrome.app", "youtube.link", ".ext", "physics-topics.dir", "in-the-end.playlist", "final.music" };
	// Add selection columns
	for (int i = 0; i < num_columns - 2; ++i) {
	    QString color_foreground = ((i % 2) == 0) ? "#FFFFFF": "#FCFCFC";
	    Column* column = new SelectionColumn(column_height, generateRandomColor(), i % 2 == 1, actions[i]);

	    layout->addWidget(column);
	}


	(layout->itemAt(1))->widget()->setFocus();
	this->show();
    }

    void Centre() {
	// Get size of screen
        QScreen *screen = QGuiApplication::primaryScreen();
        QSize screen_size = screen->size();
        int screen_width = screen_size.width();
        int screen_height = screen_size.height();

	move((screen_width - column_width) / 2, (screen_height - (column_height * num_columns)) / 2);
    };
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    MenuWindow window(8, 33, 300);

    /*
    for (int i = 0; i < 7; ++i) {
	window->NewColumn(generateRandomColor());
    }
    */

    return app.exec();
}

/*
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MenuWindow* window = new MenuWindow(7, 50, 300);

    for (int i = 0; i < 7; ++i) {
	window->NewColumn(generateRandomColor());
    }

    window->show();
    return app.exec();
}
*/

/*
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
    lineEdit->setStyleSheet(QString("QLineEdit { background-color: %1; color: %2; border-color: %3; border-style: %4; border-width: %5; border-bottom: none; border-top: none; border-left: none; font-family: DejaVu Sans Mono; }").arg(backgroundColor, textColor, borderColor, borderStyle, borderWidth));
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

    int num_columns = 7;
    int column_height = 35;

    int window_width = 300;
    int window_height = column_height * num_columns;

    window->setFixedHeight(window_height);
    window->setFixedWidth(window_width);

    window->move((screen_width - window_width) / 2, (screen_height - window_height) / 2);

    // Create a QVBoxLayout and set it as the layout for the container widget
    QVBoxLayout *layout = new QVBoxLayout(window);
    layout->setDirection(QBoxLayout::TopToBottom);

    layout->setMargin(0);

    // Set the layout's spacing to 0 pixels
    layout->setSpacing(0);

    // Create a list of QLineEdit widgets
    QList<QLineEdit*> lineEdits;
    for (int i = 0; i < num_columns; ++i) {
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
*/
