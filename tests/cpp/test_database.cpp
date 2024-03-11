#include <gtest/gtest.h>

#include <thread>

#include "database.h"
#include "barrier.h"

void add_command(const std::vector<std::string>& data,Database* database) {
    for (auto command: data)
    {
        database->execute_command(command);
    }
}



TEST(Database_test, check_tables) {
    Database* test_database = Database::get_Database();
    auto tables_from_database = test_database->get_tables();
    auto el = tables_from_database.find("A");
    ASSERT_EQ(el != tables_from_database.end(), 1);

    el = tables_from_database.find("B");
    ASSERT_EQ(el != tables_from_database.end(), 1);

    el = tables_from_database.find("C");
    ASSERT_EQ(el == tables_from_database.end(), 1);

}

TEST(Database_test, check_execute_command_errors) {
    Database* test_database = Database::get_Database();

    std::string test_arguments = {};
    ASSERT_EQ(test_database->execute_command(test_arguments), "ERR Command is empty\n");



    test_arguments = "UNCORRECT_ARGUMENT A 1 Data";
    ASSERT_EQ(test_database->execute_command(test_arguments), "ERR Unknown command\n");

    test_arguments = "INSERT A 1 Test_data";
    ASSERT_EQ(test_database->execute_command(test_arguments), "OK\n");

    test_arguments = "TRUNCATE A";
    ASSERT_EQ(test_database->execute_command(test_arguments), "OK\n");

    test_arguments = "INTERSECTION";
    ASSERT_EQ(test_database->execute_command(test_arguments), "OK\n");

    test_arguments = "SYMMETRIC_DIFFERENCE";
    ASSERT_EQ(test_database->execute_command(test_arguments), "OK\n");
}

TEST(Database_test, check_insert_command_errors) {
    Database* test_database = Database::get_Database();

    std::vector<std::string> test_arguments = {"A", "0","Test string","additional argument"};
    ASSERT_EQ(test_database->insert_command(test_arguments),"ERR Uncorrect arguments\n");

    test_arguments = { "C", "0","Test string" };
    ASSERT_EQ(test_database->insert_command(test_arguments), "ERR Unknown table C\n");

    test_arguments = { "A", "uncorrect_index","Test string" };
    ASSERT_EQ(test_database->insert_command(test_arguments), "ERR Uncorrect index\n");
}

TEST(Database_test, check_truncate_command_errors) {
    Database* test_database = Database::get_Database();

    std::vector<std::string> test_arguments = { "A", "additional argument" };
    ASSERT_EQ(test_database->truncate_command(test_arguments), "ERR Uncorrect arguments\n");

    test_arguments = { "C" };
    ASSERT_EQ(test_database->truncate_command(test_arguments), "ERR Unknown table\n");

}


TEST(Database_test, check_insert_command) {
    Database* test_database = Database::get_Database();

    std::vector<std::string> test_arguments = { "A", "0","Test string"};
    test_database->insert_command(test_arguments);
    auto table = (test_database->get_tables())["A"];
    ASSERT_TRUE(table->get_data().size());
    ASSERT_NE(table->get_data().find(0), table->get_data().end());
    ASSERT_EQ(table->get_data().find(0)->second, "Test string");

}

TEST(Database_test, check_truncate_command) {
    Database* test_database = Database::get_Database();

    std::vector<std::string> test_arguments = { "A"};
    test_database->truncate_command(test_arguments);
    auto table = (test_database->get_tables())["A"];
    ASSERT_TRUE(table->get_data().empty());


}

TEST(Database_test, check_insersect_command) {
    Database* test_database = Database::get_Database();
    std::vector<std::string> input_command_first_table = { "INSERT A 0 lean",
                                                           "INSERT A 1 sweater",
                                                           "INSERT A 2 frank",
                                                           "INSERT A 3 violation",
                                                           "INSERT A 4 quality",
                                                           "INSERT A 5 precision" };
    
    std::vector<std::string> input_command_second_table = { "INSERT B 3 proposal",
                                                            "INSERT B 4 example",
                                                            "INSERT B 5 lake",
                                                            "INSERT B 6 flour",
                                                            "INSERT B 7 wonder",
                                                            "INSERT B 8 selection" };

    add_command(input_command_first_table, test_database);
    add_command(input_command_second_table, test_database);

    std::string expected_data("3,violation,proposal\n4,quality,example\n5,precision,lake\nOK\n");
    std::string command = std::string("INTERSECTION");
    ASSERT_EQ(expected_data,test_database->execute_command(command));


}

TEST(Database_test, check_symmetric_difference_command) {
    Database* test_database = Database::get_Database();
    std::string expected_data("0,lean,\n1,sweater,\n2,frank,\n6,,flour\n7,,wonder\n8,,selection\nOK\n");
    std::string command = std::string("SYMMETRIC_DIFFERENCE");
    ASSERT_EQ(expected_data, test_database->execute_command(command));
    
}

TEST(Database_test, check_multithreading) {
    Database* test_database = Database::get_Database();

    std::string command_1 = std::string("TRUNCATE A");
    std::string command_2 = std::string("TRUNCATE B");


    test_database->execute_command(command_1);
    test_database->execute_command(command_2);

    std::string first_output;
    std::string second_output;

    
    c_barrier barrier(2);
    auto thread_runner = [&barrier](const std::vector<std::string>& commands) {
        Database* test_database = Database::get_Database();
        barrier.wait();
        for (std::string comm : commands)
        {
            test_database->execute_command(comm);
        }
    
    };


    std::vector<std::string> first_command_vector;
    std::vector<std::string> second_command_vector;

    std::map<int, std::string> expected_output;


    for (size_t i = 0; i < 100000; i++)
    {
        std::string command = "INSERT A " + std::to_string(i) + " lean_" + std::to_string(i);
        first_command_vector.push_back(command);
    }

    for (size_t i = 50000; i < 150000; i++)
    {
        std::string command = "INSERT A " + std::to_string(i) + " lean_" + std::to_string(i);
        second_command_vector.push_back(command);
    }

    for (size_t i = 0; i < 150000; i++)
    {
        std::string command ="lean_" + std::to_string(i);
        expected_output.emplace(i,command);

    }
    
    
    std::thread thr1{ thread_runner, first_command_vector };
    std::thread thr2{ thread_runner, second_command_vector };

    thr1.join();
    thr2.join();

    std::map<int, std::string> output_data = test_database->get_tables()["A"]->get_data();

    ASSERT_EQ(output_data, expected_output);

}