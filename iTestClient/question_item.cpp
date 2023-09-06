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
}

void QuestionItem::shuffleAnswers()
{
    q_ans_order.clear();
    int rand;
    for (int i = 0; i < q_answers.count(); ++i) {
        do {
            rand = qrand() % q_answers.count();
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
            rand = qrand() % q_compareAnswers.count();
        } while (q_comp_ans_order.contains(rand));
        q_comp_ans_order << rand;
    }
}

QList<int> QuestionItem::answerOrder() { return q_ans_order; }

QList<int> QuestionItem::compAnswerOrder() {return q_comp_ans_order; }

Question::Answers QuestionItem::answered() { return q_answer; }

QString QuestionItem::str_answered() { return q_str_answer; }

QMap<int, int> QuestionItem::comp_answered() { return q_comp_answer; }

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

void QuestionItem::addSvgItem(const QString &name, const QString &svg)
{
    q_svglist << name;
    q_svgmap.insert(name, svg);
}

int QuestionItem::numSvgItems() { return q_svglist.count(); }

QString QuestionItem::svgName(int i) { return q_svglist.at(i); }

QString QuestionItem::svg(int i) { return q_svgmap.value(q_svglist.at(i), QString()); }

ScoringSystem QuestionItem::q_scoringsystem;

ScoringSystem QuestionItem::scoringSystem() { return q_scoringsystem; }

void QuestionItem::setScoringSystem(const ScoringSystem &sys) { q_scoringsystem = sys; }
