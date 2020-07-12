#include "headers/PageManager.h"
#include "headers/WelcomePage.h"
#include "headers/Page.h"
#include "headers/StudentSystem.h"

using ui::PageManager;
using ui::Page;
using std::stack;
using std::string;

PageManager::PageManager(string database, string username, string password, string host) {
    initializeSystem(database, username, password, host);
    run = true;
}

PageManager::PageManager() {
    run = true;
}

PageManager::~PageManager() {
    Page *page;

    while (!pages.empty()) {
        page = pages.top();
        pages.pop();
        delete page;
    }
    delete system;
}

void PageManager::initializeSystem(string database, string username, string password, string host) {
    this->system = new StudentSystem(database, username, password, host);
}

void PageManager::showNextPage() {
    if (pages.size() == 0) {
       //no more pages to show, so signal to end the program
       run = false;
    } else {
        Page *nextPage = pages.top();
        nextPage->show();
    }
}

void PageManager::setNextPage(Page *page) {
    pages.emplace(page);
}

void PageManager::popCurrentPage() {
    Page *page = pages.top(); //the page to remove
    pages.pop();
    delete page;
}

Page* PageManager::getNextPage() {
    if (pages.size() != 0) {
        return pages.top();
    } else {
        return nullptr;
    }
}

void PageManager::start(Page *initialPage) {
    if (system != nullptr) {
        setNextPage(initialPage);

        while (run) {
            showNextPage();
        }
    }
}

void PageManager::start() {
    if (system != nullptr) {
        WelcomePage *welcome = new WelcomePage(*this->system);
        start(welcome);
    }
}