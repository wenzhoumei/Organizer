#include <QWidget>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QList>
#include <QApplication>

#include <array>
#include "title_column.hpp"
#include "search_column.hpp"
#include "selection_column.hpp"


class MenuWindow : public QWidget
{
private:
    int num_columns;
    int num_selection_columns;

    QVBoxLayout *layout;

    TitleColumn* title_column;
    SearchColumn* search_column;
    QList<SelectionColumn*> selection_columns;

    int selected_column_index = 0;

    std::vector<std::string> current_space_ = { "google.link", "hello.world", "music.dir", "cat.term", "state.li", "program.run", "last.last" };

public:
    MenuWindow(int num_columns, int column_height, int column_width)
	:num_columns(num_columns), num_selection_columns(num_columns - 2)
    {
	setWindowFlag(Qt::FramelessWindowHint);

	// Centre on screen
        QScreen *screen = QGuiApplication::primaryScreen();
        QSize screen_size = screen->size();
        int screen_width = screen_size.width();
        int screen_height = screen_size.height();

	move((screen_width - column_width) / 2, (screen_height - (column_height * num_columns)) / 2);

	setFixedHeight(column_height * num_columns);
	setFixedWidth(column_width);

	layout = new QVBoxLayout(this);
        layout->setMargin(0);
        layout->setSpacing(0);

	title_column = new TitleColumn(column_height, "Hello");
	search_column = new SearchColumn(column_height);

	/*
	// Bind functions to events
	QObject::connect(search_column, &QLineEdit::textEdited, this, &MenuWindow::OnEdit);
	QObject::connect(search_column, &QLineEdit::returnPressed, this, &MenuWindow::ReturnPressed);
	QObject::connect(search_column, &SearchColumn::upArrowPressed, this, &MenuWindow::UpArrowPressed);
	QObject::connect(search_column, &SearchColumn::downArrowPressed, this, &MenuWindow::DownArrowPressed);
	*/

	// Add search and title column
	layout->addWidget(title_column);
	layout->addWidget(search_column);

	PadSearchedSpace();

	// Add selection columns
	for (int i = 0; i < num_selection_columns; ++i) {
	    SelectionColumn* column = new SelectionColumn(column_height, current_space_[i], i % 2 == 1, );
	    selection_columns.append(column);

	    layout->addWidget(column);
	}

	/*
	selection_columns[0]->SetSelected(true);

	search_column->setFocus();
	*/
	this->show();
    }


    void PadSearchedSpace() {
	int difference = num_selection_columns - searched_space.Size();
	if (difference > 0) {
	    while (difference--) {
		searched_space.Append("");
	    }
	}
    }

    /*
    void UpArrowPressed() {
	if (searched_space.Size() == 0) return;

	if (selection_column_index == 0) {
	    return;
	} else {
	    selection_columns[selection_column_index]->SetSelected(false);
	    selection_column_index -= 1;
	    selection_columns[selection_column_index]->SetSelected(true);
	}
    }

    void DownArrowPressed() {
	if (searched_space.Size() == 0) return;

	if (selection_column_index == num_selection_columns - 1) {
	    return;
	} else {
	    selection_columns[selection_column_index]->SetSelected(false);
	    selection_column_index += 1;
	    selection_columns[selection_column_index]->SetSelected(true);
	}
    }

    void ReturnPressed() {
	this->hide();
	if (no_results) { ControlReturnPressed(); return; }
	
	std::string input = searched_space[selection_column_index];
	action_string_processor.GetScript(input)(input);
    }

    void ControlReturnPressed() {
	this->hide();
	std::string input = search_column->text().toStdString();

	action_string_processor.GetScript(input)(input);
    }

    void PadSearchedSpace() {
	int difference = num_selection_columns - searched_space.Size();
	if (difference > 0) {
	    while (difference--) {
		searched_space.Append("");
	    }
	}
    }

    void OnEdit(const QString &text) {
	std::string input = text.toStdString();
	searched_space.Clear();
	current_space.FindMatches(input, searched_space);

	if (searched_space.Size() == 0) {
	    selection_columns[selection_column_index]->SetSelected(false);
	    no_results = true;
	}
	else {
	    selection_columns[selection_column_index]->SetSelected(true);
	    no_results = false;
	}
	
	PadSearchedSpace();

	for (int i = 0; i < num_columns - 2; i++) {
	    selection_columns[i]->setTextAndLeftAlign(QString::fromStdString(searched_space[i]));
	    selection_columns[i]->SetColor(action_string_processor.GetColour(searched_space[i]));
	}

	search_column->SetColor(action_string_processor.GetColour(input));
    }
    */
};
