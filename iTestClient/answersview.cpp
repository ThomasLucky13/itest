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

#include "answersview.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QRadioButton>
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

AnswerView::AnswerView(int i, AnswersView *parent):
QWidget(parent) {
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(0, 0, 0, 0); hlayout->setSpacing(6);
    ans_radiobutton = new QRadioButton(Question::indexToLabel(i), this);
    ans_checkbox = new QCheckBox(Question::indexToLabel(i), this);
    ans_text = new QTextBrowser(this);
    ans_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ans_text->setFontPointSize(15);
    ans_text->setMaximumSize(16777215, 15 * 5);
    hlayout->addWidget(ans_radiobutton);
    hlayout->addWidget(ans_checkbox);
    hlayout->addWidget(ans_text);
}

AnswersView::AnswersView(QWidget *parent):
QWidget(parent) {
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 0, 0, 0);
#ifndef Q_OS_MAC
    vlayout->setSpacing(6);
#else
    vlayout->setSpacing(0);
#endif
    av_inputanswer_label = new QLabel(tr("Enter your answer:"), this);
    av_input_text = new QTextBrowser(this);
    av_input_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    av_input_text->setFontPointSize(15);
    av_inputanswer_label->setFont(av_input_text->font());
    av_input_text->setMaximumSize(16777215, 15 * 5);
    av_input_text->setReadOnly(false);
    vlayout->addSpacerItem(
                new QSpacerItem(20,40, QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding));
    vlayout->addWidget(av_inputanswer_label);
    vlayout->addWidget(av_input_text);
    av_grp_checkboxes = new QButtonGroup(this);
    av_grp_checkboxes->setExclusive(false);
    av_grp_radiobuttons = new QButtonGroup(this);
    av_grp_ansbuttons1 = new QButtonGroup(this);
    av_grp_ansbuttons2 = new QButtonGroup(this);
    av_ans1_compInd = -1; av_ans2_compInd = -1;
    QVBoxLayout *vAnsLayout1 = new QVBoxLayout(this), *vAnsLayout2 = new QVBoxLayout(this);
    vAnsLayout1->setContentsMargins(0, 0, 0, 0); vAnsLayout2->setContentsMargins(0, 0, 0, 0);
    vAnsLayout1->addSpacerItem(new QSpacerItem(20,40, QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding));
    vAnsLayout2->addSpacerItem(new QSpacerItem(20,40, QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding));
    for (int i = 0; i < 9; ++i) {
        AnswerView *ans = new AnswerView(i + 1, this);
        QPushButton * cans = new QPushButton(this);
        if (i >= 4) {
            ans->setVisible(false);
        }
        av_answers << ans;
        av_grp_checkboxes->addButton(ans->ans_checkbox);
        av_grp_radiobuttons->addButton(ans->ans_radiobutton);
        av_grp_ansbuttons1->addButton(cans, i);
        vAnsLayout1->addWidget(ans);
        vAnsLayout1->addWidget(cans);
    }
    for (int i = 0; i < 9; ++i) {
        QPushButton *ans = new QPushButton( this);
        av_grp_ansbuttons2->addButton(ans, i);
        vAnsLayout2->addWidget(ans);
    }
    QHBoxLayout *hAnsLayout = new QHBoxLayout(this);
    vAnsLayout1->addSpacerItem(new QSpacerItem(20,40, QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding));
    vAnsLayout2->addSpacerItem(new QSpacerItem(20,40, QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding));
    hAnsLayout->addLayout(vAnsLayout1); hAnsLayout->addLayout(vAnsLayout2);
    vlayout->addLayout(hAnsLayout);
    av_reanswer_button = new QPushButton(this);
    av_reanswer_button->setText(tr("Reset answer"));
    av_reanswer_button->setIcon(QIcon(":/images/images/reload.png"));
    av_reanswer_button->setIconSize(QSize(24,24));
    QHBoxLayout *hReAnsLayout = new QHBoxLayout(this);
    hReAnsLayout->addWidget(av_reanswer_button);
    hReAnsLayout->addSpacerItem(new QSpacerItem(20,40,  QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Preferred));
    vlayout->addLayout(hReAnsLayout);
    av_comparePairs_label = new QLabel();
    vlayout->addWidget(av_comparePairs_label);
    QObject::connect(av_grp_checkboxes, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(emitButtonReleased(QAbstractButton *)));
    QObject::connect(av_grp_radiobuttons, SIGNAL(buttonReleased(QAbstractButton *)), this, SLOT(emitButtonReleased(QAbstractButton *)));
    QObject::connect(av_grp_ansbuttons1, SIGNAL(idClicked(int)), this, SLOT(compareAnswer1Choosed(int)));
    QObject::connect(av_grp_ansbuttons2, SIGNAL(idClicked(int)), this, SLOT(compareAnswer2Choosed(int)));
    QObject::connect(av_input_text, SIGNAL(textChanged()), this, SLOT(emitInputReleased()));
    QObject::connect(av_reanswer_button, SIGNAL(clicked()), this, SLOT(resetAnswersClick()));
}

