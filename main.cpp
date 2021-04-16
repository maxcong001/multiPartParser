#include "multipart_reader.hpp"
#include "iostream"

multipart_reader multipart_parser_;
void init_multipart_parser()
{
    multipart_parser_.on_part_begin = [](const multipart_headers &headers) {
        for (auto it = headers.begin(); it != headers.end(); ++it)
        {
            std::cout << (*it).first << " => " << (*it).second << std::endl;
        }
    };
    multipart_parser_.on_part_data = [](const char *buf, size_t size) {
        std::string body(buf, size);
        std::cout << "body is :" << body << std::endl;
    };
    multipart_parser_.on_part_end = [] {

    };
    multipart_parser_.on_end = [] {

    };
}

bool parse_multipart(std::string &multipart_body)
{
    std::cout << "input body is : " << multipart_body << std::endl;
    std::cout << "size of the body is :" << multipart_body.size() << std::endl;
    size_t bufsize = multipart_body.length();

    size_t fed = 0;
    do
    {
        size_t ret = multipart_parser_.feed(multipart_body.data() + fed,
                                            multipart_body.length() - fed);

        fed += ret;
    } while (fed < bufsize && !multipart_parser_.stopped());

    if (multipart_parser_.has_error())
    {
        // log?
        std::cout << "parse error" << std::endl;
        return true;
    }

    return false;
}
int main()
{
    std::string testStr = R"()";

    std::string body = "--82ebdd74\r\n"
                       "Content-Type: application/json\r\n\r\n"
                       "{\"test\":\"test\"}"
                       "\r\n--82ebdd74\r\n"
                       "Content-Type: application/vnd.3gpp.5gnas\r\n"
                       "Content-Id: n1msg\r\n\r\n"
                       "FFFFFF"
                       "\r\n--82ebdd74--\r\n\r\n";
    init_multipart_parser();
    multipart_parser_.set_boundary("\r\n--82ebdd74");
    parse_multipart(body);
}
