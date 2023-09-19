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

#include "answersedit.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

AnswerEdit::AnswerEdit(int i, AnswersEdit *parent):
QWidget(parent) {
    ans_visible = this->QWidget::isVisible();
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(0, 0, 0, 0); hlayout->setSpacing(6);
    ans_text = new QLineEdit(this);
    ans_text->setStatusTip(tr("Answer %1 of the selected question").arg(Question::indexToLabel(i + 1)));
    ans_correct = new QCheckBox(this);
    ans_correct->setStatusTip(tr("A checked checkbox indicates a correct answer"));
    ans_correct->setMaximumSize(ans_text->sizeHint().height(), ans_text->sizeHint().height());
    ans_remove = new MTToolButton(this, QString::number(i));
    ans_remove->setIcon(QIcon(QString::fromUtf8(":/images/images/list-remove.png")));
    ans_remove->setStatusTip(tr("Remove this answer"));
    ans_remove->setMaximumSize(ans_text->sizeHint().height(), ans_text->sizeHint().height());
    QObject::connect(ans_remove, SIGNAL(released(QString)), parent, SLOT(removeAnswer(QString)));
    ans_label = new QLabel(Question::indexToLabel(i + 1));
    ans_label->setMinimumSize(15, 0);
    hlayout->addWidget(ans_remove);
    hlayout->addWidget(ans_label);
    hlayout->addWidget(ans_text);
    hlayout->addWidget(ans_correct);
}

void AnswerEdit::setVisible(bool visible)
{
    ans_visible = visible;
    this->QWidget::setVisible(visible);
}

bool AnswerEdit::isVisible() { return ans_visible; }

AnswersEdit::AnswersEdit(QWidget *parent):
QWidget(parent) {
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 0, 0, 0);
#ifndef Q_OS_MAC
    vlayout->setSpacing(6);
#else
    vlayout->setSpacing(3);
#endif
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0); hlayout->setSpacing(6);
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->setContentsMargins(0, 0, 0, 0); hlayout2->setSpacing(6);
    //hlayout->addStretch();
    ae_answers_label = new QLabel(tr("Question type:"), this);
    hlayout->addWidget(ae_answers_label);
    //hlayout->addStretch();
    ae_singleselection = new QRadioButton(tr("Single choice"), this);
    ae_singleselection->setStatusTip(tr("Single choice questions allow selecting one answer only, even if the question has more correct answers"));
    ae_singleselection->setChecked(true);
    ae_singleselection->setIcon(QIcon(":/images/images/SingleSelection.svg"));
    hlayout->addWidget(ae_singleselection);
    connect(ae_singleselection, SIGNAL(toggled(bool)), this, SLOT(setSingleSelectionView(bool)));
#ifdef Q_OS_MAC
#if QT_VERSION < 0x040400
    hlayout->addSpacing(6);
#endif
#endif
    ae_multiselection = new QRadioButton(tr("Multiple choice"), this);
    ae_multiselection->setStatusTip(tr("Multiple choice questions allow selecting more answers"));
    ae_multiselection->setIcon(QIcon(":/images/images/MultiSelection.svg"));
    hlayout->addWidget(ae_multiselection);
    connect(ae_multiselection, SIGNAL(toggled(bool)), this, SLOT(setMultiSelectionView(bool)));
    ae_openquestion = new QRadioButton(tr("Open question"), this);
    ae_openquestion->setStatusTip(tr("Open questions allow enter the answer manually"));
    ae_openquestion->setIcon(QIcon(":/images/images/OpenQuestion.svg"));
    hlayout->addWidget(ae_openquestion);
    connect(ae_openquestion, SIGNAL(toggled(bool)), this, SLOT(setOpenQuestionView(bool)));
    ae_comparison = new QRadioButton(tr("Comparison"), this);
    ae_comparison->setStatusTip(tr("Comparison allow match words in pairs"));
    ae_comparison->setIcon(QIcon(":/images/images/Comparison.svg"));
    hlayout->addWidget(ae_comparison);
    connect(ae_comparison, SIGNAL(toggled(bool)), this, SLOT(setComparisonView(bool)));
#ifdef Q_OS_MAC
#if QT_VERSION < 0x040400
    hlayout->addSpacing(6);
#endif
#endif
    hlayout->addStretch();
    ae_openanswer_label = new QLabel(tr("Valid answers:"), this);
    hlayout2->addWidget(ae_openanswer_label);
    hlayout2->addSpacerItem(
                new QSpacerItem(20,40, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred));
    ae_correct_label = new QLabel(tr("Correct:"), this);
    hlayout2->addWidget(ae_correct_label);
    vlayout->addLayout(hlayout);
#ifdef Q_OS_MAC
    QVBoxLayout *vlayout2 = new QVBoxLayout;
    vlayout2->setContentsMargins(0, 0, 0, 0); vlayout2->setSpacing(0);
    vlayout->addLayout(vlayout2);
