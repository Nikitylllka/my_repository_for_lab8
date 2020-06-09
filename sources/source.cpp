// Copyright 2018 Your Name <your_email>

#include <header.hpp>

#include <iostream>
#include <boost/asio.hpp>



using std::thread;
using std::exception;
using sock = boost::asio::ip::tcp::socket;
using acceptor = boost::asio::ip::tcp::acceptor;
using endpoint = boost::asio::ip::tcp::endpoint;
using input_ip = boost::asio::ip::address;
using io_context = boost::asio::io_context;
const char ip_sock[] = "127.0.0.1";
const char ERROR_unknown_answer_request = 1;

struct MyClient {
public:
    io_context context;
    sock my_socket;
    boost::asio::streambuf buffer{};

    MyClient() : my_socket(context) {}

    std::string Login_record() {
        std::cout << "Put your name : ";
        std::string name;
        std::cin >> name;
        return name + "\n";
    }

    std::string Request_record() {
        std::cout << "Choose request: login, ping or list : " << std::endl;
        std::string str;
        std::cin >> str;
        return str + "\n";
    }

    std::string Erase_str(std::string a) {
        a = a.erase(a.size() - 1);
        return a;
    }

    void Check_ping() {
        boost::asio::read_until(my_socket, buffer, '\n');

        std::string check_ping(std::istreambuf_iterator<char>{&buffer},
                              std::istreambuf_iterator<char>{});
        check_ping = Erase_str(check_ping);

        std::cout << check_ping << std::endl;
    }

    void Ping() {
        std::ostream out(&buffer);
        std::string ping;
        ping = "ping ok?";
        out << ping << "\n";
        boost::asio::write(my_socket, buffer);

        Check_ping();
    }

    void Login() {
        std::ostream out(&buffer);
        std::string login = Login_record();
        out << login;
        boost::asio::write(my_socket, buffer);

        boost::asio::read_until(my_socket, buffer, '\n');

        std::string answer_login(std::istreambuf_iterator<char>{&buffer},
                                 std::istreambuf_iterator<char>{});

        std::cout << answer_login << std::endl;
    }

    void Read() {
        boost::asio::read_until(my_socket, buffer, '\n');

           std::string answer_request(std::istreambuf_iterator<char>{&buffer},
                              std::istreambuf_iterator<char>{});
        answer_request = Erase_str(answer_request);

        if (answer_request == "Please enter a valid request") {
            std::cout << answer_request << std::endl;
            Write();
        } else if (answer_request == "login") {
            Login();
        } else if (answer_request == "ping") {
            Ping();
        } else {
            std::cout << answer_request << std::endl;
            //  exit(ERROR_unknown_answer_request);
        }
    }

    void Write() {
        boost::asio::streambuf buffer{};
        std::ostream out(&buffer);
        std::string request = Request_record();
        out << request;
        boost::asio::write(my_socket, buffer);

        Read();
    }

    void Connect() {
        endpoint ep(input_ip::from_string(ip_sock), 8001);
        my_socket.connect(ep);

        while (true) {
            Write();
        }
    }

    void Start() {
        Connect();
    }
};

int main() {
    MyClient c1;
    c1.Start();
    return 0;
}

