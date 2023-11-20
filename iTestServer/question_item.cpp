/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2016 Michal Tomlein

 iTest is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public Licence
 as published by the Free Software Foundation; either version 2
 of the Licence, or (at your option) any later version.

 iTest is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public Licence for more details.

 You should have received a copy of the GNU General Public Licence
 along with iTest; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************/

#include "question_item.h"
#include "svg_item.h"

QuestionItem::QuestionItem(const QString &name,
                           int category,
                           const QString &group,
                           int difficulty,
                           const QString &text,
                           const QStringList &answers,
                           Question::Answers correctanswers,
                           Question::SelectionType selectiontype,
                           const QString &explanation,
                           unsigned int inccount,
                           unsigned int ccount,
                           bool hidden,
                           QList<QListWidgetItem *> images,
                           bool copysvgs)
{
    q_name = name;
    q_category = category;
    q_group = group;
    q_difficulty = difficulty;
    q_explanation = explanation;
    q_selectiontype = selectiontype;
    setText(text);
    q_answers = answers;
    q_correctanswers = correctanswers;
    q_incorrectanscount = inccount;
    q_correctanscount = ccount;
    q_hidden = hidden;
    if (copysvgs) {
        for (int i = 0; i < images.count(); ++i) {
            SvgItem* svg = dynamic_cast<SvgItem*>(images[i]);
            if (svg)
                q_imagesitems << new SvgItem(svg->text(), svg->svg());
            ImageItem* image = dynamic_cast<ImageItem*>(images[i]);
            if (image)
                q_imagesitems << new ImageItem(image->text(), image->image());
        }
    } else {
        q_imagesitems = images;
    }
}

QuestionItem::~QuestionItem()
{
    for (int i = 0; i < q_imagesitems.count(); ++i) {
        if (q_imagesitems.at(i)) delete q_imagesitems.at(i);
    }
}

bool QuestionItem::isHidden() { return q_hidden; }

void QuestionItem::setHidden(bool hidden) { q_hidden = hidden; }

unsigned int QuestionItem::incorrectAnsCount() { return q_incorrectanscount; }

void QuestionItem::setIncorrectAnsCount(unsigned int count) { q_incorrectanscount = count; }

unsigned int QuestionItem::correctAnsCount() { return q_correctanscount; }

void QuestionItem::setCorrectAnsCount(unsigned int count) { q_correctanscount = count; }

void QuestionItem::addIncorrectAns() { q_incorrectanscount++; }

void QuestionItem::addCorrectAns() { q_correctanscount++; }

void QuestionItem::addImageItem(QListWidgetItem *image) { q_imagesitems << image; }

bool QuestionItem::removeImageItem(QListWidgetItem *image) { return q_imagesitems.removeAll(image) > 0; }

QListWidgetItem *QuestionItem::removeImageItem(int i) { return q_imagesitems.takeAt(i); }

int QuestionItem::numImageItems() { return q_imagesitems.count(); }

QListWidgetItem *QuestionItem::imageItem(int i) { return q_imagesitems.at(i); }

QList<QListWidgetItem *> QuestionItem::imagesItems() { return q_imagesitems; }

int QuestionItem::recommendedDifficulty()
{
    if (q_correctanscount == 0 && q_incorrectanscount == 0) {
        return -1;
    } else if (q_correctanscount == 0 && q_incorrectanscount > 0) {
        return 2;
    }
    long double ansratio = (long double)q_incorrectanscount / (long double)q_correctanscount;
    if (ansratio <= 0.5) {
        return 0;
    } else if (ansratio > 0.5 && ansratio < 2.0) {
        return 1;
    } else if (ansratio >= 2.0) {
        return 2;
    }
    return -1;
}

QString QuestionItem::allProperties(bool itdb1_3)
{
    QString out;
    // Q_NAME
    out.append("[Q_NAME]\n");
    out.append(q_name);
    // Q_FLAG
    out.append("\n[Q_FLAG]\n");
    out.append(QString("%1").arg(q_category));
    // Q_GROUP
    out.append("\n[Q_GRP]\n");
    out.append(q_group);
    // Q_DIFFICULTY
    out.append("\n[Q_DIF]\n");
    out.append(QString("%1").arg(q_difficulty));
    // Q_TEXT
    out.append("\n[Q_TEXT]\n");
    out.append(q_text);
    // Q_ANSWERS
    if (itdb1_3) {
        for (int i = 0; i < 4; ++i) {
            out.append(QString("\n[Q_ANS%1]\n").arg((char)('A' + i)));
            if (i < q_answers.count()) {
                out.append(q_answers.at(i));
                out.append(isAnswerAtIndexCorrect(i + 1) ? "\ntrue" : "\nfalse");
            } else {
                out.append("\nfalse");
            }
        }
    } else {
        out.append("\n[Q_ANS]\n");
        out.append(QString("%1\n").arg(q_selectiontype));
        out.append(QString("%1\n").arg(q_correctanswers));
        out.append(QString("%1\n").arg(q_answers.count()));
        for (int i = 0; i < q_answers.count(); ++i) {
            out.append(q_answers.at(i));
            out.append("\n");
        }
        // Q_COMP_ANS
        out.append("[Q_COMP_ANS]\n");
        out.append(QString("%1\n").arg(q_compareAnswers.count()));
        for (int i = 0; i < q_compareAnswers.count(); ++i) {
            out.append(q_compareAnswers.at(i));
            out.append("\n");
        }
        // Q_EXPLANATION
        out.append("[Q_EXPL]\n");
        out.append(q_explanation);
    }
    // STATISTICS
    out.append("\n[Q_ICCNT]\n");
    out.append(QString("%1\n%2").arg(q_incorrectanscount).arg(q_correctanscount));
    // Q_HIDDEN
    out.append("\n[Q_HID]\n");
    out.append(q_hidden ? "true" : "false");
    // Q_SVG
    out.append("\n[Q_SVG]\n");
    out.append(QString("%1").arg(q_imagesitems.count()));
    for (int i = 0; i < q_imagesitems.count(); ++i) {
        out.append("\n");
        out.append(q_imagesitems.at(i)->text());
        SvgItem* svg = dynamic_cast<SvgItem*>(q_imagesitems.at(i));
        if (svg)
        {
            out.append("\n");
            out.append("svg");
            out.append("\n");
            out.append(svg->svg());
        }
        ImageItem* image = dynamic_cast<ImageItem*>(q_imagesitems.at(i));
        if (image)
        {
            out.append("\n");
            out.append("image");
            out.append("\n");
            out.append(image->image());
        }

    }
    return out;
}

