//
// Created by Alen on 16/03/2022.
//

#include "Ui.h"

void UserInterface::ui_add() {

    char link[61], title[41], description[41], date_time[41], people_number[41];
    unsigned int new_people_number;

    std::cout << "Link:\n";
    std::cin.getline(link, 61);
    if (link[0] == '\0')
        throw UserError("Invalid link\n");

    std::cout << "Title:\n";
    std::cin.getline(title, 41);
    if (title[0] == '\0')
        throw UserError("Invalid title\n");

    std::cout << "Description:\n";
    std::cin.getline(description, 41);
    if (description[0] == '\0')
        throw UserError("Invalid description\n");

    std::cout << "Date and time (dd/mm/yyyy hh:mm):\n";
    std::cin.getline(date_time, 41);
    if (date_time[0] == '\0')
        throw UserError("Invalid date and time\n");

    std::cout << "Number of people:\n";
    std::cin.getline(people_number, 41);
    if (people_number[0] == '\0')
        throw UserError("Invalid number of people\n");
    new_people_number = std::stoi(people_number);

    this->event_service.add_event(link, title, description, date_time, new_people_number);
}

void UserInterface::ui_delete() {

    char link[61];
    std::cout << "Link:\n";
    std::cin.getline(link, 61);
    if (link[0] == '\0')
        throw UserError("Invalid link\n");
    this->event_service.remove_event(link);
}

void UserInterface::ui_update() {

    char link[41], title[41], description[41], date_time[41], people_number[41];
    unsigned int new_people_number;

    std::cout << "Link:\n";
    std::cin.getline(link, 41);
    if (link[0] == '\0')
        throw UserError("Invalid link\n");

    std::cout << "Title:\n";
    std::cin.getline(title, 41);
    if (title[0] == '\0')
        throw UserError("Invalid title\n");

    std::cout << "Description:\n";
    std::cin.getline(description, 41);
    if (description[0] == '\0')
        throw UserError("Invalid description\n");

    std::cout << "Date and time (dd/mm/yyyy hh:mm):\n";
    std::cin.getline(date_time, 41);
    if (date_time[0] == '\0')
        throw UserError("Invalid date and time\n");

    std::cout << "Number of people:\n";
    std::cin.getline(people_number, 41);
    if (people_number[0] == '\0')
        throw UserError("Invalid number of people\n");
    new_people_number = std::stoi(people_number);

    this->event_service.update_event(link, title, description, date_time, new_people_number);
}

void UserInterface::ui_list() {

    std::vector<Event> events = this->event_service.list_events();
    for(const auto& event: events)
        std::cout << event;
}

void UserInterface::ui_save(const std::string& file_name) {

    this->event_service.add_to_file(file_name);
}

void UserInterface::ui_read(const std::string& file_name) {

    this->event_service.read_from_file(file_name);
}

void UserInterface::ui_user_show_events() {

    char month[41], command[41];
    unsigned int int_month;

    while(true)
    {
        try {
            std::cout << "Please enter a month (1-12):\n";
            std::cin.getline(month, 41);
            int_month = std::stoi(month);
            if (int_month <= 0 || int_month >= 13)
                throw ValidError("Invalid month");
            break;
        }
        catch (ValidError &valid_error) {
            std::cout << valid_error.get_message() << std::endl;
        }
        catch (const std::invalid_argument &ia) {
            std::cout << "Invalid month" << std::endl;
        }
    }

    if(this->event_service.get_number_events_for_month(int_month) == 0)
    {
        std::cout << "No events for this month\n";
        return;
    }

    if(this->event_service.get_number_events_for_month(int_month) == this->user_service.get_number_events_for_month(int_month))
    {
        std::cout << "All events available for this month are already in your list of events\n";
        return;
    }

    std::cout << "Here are your options:\n";
    std::cout << "'add' - Add event to your list\n";
    std::cout << "'next' - Ignore this event\n";
    std::cout << "'exit' - Exit this menu\n";

    std::vector<Event> all_events = this->event_service.filter_array(int_month);
    std::vector<Event> user_events = this->user_service.filter_array(int_month);

    bool valid;

    while(all_events.size() != user_events.size()) {
        for (auto &event : all_events) {

            valid = true;
            for(auto& user_event : this->user_service.list_events())
                if(user_event.get_link() == event.get_link())
                {
                    valid = false;
                    break;
                }

            while (valid) {
                std::cout << event;
                //system(std::string("start " + event.get_link()).c_str());
                std::cout << "Please enter your option:\n";
                std::cin.getline(command, 41);
                if (!strcmp(command, "add")) {
                    this->user_service.add_event(event.get_link(),
                                                 event.get_title(),
                                                 event.get_description(),
                                                 event.get_date_time(),
                                                 event.get_people_number() + 1);

                    this->event_service.update_event(event.get_link(),
                                                     event.get_title(),
                                                     event.get_description(),
                                                     event.get_date_time(),
                                                     event.get_people_number() + 1);

                    if (this->event_service.get_number_events_for_month(int_month) == this->user_service.get_number_events_for_month(int_month)) {
                        std::cout << "All events available for this month are already in your list of events\n";
                        return;
                    }
                    break;
                } else if (!strcmp(command, "next"))
                    break;
                else if (!strcmp(command, "exit"))
                    return;
                else
                    std::cout << "Invalid command\n";
            }
        }
    }
    std::cout << "All events available for this month are already in your list of events\n";
}

