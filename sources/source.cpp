// Copyright 2018 Your Name <your_email>

#include <header.hpp>

#include <iostream>
#include <boost/asio.hpp>

using namespace std;

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

    explicit MyClient() : my_socket(context) {}

    string Login_record() {
        cout << "Put your name : ";
        string name;
        cin >> name;
        return name + "\n";
    }

    string Request_record() {
        cout << "Choose request: login, ping or list : " << endl;
        string str;
        cin >> str;
        return str + "\n";
    }

    string Erase_str(string a) {
        a = a.erase(a.size() - 1);
        return a;
    }

    void Check_ping() {
        boost::asio::read_until(my_socket, buffer, '\n');

        std::string check_ping(istreambuf_iterator<char>{&buffer},
                                   istreambuf_iterator<char>{}); // (c) MoraPresence
        check_ping = Erase_str(check_ping);

        cout << check_ping << endl;
    }

    void Ping() {
        std::ostream out(&buffer);
        string ping;
        ping = "ping ok?";
        out << ping << "\n";
        boost::asio::write(my_socket, buffer);

        Check_ping();
    }

    void Login() {
        std::ostream out(&buffer);
        string login = Login_record();
        out << login;
        boost::asio::write(my_socket, buffer);

        boost::asio::read_until(my_socket, buffer, '\n');

        std::string answer_login(istreambuf_iterator<char>{&buffer},
                                 istreambuf_iterator<char>{}); // (c) MoraPresence

        cout << answer_login << endl;
    }

    void Read() {
        boost::asio::read_until(my_socket, buffer, '\n');

        std::string answer_request(istreambuf_iterator<char>{&buffer},
                                   istreambuf_iterator<char>{}); // (c) MoraPresence
        answer_request = Erase_str(answer_request);

        if (answer_request == "Please enter a valid request") {
            cout << answer_request << endl;
            Write();
        } else if (answer_request == "login") {
            Login();
        } else if (answer_request == "ping") {
            Ping();
        } else {
            cout << answer_request << endl;
            //  exit(ERROR_unknown_answer_request);
        }
    }

    void Write() {
        boost::asio::streambuf buffer{};
        std::ostream out(&buffer);
        string request = Request_record();
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