void AnswersView::setAnswers(const QStringList &answers, Question::Answers selected_answers, Question::SelectionType selectiontype, QList<int> ans_order,
                              QString str_answered, const QStringList & compAnswers,const QMap<int,int> & comp_pairs, QList<int> comp_ans_order, QString compareText)
{
    if (selectiontype == Question::OpenQuestion)
    {
        showOpenQuestion(true);
        hideSelectAnswer();
        hideComparison();
        av_input_text->setText(str_answered);
    }
    else if (selectiontype == Question::Comparison)
    {
        av_comparePairs_label->setVisible(true);
        av_comparePairs_label->setText(compareText);
        showOpenQuestion(false);
        hideSelectAnswer();
        av_ans_order = ans_order;
        av_comp_ans_order = comp_ans_order;
        av_reanswer_button->setVisible(true);
        for (int i = 0; i < 9; ++i) {
            QPushButton *ans1 = dynamic_cast<QPushButton*>(av_grp_ansbuttons1->button(i));
            QPushButton *ans2 = dynamic_cast<QPushButton*>(av_grp_ansbuttons2->button(i));
            if (i < answers.count()) {
                ans1->setText(answers.at(av_ans_order.at(i)));
                ans1->setVisible(!comp_pairs.contains(i));
            } else {
                ans1->setText("");
                ans1->setVisible(false);
            }
            if (i < compAnswers.count()) {
                ans2->setText(compAnswers.at(av_comp_ans_order.at(i)));
                int k = comp_pairs.key(i);
                ans2->setVisible(!(comp_pairs.contains(k) && (comp_pairs.value(k)==i)));
            } else {
                ans2->setText("");
                ans2->setVisible(false);
            }
        }
    } else
    {
        showOpenQuestion(false);
        hideComparison();
        av_ans_order = ans_order;
        av_grp_radiobuttons->setExclusive(false);
        for (int i = 0; i < 9; ++i) {
            AnswerView *ans = av_answers.at(i);
            ans->ans_radiobutton->setVisible(selectiontype == Question::SingleSelection);
            ans->ans_checkbox->setVisible(selectiontype == Question::MultiSelection);
            if (i < answers.count()) {
                ans->ans_text->setText(answers.at(av_ans_order.at(i)));
                ans->ans_text->setFixedHeight(ans->ans_text->document()->size().height());
                ans->ans_checkbox->setChecked(selected_answers.testFlag(Question::indexToAnswer(av_ans_order.at(i) + 1)));
                ans->ans_radiobutton->setChecked(selected_answers.testFlag(Question::indexToAnswer(av_ans_order.at(i) + 1)));
                ans->setVisible(true);
            } else {
                ans->ans_text->clear();
                ans->ans_checkbox->setChecked(false);
                ans->ans_radiobutton->setChecked(false);
                ans->setVisible(false);
            }
        }
        av_grp_radiobuttons->setExclusive(true);
    }
}

Question::Answers AnswersView::selectedAnswers()
{
    Question::Answers selected_answers; AnswerView *ans;
    for (int i = 0; i < 9; ++i) {
        ans = av_answers.at(i);
        if (ans->isVisible() && ((ans->ans_checkbox->isVisible() && ans->ans_checkbox->isChecked()) || (ans->ans_radiobutton->isVisible() && ans->ans_radiobutton->isChecked()))) {
            selected_answers |= Question::indexToAnswer(av_ans_order.at(i) + 1);
        }
    }
    return selected_answers;
}

void AnswersView::clear()
{
    av_grp_radiobuttons->setExclusive(false);
    for (int i = 0; i < 9; ++i) {
        AnswerView *ans = av_answers.at(i);
        ans->ans_text->clear();
        ans->ans_checkbox->setChecked(false);
        ans->ans_radiobutton->setChecked(false);
        ans->setVisible(i < 4);

        QPushButton *comp_ans1 = dynamic_cast<QPushButton*>(av_grp_ansbuttons1->button(i));
        comp_ans1->setText("");
        comp_ans1->setVisible(false);
        QPushButton *comp_ans2 = dynamic_cast<QPushButton*>(av_grp_ansbuttons2->button(i));
        comp_ans2->setText("");
        comp_ans2->setVisible(false);
    }
    av_grp_radiobuttons->setExclusive(true);
    av_input_text->setText("");
    av_ans1_compInd = -1; av_ans2_compInd = -1;
    av_comparePairs_label->setText("");
}

void AnswersView::emitButtonReleased(QAbstractButton *)
{
    emit buttonReleased(selectedAnswers());
}

