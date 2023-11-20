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
#include <QRandomGenerator>

QuestionItem::QuestionItem(const QString &name)
{
     q_name = name;
     q_category = -1;
     q_difficulty = 0;
     q_selectiontype = Question::SingleSelection;
     q_answers << "" << "" << "" << "";
     q_correctanswers = Question::None;
     q_answer = Question::None;
     for (int i = 0; i < 9; ++i) {
         q_ans_order << i;
         q_comp_ans_order << i;
     }
     allCompAnswered = false;
     compText = "";
}

void QuestionItem::shuffleAnswers()
{
    q_ans_order.clear();
    int rand;
    for (int i = 0; i < q_answers.count(); ++i) {
        do {
            rand = QRandomGenerator::global()->bounded(q_answers.count());
        } while (q_ans_order.contains(rand));
        q_ans_order << rand;
    }
}

void QuestionItem::shuffleCompAnswers()
{
    q_comp_ans_order.clear();
    int rand;
    for (int i = 0; i < q_compareAnswers.count(); ++i) {
        do {
            rand = QRandomGenerator::global()->bounded(q_compareAnswers.count());
        } while (q_comp_ans_order.contains(rand));
        q_comp_ans_order << rand;
    }
}

QList<int> QuestionItem::answerOrder() { return q_ans_order; }

QList<int> QuestionItem::compAnswerOrder() {return q_comp_ans_order; }

Question::Answers QuestionItem::answered() { return q_answer; }

QString QuestionItem::str_answered() { return q_str_answer; }

QMap<int, int> QuestionItem::comp_answered() { return q_comp_answer; }

QString QuestionItem::comp_ans_check()
{
    QString res;
    QList<int> keys = q_comp_answer.keys();
    for (int i = 0; i < keys.count(); ++i)
    {
        int k = keys[i];
        res += QString().number(q_ans_order.at(k));
        res += "!";
        res += QString().number(q_comp_ans_order.at(q_comp_answer.value(k)));
        res += "!";
    }
    return res;
}

QString QuestionItem::comp_ans_names()
{
    QString res;
    QList<int> ans;
    for (int i = 0 ; i < q_answers.count(); ++i)
        ans.push_back(-1);
    for (int i = 0; i < q_ans_order.count(); ++i)
    {
        if (q_comp_answer.keys().contains(i))
        {
            int k = q_ans_order.at(i);
            ans[k] = q_comp_ans_order.at(q_comp_answer.value(i));
        }
    }
    for (int i = 0; i < ans.count(); ++i)
    {
        res += QString().number(ans[i]);
        res += "!";
    }
    return res;
}

QString QuestionItem::comp_answered_at(int i)
{
    QString res = q_answers.at(q_ans_order.at(i));
    res += " - ";
    if (q_comp_answer.contains(i))
    {
        int j = q_comp_answer.value(i);
        res += q_compareAnswers.at(q_comp_ans_order.at(j));
    }
    return res;
}

QString QuestionItem::correct_comp_answered_at(int i)
{
    QString res = q_answers.at(q_ans_order.at(i));
    res += " - ";
    res += q_compareAnswers.at(q_ans_order.at(i));
    return res;
}
bool QuestionItem::is_comp_answered_correct(int i)
{
    if (q_comp_answer.contains(i))
        return (q_ans_order.at(i) == q_comp_ans_order.at(q_comp_answer.value(i)));
    return false;
}

bool QuestionItem::isAnswered()
{
    if (q_answer != Question::None) return true;
    if (q_str_answer.length() > 0) return true;
    if(allCompAnswered) return true;
    return false;
}

void QuestionItem::setAnswered(Question::Answers ans) { q_answer = ans; }

void QuestionItem::setAnswered(QString ans) { q_str_answer = ans; }

void QuestionItem::setAnswered(QMap<int, int> ans, bool isAll)
{
    q_comp_answer = ans;
    allCompAnswered = isAll;
}

void QuestionItem::addImageItem(const QString &name, const QString &svg, const bool &isSvg)
{
    q_svglist << name;
    q_svgmap.insert(name, svg);
    q_isSvg << isSvg;
}

int QuestionItem::numImageItems() { return q_svglist.count(); }

QString QuestionItem::imageName(int i) { return q_svglist.at(i); }

QString QuestionItem::imageData(int i) { return q_svgmap.value(q_svglist.at(i), QString()); }

bool QuestionItem::imageTypeIsSvg(int i) { return q_isSvg.at(i); }

ScoringSystem QuestionItem::q_scoringsystem;

ScoringSystem QuestionItem::scoringSystem() { return q_scoringsystem; }

void QuestionItem::setScoringSystem(const ScoringSystem &sys) { q_scoringsystem = sys; }