#endif
    vlayout->addLayout(hlayout2);
    QVBoxLayout *vlayout_ans1 = new QVBoxLayout;
    vlayout_ans1->setContentsMargins(0, 0, 0, 0); vlayout_ans1->setSpacing(6);
    QVBoxLayout *vlayout_ans2 = new QVBoxLayout;
    vlayout_ans2->setContentsMargins(0, 0, 0, 0); vlayout_ans2->setSpacing(6);
    for (int i = 0; i < 9; ++i) {
        AnswerEdit *ans = new AnswerEdit(i, this);
        AnswerEdit *ans_2 = new AnswerEdit(i, this);
        ans_2->ans_label->setText(QString::number(i+1));
        ans_2->ans_correct->setVisible(false);
        ans_2->ans_remove->setVisible(false);
        if (i >= 4) {
            ans->setVisible(false);
            ans_2->setVisible(false);
        }
        ae_answers << ans;
        ae_compare_answers << ans_2;
        vlayout_ans1->addWidget(ans);
        vlayout_ans2->addWidget(ans_2);
    }
    QHBoxLayout *hlayout_ans = new QHBoxLayout;
    hlayout_ans->addLayout(vlayout_ans1);
    hlayout_ans->addLayout(vlayout_ans2);
#ifndef Q_OS_MAC
        vlayout->addLayout(hlayout_ans);
#else
        vlayout2->addLayout(hlayout_ans);
#endif
    ae_add_button = new QToolButton(this);
    ae_add_button->setText(tr("Add answer"));
    ae_add_button->setIcon(QIcon(QString::fromUtf8(":/images/images/list-add.png")));
    QObject::connect(ae_add_button, SIGNAL(released()), this, SLOT(addAnswer()));
    vlayout->addWidget(ae_add_button);
    ae_add_button->setMaximumSize(16777215, ae_answers.at(0)->ans_text->sizeHint().height());
    ae_add_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void AnswersEdit::setAnswers(const QStringList &answers)
{
    for (int i = 0; i < 9; ++i) {
        AnswerEdit *ans = ae_answers.at(i);
        AnswerEdit *ans2 = ae_compare_answers.at(i);
        if (i < answers.count()) {
            ans->ans_text->setText(answers.at(i));
            ans2->ans_text->setText("");
            ans->setVisible(true);
            ans2->setVisible(ae_comparison->isChecked());
        } else {
            ans->ans_text->clear();
            ans->setVisible(false);
            ans2->ans_text->clear();
            ans2->setVisible(false);
        }
    }
    enableAddAnswerButton();
}

void AnswersEdit::setAnswers(const QStringList &answers, Question::Answers correct_answers, Question::SelectionType selectiontype, const QStringList &answers2)
{
    for (int i = 0; i < 9; ++i) {
        AnswerEdit *ans = ae_answers.at(i);
        AnswerEdit *ans2 = ae_compare_answers.at(i);
        if (i < answers.count()) {
            ans->ans_text->setText(answers.at(i));
            ans->ans_correct->setChecked(correct_answers.testFlag(Question::indexToAnswer(i + 1)));
            ans->setVisible(true);
            if(i < answers2.count())
                ans2->ans_text->setText(answers2.at(i));
            else
                ans2->ans_text->setText("");
            ans2->setVisible(ae_comparison->isChecked());
        } else {
            ans->ans_text->clear();
            ans->ans_correct->setChecked(false);
            ans->setVisible(false);
            ans2->ans_text->clear();
            ans2->setVisible(false);
        }
    }
    setSelectionType(selectiontype);
    enableAddAnswerButton();
}

QStringList AnswersEdit::answers()
{
    QStringList answers;
    for (int i = 0; i < 9; ++i) {
        if (ae_answers.at(i)->isVisible()) {
            answers << removeLineBreaks(ae_answers.at(i)->ans_text->text());
        }
    }
    return answers;
}

QStringList AnswersEdit::compareAnswers()
{
    QStringList answers;
    for (int i = 0; i < 9; ++i) {
        if (ae_compare_answers.at(i)->isVisible()) {
            answers << removeLineBreaks(ae_compare_answers.at(i)->ans_text->text());
        }
    }
    return answers;
}

void AnswersEdit::setCorrectAnswers(Question::Answers correct_answers)
{
    for (int i = 0; i < 9; ++i) {
        ae_answers.at(i)->ans_correct->setChecked(correct_answers.testFlag(Question::indexToAnswer(i + 1)));
    }
}

Question::Answers AnswersEdit::correctAnswers()
{
    Question::Answers correct_answers;
    for (int i = 0; i < 9; ++i) {
        if (ae_answers.at(i)->isVisible() && ae_answers.at(i)->ans_correct->isChecked()) {
            correct_answers |= Question::indexToAnswer(i + 1);
        }
    }
    return correct_answers;
}

void AnswersEdit::replaceAnswer(int i, const QString &text)
{
    if (i < 0 || i > 8)
        return;
    ae_answers.at(i)->ans_text->setText(text);
    ae_answers.at(i)->setVisible(true);
}

QString AnswersEdit::answer(int i)
{
    if (i < 0 || i > 8)
        return QString();
    return removeLineBreaks(ae_answers.at(i)->ans_text->text());
}

