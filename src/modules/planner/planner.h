#pragma once
#include "core/lib.h"
#include "core/module.h"
#include "modules/planner/task.h"
#include "modules/planner/objective.h"


class Planner : public Module{
public:

    Planner();
    ~Planner();

    void update() override;

    std::shared_ptr<Task> getTaskToExecute();

    std::shared_ptr<Objective> getCurrentObjective() const;

    void addObjective(const std::shared_ptr<Objective>& objective);
    void removeObjective(const std::shared_ptr<Objective>& objective);

    void loadStrategy(std::deque<std::shared_ptr<Objective>>& objectives, bool erase = true);

    std::shared_ptr<Objective> getCurrentObjective();

private:

    std::deque<std::shared_ptr<Objective>> m_objectives;
    decltype(m_objectives)::iterator m_activeObjective;
};

Planner::Planner() : Module(PLANNER){}
Planner::~Planner(){}

std::shared_ptr<Objective> Planner::getCurrentObjective() const {
        if (m_activeObjective != m_objectives.end()) {
        return *m_activeObjective;
    } else {
        return nullptr;
    }
}

/*
void Planner::nextObjective() {
    if (m_activeObjective != m_objectives.end()) {
        ++m_activeObjective;
    }
}*/

void Planner::loadStrategy(std::deque<std::shared_ptr<Objective>>& objectives, bool erase){
    if (erase) {
        m_objectives.clear();
    }
    std::copy(objectives.begin(), objectives.end(), std::back_inserter(m_objectives));
    m_activeObjective = m_objectives.begin();
}

void Planner::addObjective(const std::shared_ptr<Objective>& objective) {
    m_objectives.push_back(objective);
}

void Planner::removeObjective(const std::shared_ptr<Objective>& objective) {
    m_objectives.erase(std::remove(m_objectives.begin(), m_objectives.end(), objective), m_objectives.end());
}

std::shared_ptr<Objective> Planner::getCurrentObjective() {
    if (!m_objectives.empty()) {
        return m_objectives.front();
    }
    return nullptr; // or throw an exception, or however you want to handle this case
}

void Planner::update() {
    // If there is no active objective, start one
    if (!m_activeObjective) {
        if (m_objectives.empty()) return; // No objectives left
        m_activeObjective = m_objectives.front();
        m_objectives.pop_front();
    }

    // If the active objective is complete, clear it
    if (m_activeObjective->isComplete()) {
        m_activeObjective.reset();
        return;
    }

    // If the active objective is not complete and has no active task, start one
    if (!m_activeTask) {
        if (m_activeObjective->getTasks().empty()) return; // No tasks left in objective
        m_activeTask = m_activeObjective->getTasks().front();
        m_activeObjective->getTasks().pop_front();
        m_activeTask->start();
    }

    // If the active task is complete, clear it
    if (m_activeTask->isComplete()) {
        m_activeTask.reset();
    }

    // If the active task is not complete, update it
    if (m_activeTask) {
        m_activeTask->update();
    }
}