QString QuestionItem::allPublicProperties()
{
    QString out;
    // Q_NAME
    out.append("[Q_NAME]\n");
    out.append(q_name);
    // Q_FLAG
    out.append("\n[Q_FLAG]\n");
    out.append(QString("%1").arg(q_category));
    // Q_GROUP
    out.append("\n[Q_GRP]\n");
    out.append(q_group);
    // Q_DIFFICULTY
    out.append("\n[Q_DIF]\n");
    out.append(QString("%1").arg(q_difficulty));
    // Q_TEXT
    out.append("\n[Q_TEXT]\n");
    out.append(q_text);
    // Q_ANSWERS
    out.append("\n[Q_ANS]\n");
    out.append(QString("%1\n").arg(q_selectiontype));
    out.append(QString("%1\n").arg(q_answers.count()));
    for (int i = 0; i < q_answers.count(); ++i) {
        out.append(q_answers.at(i));
        out.append("\n");
    }
    // Q_COMP_ANSWERS
    out.append("[Q_COMP_ANS]\n");
    out.append(QString("%1\n").arg(q_compareAnswers.count()));
    for (int i = 0; i < q_compareAnswers.count(); ++i) {
        out.append(q_compareAnswers.at(i));
        out.append("\n");
    }
    // Q_SVG
    out.append("[Q_SVG]\n");
    out.append(QString("%1").arg(q_imagesitems.count()));
    return out;
}

QuestionAnswer::QuestionAnswer(Question::Answer correct, Question::Answer ans, QString n_str_answered, int num_answers, QList<int> n_compAnswered, int category, int difficulty, Question::SelectionType type, const QString &explanation)
{
    qa_answered = ans;
    qa_correct_answer = correct;
    qa_str_answered = n_str_answered;
    qa_num_answers = num_answers;
    qa_compAnswered = n_compAnswered;
    qa_category = category;
    q_difficulty = difficulty;
    q_selectiontype = type;
    qa_explanation = explanation;
}

void QuestionAnswer::setAnswered(Question::Answer ans) { qa_answered = ans; }

Question::Answer QuestionAnswer::answered() { return qa_answered; }

void QuestionAnswer::setCorrectAnswer(Question::Answer ans) { qa_correct_answer = ans; }

Question::Answer QuestionAnswer::correctAnswer() { return qa_correct_answer; }

void QuestionAnswer::setNumAnswers(int num_answers) { qa_num_answers = num_answers; }

int QuestionAnswer::numAnswers() { return qa_num_answers; }

void QuestionAnswer::setCompAnswered(QList<int> ans) { qa_compAnswered = ans; }

QList<int> QuestionAnswer::compAnswered() { return qa_compAnswered; }

/*bool QuestionAnswer::isAnsweredCorrectly()
{
    if (qa_answered == Question::None && qa_correct_answer != Question::None) { return false; }
    if ((qa_answered & qa_correct_answer) == qa_answered) { return true; }
    return false;
}*/

void QuestionAnswer::setCategory(int category) { qa_category = category; }

int QuestionAnswer::category() { return qa_category; }

void QuestionAnswer::setDifficulty(int difficulty) { q_difficulty = difficulty; }

int QuestionAnswer::difficulty() { return q_difficulty; }

void QuestionAnswer::setSelectionType(Question::SelectionType type) { q_selectiontype = type; }

Question::SelectionType QuestionAnswer::selectionType() { return q_selectiontype; }

void QuestionAnswer::setExplanation(const QString &explanation) { qa_explanation = explanation; }

QString QuestionAnswer::explanation() { return qa_explanation; }

void QuestionAnswer::setStrAnswered(QString n_str_answered)
{
    qa_str_answered = n_str_answered;
}

QString QuestionAnswer::str_answered() {return qa_str_answered;}
