#pragma once
#include "system/core/lib.h"
#include "system/core/job.h"




class Objective {
public:
    Objective(const std::string& name, int points, std::function<bool()> condition = []{return true;});
    ~Objective();

    bool isComplete() const;
    std::deque<std::shared_ptr<Task>>& getTasks();

    void addTask(std::shared_ptr<Task> task);

private:
    std::string m_name;
    int m_points;
    std::function<bool()> m_condition;
    std::deque<std::shared_ptr<Task>> m_tasks;
};

Objective::Objective(const std::string& name, int points, std::function<bool()> condition)
    : m_name(name), m_points(points), m_condition(condition) {}

Objective::~Objective() {}

bool Objective::isComplete() const {
    // An objective is complete if all tasks are complete
    // This assumes that a Task has a isComplete() method
    for (const auto& task : m_tasks) {
        if (!task->isCompleted()) {
            return false;
        }
    }
    return true;
}

std::deque<std::shared_ptr<Task>>& Objective::getTasks() {
    return m_tasks;
}

void Objective::addTask(std::shared_ptr<Task> task) {
    m_tasks.push_back(std::move(task));
}