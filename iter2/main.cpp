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
#include <unordered_map>
#include <fstream>




struct SelectionColumnPreload {
    inline static std::unordered_map<std::string, std::function<void(std::string)>> extension_functions;

    QString stylesheet;
    ActionString selection_text;

    SelectionColumnPreload(const std::string& text)
	:selection_text(text)
    {
	ActionString action_text(text);
	if (!extension_functions.contains(action_text.Extension())) {
	}

	extension_functions[extension](*this);
	MakeCurrent();
    }

    QString Colour(std::string extension) const {
	if (extension_colors.contains(extension)) {
	    // If extension exists set to corresponding color based on map
	    return extension_colors[extension];
	} else {
	    if (extension_functions.contains(extension)) {
		// If extension exists create new file
	    } else {
		// If extension doesn't exist
	    }
	}
};

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
    inline static std::unordered_map<std::string, QString> extension_to_border_color;
    QString border_color;

public:
    SelectionColumn(const int& column_height, const bool& odd, const QString& text)
    {
	background_color = odd ? background_color_even: background_color_odd;
        setFixedHeight(column_height);
        setReadOnly(true);
	SetColor(SetColorBasedOnExtension(text));
        setAlignment(Qt::AlignCenter);
	setText(text);
    }


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
	    Column* column = new SelectionColumn(column_height, i % 2 == 1, actions[i]);

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
    return app.exec();
}
