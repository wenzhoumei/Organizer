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

#include "action_string_processor.hpp"

void FindMatches(const std::string& input_text, const std::vector<std::string>& input_array, std::vector<std::string>& output_array)
{
    if (input_text == "") {
	output_array = input_array;
	return;
    }
    // Split the input text into tokens
    std::istringstream iss(input_text);
    std::vector<std::string> inputTokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    // Create vectors to store the exact, prefix, and substring matches
    std::vector<std::string> exactMatches;
    std::vector<std::string> prefixMatches;
    std::vector<std::string> substringMatches;

    // Iterate through each item in the search array
    for (const std::string& item : input_array)
    {
	// Check if all of the input tokens can be found in the item's text
	bool allTokensFound = true;
	for (const std::string& token : inputTokens)
	{
	    if (item.find(token) == std::string::npos)
	    {
		allTokensFound = false;
		break;
	    }
	}

	// If all of the input tokens were found, determine if the item's text is an exact match,
	// a prefix match, or a substring match, and add the item to the appropriate vector
	if (allTokensFound)
	{
	    if (input_text == item)
	    {
		exactMatches.push_back(item);
	    }
	    else if (item.find(inputTokens[0]) == 0)
	    {
		prefixMatches.push_back(item);
	    }
	    else
	    {
		substringMatches.push_back(item);
	    }
	}
    }

    // Concatenate the exact, prefix, and substring matches vectors and add them to the output array
    output_array.insert(output_array.end(), exactMatches.begin(), exactMatches.end());
    output_array.insert(output_array.end(), prefixMatches.begin(), prefixMatches.end());
    output_array.insert(output_array.end(), substringMatches.begin(), substringMatches.end());
}

void LoadLines(const std::string& filename, std::vector<std::string>& lines) {
    std::ifstream file(filename);
    if (!file.good()) {
	std::cerr << "File not found" << std::endl;
    }

    std::copy(std::istream_iterator<std::string>(file),
	      std::istream_iterator<std::string>(),
	      std::back_inserter(lines));
    file.close();
}

class Column : public QLineEdit
{
protected:
    virtual void SetColor(const QString &color) = 0;
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
    ActionStringProcessor action_string_processor;

public:
    SelectionColumn(const int& column_height, const bool& odd, const QString& text, const ActionStringProcessor& action_string_processor)
	:action_string_processor(action_string_processor)
    {
	background_color = odd ? background_color_even: background_color_odd;
        setFixedHeight(column_height);
        setReadOnly(true);
	SetColor(action_string_processor.GetColour(text.toStdString()));
        setAlignment(Qt::AlignCenter);
	setText(text);
    }

    void SetColor(const QString &color) override
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
    ActionStringProcessor action_string_processor;

public:
    SearchColumn(const int& column_height, ActionStringProcessor action_string_processor)
	: action_string_processor(action_string_processor)
    {
        setAlignment(Qt::AlignCenter);
        setFixedHeight(column_height);
	setStyleSheet(stylesheet_template.arg(text_color, border_color, background_color, border_width, font_family));
    }

    void SetColor(const QString &color) override
    {
	border_color = color;
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
    ActionStringProcessor action_string_processor;

public:
    TitleColumn(const int& column_height, const QString& text, ActionStringProcessor action_string_processor)
	: action_string_processor(action_string_processor)
    {
        setFixedHeight(column_height);
        setAlignment(Qt::AlignCenter);
	setText(text);
        setReadOnly(true);
	setStyleSheet(stylesheet_template.arg(text_color, border_color, background_color, border_width, font_family));
    }

    void SetColor(const QString &color) override
    {
	border_color = color;
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

    TitleColumn* title_column;
    SearchColumn* search_column;
    QList<SelectionColumn*> selection_columns;
    std::string directory_path = "/home/wenzhou/Organizer/files";
    ActionStringProcessor action_string_processor = ActionStringProcessor(directory_path);
    std::vector<std::string> current_space;

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

	std::filesystem::current_path(directory_path);
	action_string_processor.PreloadExtensions();

	LoadLines("root.space", current_space);

	title_column = new TitleColumn(column_height, "Hello", action_string_processor);
	search_column = new SearchColumn(column_height, action_string_processor);
	QObject::connect(search_column, &QLineEdit::textEdited, this, &MenuWindow::OnEdit);

	// Add search and title column
	layout->addWidget(title_column);
	layout->addWidget(search_column);

	int difference = current_space.size() - (num_columns - 2);
	if (difference < 0) {
	    for (int i = 0; i < -difference; i++) {
		current_space.push_back("");
	    }
	}

	// Add selection columns
	for (int i = 0; i < num_columns - 2; ++i) {
	    QString color_foreground = ((i % 2) == 0) ? "#FFFFFF": "#FCFCFC";
	    SelectionColumn* column = new SelectionColumn(column_height, i % 2 == 1, QString::fromStdString(current_space[i]), action_string_processor);
	    selection_columns.append(column);

	    layout->addWidget(column);
	}

	search_column->setFocus();
	this->show();
    }

    void OnEdit(const QString &text) {
	std::vector<std::string> output_array;
	std::string input = text.toStdString();
	FindMatches(input, current_space, output_array);
	
	int difference = output_array.size() - (num_columns - 2);
	if (difference < 0) {
	    for (int i = 0; i < -difference; i++) {
		output_array.push_back("");
	    }
	}

	for (int i = 0; i < num_columns - 2; i++) {
	    selection_columns[i]->setText(QString::fromStdString(output_array[i]));
	    selection_columns[i]->SetColor(action_string_processor.GetColour(output_array[i]));
	}
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