void AnswersEdit::setSingleSelectionView(bool check)
{
    if (check)
    {
        ae_correct_label->setVisible(true);
        ae_openanswer_label->setVisible(false);
        for (int i = 0; i < 9; ++i)
            ae_answers.at(i)->ans_correct->setVisible(true);
        for (int i = 0; i < 9; ++i)
            ae_compare_answers.at(i)->setVisible(false);
    }
}

void AnswersEdit::setMultiSelectionView(bool check)
{
    if (check)
    {
        ae_correct_label->setVisible(true);
        ae_openanswer_label->setVisible(false);
        for (int i = 0; i < 9; ++i)
            ae_answers.at(i)->ans_correct->setVisible(true);
        for (int i = 0; i < 9; ++i)
            ae_compare_answers.at(i)->setVisible(false);
    }
}

void AnswersEdit::setOpenQuestionView(bool check)
{
    if (check)
    {
        ae_correct_label->setVisible(false);
        ae_openanswer_label->setVisible(true);
        for (int i = 0; i < 9; ++i)
            ae_answers.at(i)->ans_correct->setVisible(false);
        for (int i = 0; i < 9; ++i)
            ae_compare_answers.at(i)->setVisible(false);
    }
}

void AnswersEdit::setComparisonView(bool check)
{
    if (check)
    {
        ae_correct_label->setVisible(false);
        ae_openanswer_label->setVisible(false);
        for (int i = 0; i < 9; ++i)
            ae_answers.at(i)->ans_correct->setVisible(false);
        for (int i = 0; i < 9; ++i)
            ae_compare_answers.at(i)->setVisible(ae_answers.at(i)->isVisible());
    }
}

void AnswersEdit::setSelectionType(Question::SelectionType type)
{
    switch (type) {
        case Question::SingleSelection:
        {
            ae_singleselection->setChecked(true);
            setSingleSelectionView(true);
            break;
        }
        case Question::MultiSelection:
        {
            ae_multiselection->setChecked(true);
            setMultiSelectionView(true);
            break;
        }
        case Question::OpenQuestion:
        {
            ae_openquestion->setChecked(true);
            setOpenQuestionView(true);
            break;
        }
        case Question::Comparison:
        {
            ae_comparison->setChecked(true);
            setComparisonView(true);
            break;
        }
        default:
        {
            ae_singleselection->setChecked(true);
            setSingleSelectionView(true);
            break;
        }
    }
}

Question::SelectionType AnswersEdit::selectionType()
{
    if(ae_multiselection->isChecked())
        return Question::MultiSelection;
    if(ae_openquestion->isChecked())
        return Question::OpenQuestion;
    if(ae_comparison->isChecked())
        return Question::Comparison;
    return Question::SingleSelection;
}

int AnswersEdit::count()
{
    int n = 0;
    for (int i = 0; i < 9; ++i) {
        if (ae_answers.at(i)->isVisible()) {
            n++;
        }
    }
    return n;
}

void AnswersEdit::addAnswer()
{
    for (int i = 0; i < 9; ++i) {
        if (!ae_answers.at(i)->isVisible()) {
            ae_answers.at(i)->ans_text->clear();
            ae_answers.at(i)->ans_correct->setChecked(false);
            ae_answers.at(i)->setVisible(true);
            ae_compare_answers.at(i)->ans_text->clear();
            ae_compare_answers.at(i)->setVisible(ae_comparison->isChecked());
            enableAddAnswerButton();
            break;
        }
    }
}

void AnswersEdit::removeAnswer(const QString &id)
{
    bool ok;
    int i = id.toInt(&ok);
    if (ok) {
        removeAnswer(i);
    }
}

void AnswersEdit::removeAnswer(int i)
{
    if (i < 0 || i > 8)
        return;
    AnswerEdit *prev_ans = ae_answers.at(i);
    AnswerEdit *prev_ans2 = ae_compare_answers.at(i);
    for (int n = i + 1; n < 9; ++n) {
        if (ae_answers.at(n)->isVisible()) {
            prev_ans->ans_text->setText(ae_answers.at(n)->ans_text->text());
            prev_ans->ans_correct->setChecked(ae_answers.at(n)->ans_correct->isChecked());
            prev_ans = ae_answers.at(n);

            prev_ans2->ans_text->setText(ae_compare_answers.at(n)->ans_text->text());
            prev_ans2 = ae_compare_answers.at(n);
        }
    }
    prev_ans->ans_text->clear();
    prev_ans->ans_correct->setChecked(false);
    prev_ans->setVisible(false);
    prev_ans2->ans_text->clear();
    prev_ans2->setVisible(false);
    enableAddAnswerButton();
}

void AnswersEdit::clear()
{
    for (int i = 0; i < 9; ++i) {
        AnswerEdit *ans = ae_answers.at(i);
        ans->ans_text->clear();
        ans->ans_correct->setChecked(false);
        ans->setVisible(i < 4);
    }
    enableAddAnswerButton();
}

void AnswersEdit::enableAddAnswerButton()
{
    int n = 0;
    for (int i = 0; i < 9; ++i) {
        if (ae_answers.at(i)->isVisible()) {
            n++;
        }
    }
    ae_add_button->setEnabled(n < 9);
}