void AnswersView::emitInputReleased()
{
    emit inputReleased(av_input_text->toPlainText());
}

void AnswersView::showOpenQuestion(bool isVisable)
{
    av_inputanswer_label->setVisible(isVisable);
    av_input_text->setVisible(isVisable);
}

void AnswersView::hideComparison()
{
    av_reanswer_button->setVisible(false);
    for (int i = 0; i < 9; ++i)
    {
        QPushButton *ans = dynamic_cast<QPushButton*>(av_grp_ansbuttons1->button(i));
        ans->setVisible(false);
    }
    for (int i = 0; i < 9; ++i)
    {
        QPushButton *ans = dynamic_cast<QPushButton*>(av_grp_ansbuttons2->button(i));
        ans->setVisible(false);
    }
    av_comparePairs_label->setVisible(false);
}

void AnswersView::hideSelectAnswer()
{
    for (int i = 0; i < 9; ++i)
    {
        AnswerView *ans = av_answers.at(i);
        ans->setVisible(false);
    }
}

void AnswersView::compareAnswer1Choosed(int i)
{
    if(av_ans1_compInd!=-1)
    {
        if(av_ans1_compInd==i)
        {
            QPushButton *ans = dynamic_cast<QPushButton*>(av_grp_ansbuttons1->button(i));
            ans->setStyleSheet("background-color: rgb(255, 255, 255);");
            av_ans1_compInd = -1;
            return;
        } else
        {
            QPushButton *ans = dynamic_cast<QPushButton*>(av_grp_ansbuttons1->button(av_ans1_compInd));
            ans->setStyleSheet("background-color: rgb(255, 255, 255);");
        }
    }
    QPushButton *ans = dynamic_cast<QPushButton*>(av_grp_ansbuttons1->button(i));
    ans->setStyleSheet("background-color: rgb(204,243,252);");
    av_ans1_compInd = i;
    if(av_ans2_compInd > -1)
    {
        matchComparison();
    }
}

void AnswersView::compareAnswer2Choosed(int i)
{
    if(av_ans2_compInd!=-1)
    {
        if(av_ans2_compInd==i)
        {
            QPushButton *ans = dynamic_cast<QPushButton*>(av_grp_ansbuttons2->button(i));
            ans->setStyleSheet("background-color: rgb(255, 255, 255);");
            av_ans2_compInd = -1;
            return;
        } else
        {
            QPushButton *ans = dynamic_cast<QPushButton*>(av_grp_ansbuttons2->button(av_ans2_compInd));
            ans->setStyleSheet("background-color: rgb(255, 255, 255);");
        }
    }
    QPushButton *ans = dynamic_cast<QPushButton*>(av_grp_ansbuttons2->button(i));
    ans->setStyleSheet("background-color: rgb(204,243,252);");
    av_ans2_compInd = i;
    if(av_ans1_compInd > -1)
    {
        matchComparison();
    }
}

void AnswersView::matchComparison()
{
    QPushButton *ans1 = dynamic_cast<QPushButton*>(av_grp_ansbuttons1->button(av_ans1_compInd));
    ans1->setStyleSheet("background-color: rgb(255, 255, 255);");
    ans1->setVisible(false);
    QPushButton *ans2 = dynamic_cast<QPushButton*>(av_grp_ansbuttons2->button(av_ans2_compInd));
    ans2->setStyleSheet("background-color: rgb(255, 255, 255);");
    ans2->setVisible(false);
    bool isAll = true;
    for(int i = 0; i < 9; ++i)
    {
         QPushButton *ansCheck = dynamic_cast<QPushButton*>(av_grp_ansbuttons1->button(i));
         if (ansCheck->isVisible()) isAll = false;
    }
    emit pairMatch(av_ans1_compInd, av_ans2_compInd, isAll);

    QString displayText = av_comparePairs_label->text();
    if (!displayText.isEmpty())
        displayText += "  |  ";
    displayText += ans1->text();
    displayText += " - ";
    displayText += ans2->text();
    av_comparePairs_label->setText(displayText);
    emit compareTextChanged(displayText);
    av_ans1_compInd = -1; av_ans2_compInd = -1;
}

void AnswersView::resetAnswersClick()
{
    for (int i = 0; i < 9; ++i)
    {
        QPushButton *ans1 = dynamic_cast<QPushButton*>(av_grp_ansbuttons1->button(i));
        ans1->setStyleSheet("background-color: rgb(255, 255, 255);");
        ans1->setVisible(ans1->text().length()>0);
        QPushButton *ans2 = dynamic_cast<QPushButton*>(av_grp_ansbuttons2->button(i));
        ans2->setStyleSheet("background-color: rgb(255, 255, 255);");
        ans2->setVisible(ans2->text().length()>0);
    }
    emit resetAnswers();
    av_ans1_compInd = -1; av_ans2_compInd = -1;
    av_comparePairs_label->setText("");
}
