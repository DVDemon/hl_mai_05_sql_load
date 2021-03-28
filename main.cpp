#include <string>
#include <iostream>

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>

auto main(int argc,char *argv[]) -> int
{
    if(argc<2) return 0;
    std::string host(argv[1]);
    std::cout << "connecting to:" << host << std::endl;
    Poco::Data::MySQL::Connector::registerConnector();
    std::cout << "connector registered" << std::endl;
    
    std::string connection_str;
    connection_str = "host=";
    connection_str += host;
    connection_str += ";user=test;db=sql_test;password=pzjqUkMnc7vfNHET";

    Poco::Data::Session session(
        Poco::Data::SessionFactory::instance().create(
            Poco::Data::MySQL::Connector::KEY, connection_str));



    try
    {
        Poco::Data::Statement create_stmt(session);
        create_stmt << "CREATE TABLE IF NOT EXISTS `Author` (`id` INT NOT NULL AUTO_INCREMENT,"
                    << "`first_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,"
                    << "`last_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,"
                    << "`email` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,"
                    << "`title` VARCHAR(1024) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,"
                    << "PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`));"; 
        create_stmt.execute();
        std::cout << "table created" << std::endl;

        Poco::Data::Statement insert(session);

        std::string first_name{"Ivan"};
        std::string last_name{"Ivanov"};
        std::string email{"ivanov@ivan.ru"};
        std::string title{"mister"};

        insert << "INSERT INTO Author (first_name,last_name,email,title) VALUES(?, ?, ?, ?)",
            Poco::Data::Keywords::use(first_name),
            Poco::Data::Keywords::use(last_name),
            Poco::Data::Keywords::use(email),
            Poco::Data::Keywords::use(title);

        insert.execute();

        std::cout << "record inserted" << std::endl;

        Poco::Data::Statement select(session);
        long id;
        select << "SELECT id, first_name, last_name, email, title FROM Author",
            Poco::Data::Keywords::into(id),
            Poco::Data::Keywords::into(first_name),
            Poco::Data::Keywords::into(last_name),
            Poco::Data::Keywords::into(email),
            Poco::Data::Keywords::into(title),
            Poco::Data::Keywords::range(0, 1); //  iterate over result set one row at a time

        while (!select.done())
        {
            select.execute();
            std::cout << id << ":[" << first_name << "," << last_name << "," << email << "," << title << "]" << std::endl;
        }
    }
    catch (Poco::Data::MySQL::ConnectionException &e)
    {
        std::cout << "connection:" << e.what() << std::endl;
    }
    catch (Poco::Data::MySQL::StatementException &e)
    {

        std::cout << "statement:" << e.what() << std::endl;
    }
    catch (std::exception ex)
    {
        std::cout << "exception:" << ex.what() << std::endl;
    }
    return 1;
}