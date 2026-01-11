#include "SubtitleLabel.h"

SubtitleLabel::SubtitleLabel(const QString& text, bool clickable, QWidget* parent) : TitleLabel(text, clickable, parent) {
    setObjectName("SubtitleLabel");
}

SubtitleLabel::SubtitleLabel(const QString& text, QWidget* parent) : SubtitleLabel(text, false, parent) {}
