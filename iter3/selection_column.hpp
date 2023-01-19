#ifndef SELECTION_COLUMN_HPP
#define SELECTION_COLUMN_HPP

#include "column.hpp"
#include "types.hpp"
#include <QString>

class SelectionColumn : public Column {
private:
    QString stylesheet_template = "QLineEdit { color: %1; border-color: %2; background-color: %3; border-style: solid; border-width: %4; border-bottom: none; border-top: none; border-left: none; font-family: %5; padding-left: 6px; }";

    QString text_color;
    inline static QString text_color_default_odd = "#222222";
    inline static QString text_color_default_even = "#222222";

    QString background_color;
    bool odd_;
    inline static QString background_color_default_odd = "#FFFFFF";
    inline static QString background_color_default_even = "#FCFCFC";
    inline static QString background_color_selected = "#555555";
    inline static QString text_color_selected = "#FFFFFF";

    inline static QString border_width = "15px";
    inline static QString font_family = "DejaVu Sans Mono";

    QString border_color;

    inline static std::unordered_map<std::string, QString> extension_to_border_color;
public:
    void setTextAndLeftAlign(const std::string& text) {
	setText(QString::fromStdString(text));
	home(false);
    }

    SelectionColumn(const int& column_height, const std::string& text, bool odd, ProcessedString action)
	: odd_(odd)
    {
        setFixedHeight(column_height);
        setReadOnly(true);
	SetColor(action.Colour());
        setAlignment(Qt::AlignLeft);
	setTextAndLeftAlign(text);
    }

    void SetColor(const QString &color) override
    {
	border_color = color;
	setStyleSheet(stylesheet_template.arg(text_color, border_color, background_color, border_width, font_family));
    }

    virtual void SetSelected(bool selected)
    {
	if (selected) {
	    text_color = text_color_selected;
	    background_color = background_color_selected;
	} else {
	    background_color = odd_ ? background_color_default_even: background_color_default_odd;
	    text_color = odd_ ? text_color_default_even: text_color_default_odd;
	}

	setStyleSheet(stylesheet_template.arg(text_color, border_color, background_color, border_width, font_family));
    }
};

#endif