void UserInterface::ui_user_my_events() {

    char command[41];

    if(this->user_service.get_number_events() == 0)
    {
        std::cout << "You are not interested in any event\n";
        return;
    }

    std::vector<Event> user_events = this->user_service.list_events();
    for(const auto& user_event: user_events)
        std::cout << user_event;

    std::cout << "Here are your options:\n";
    std::cout << "'delete' - Remove an event from your list\n";
    std::cout << "'exit' - Exit this menu\n";

    while(true)
    {
        try {
            std::cout << "Please enter your option:\n";
            std::cin.getline(command, 41);
            if (!strcmp(command, "delete"))
            {
                char link[61];
                std::cout << "Link:\n";
                std::cin.getline(link, 61);
                this->user_service.remove_event(link);

                std::vector<Event> events = this->event_service.list_events();
                for(auto& event: user_events)
                    if(event.get_link() == link)
                        this->event_service.update_event(event.get_link(), event.get_title(), event.get_description(), event.get_date_time(), event.get_people_number() - 1);

            }
            else if (!strcmp(command, "exit"))
                return;
            else
            {
                std::cout << "Invalid command\n";
                command[0] = '\0';
            }
        }
        catch (RepoError &repo_error) {
            std::cout << repo_error.get_message() << std::endl;
        }
        catch (ValidError &valid_error) {
            std::cout << valid_error.get_message() << std::endl;
        }
        catch (UserError &user_error) {
            std::cout << user_error.get_message() << std::endl;
        }
    }
}

void UserInterface::ui_user_save(const std::string& file_name) {

    this->user_service.add_to_generic_file(file_name);
    this->user_service.open_file(file_name);
}

void UserInterface::print_admin_menu() {

    std::cout << "Here is a list of your options (Admin Mode):\n";
    std::cout << "-------------------------------\n";
    std::cout << "'add': Add an event\n";
    std::cout << "'delete': Delete an event\n";
    std::cout << "'update': Update an event\n";
    std::cout << "-------------------------\n";
    std::cout << "'list': List all events\n";
    std::cout << "-----------------------\n";
    std::cout << "'save': Save content\n";
    std::cout << "'read': Read content from file\n";
    std::cout << "------------------------------\n";
    std::cout << "'exit': Exit admin mode\n";
}

void UserInterface::print_user_menu() {

    std::cout << "Here is a list of your options (User Mode):\n";
    std::cout << "-------------------------------\n";
    std::cout << "'show events': Lists all available events for a month for you to choose\n";
    std::cout << "'my events': Lists all events you're interested in\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "'save': Save content\n";
    std::cout << "------------------------------\n";
    std::cout << "'exit' - Exit user mode\n";
}

void UserInterface::run(const std::string& repo_type)
{
    char command[41], user_command[41], admin_command[41];

    std::cout << "Welcome to the Event Calendar!!!\n";
    std::cout << "Please enter a option: \n'admin' - To enter admin mode\n'user' - To enter user mode\n'exit' - Exit\n";

    this->event_service.populate_array();

    while (true)
    {
        std::cout << "Please enter a mode (Admin or User):\n";
        std::cin.getline(command, 41);
        if (!strcmp(command, "user"))
        {
            while (true)
            {
                this->print_user_menu();
                std::cout << "Please enter your option:\n";
                std::cin.getline(user_command, 41);
                try {
                    if(!strcmp(user_command, "show events"))
                        this->ui_user_show_events();
                    else if (!strcmp(user_command, "my events"))
                        this->ui_user_my_events();
                    else if (!strcmp(user_command, "save"))
                    {
                        if(repo_type == "html")
                            this->ui_user_save("user.html");
                        else
                            this->ui_user_save("user.csv");
                        this->ui_user_save("user.txt");
                    }
                    else if (!strcmp(user_command, "exit"))
                        break;
                    else
                        std::cout << "Invalid command\n";
                }
                catch (RepoError &repo_error) {
                    std::cout << repo_error.get_message() << std::endl;
                }
                catch (ValidError &valid_error) {
                    std::cout << valid_error.get_message() << std::endl;
                }
                catch (UserError &user_error) {
                    std::cout << user_error.get_message() << std::endl;
                }
            }
        }
        else if (!strcmp(command, "admin"))
        {
            this->print_admin_menu();
            while (true) {
                std::cout << "Please enter your option:\n";
                std::cin.getline(admin_command, 41);
                try {
                    if (!strcmp(admin_command, "add"))
                        this->ui_add();
                    else if (!strcmp(admin_command, "delete"))
                        this->ui_delete();
                    else if (!strcmp(admin_command, "update"))
                        this->ui_update();
                    else if (!strcmp(admin_command, "list"))
                        this->ui_list();
                    else if (!strcmp(admin_command, "exit"))
                        break;
                    else if (!strcmp(admin_command, "save"))
                        this->ui_save(R"(C:\Users\Alen\Documents\GitHub\a8-9-913-Iaguta-Alen-Mihael\admin_save.txt)");
                    else if (!strcmp(admin_command, "read"))
                        this->ui_read(R"(C:\Users\Alen\Documents\GitHub\a8-9-913-Iaguta-Alen-Mihael\admin_read.txt)");
                    else
                        std::cout << "Invalid command!\n";
                }
                catch (RepoError &repo_error) {
                    std::cout << repo_error.get_message() << std::endl;
                }
                catch (ValidError &valid_error) {
                    std::cout << valid_error.get_message() << std::endl;
                }
                catch (UserError &user_error) {
                    std::cout << user_error.get_message() << std::endl;
                }
                catch (const std::invalid_argument &ia) {
                    std::cout << "Invalid number of people" << std::endl;
                }
            }
        }
        else if (!strcmp(command, "exit"))
            return;
        else
            std::cout << "Invalid mode!\n";
    }
}
