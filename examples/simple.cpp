#include <boost/idl/mirror_interface.hpp>
#include <boost/idl/idl.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "cli.hpp"

struct Service
{
    std::string name()const;
    int         exit();
};
struct Calculator : Service
{
    double add( double v );           
    double add2( double v, double v2 );
    double sub( double v );           
    double mult( double v );           
    double div( double v );           
    double result()const;
};

BOOST_IDL_INTERFACE( Service, BOOST_PP_NIL, (name)(exit) )
BOOST_IDL_INTERFACE( Calculator, (Service, BOOST_PP_NIL), (add)(add2)(sub)(mult)(div)(result) )

class CalculatorService
{
    public:
        CalculatorService():m_result(0){}

        std::string name()const            { return "CalculatorService"; }
        int   exit()                       { ::exit(0);                  }
        double add( double v )             { return m_result += v;       }
        double sub( double v )             { return m_result -= v;       }
        double mult( double v )            { return m_result *= v;       }
        double div( double v )             { return m_result /= v;       }
        double add2( double v, double v2 ) { return m_result += v + v2;  }

        double result()const { return m_result; }

    private:
        double m_result;
};


int main( int argc, char** argv )
{
    boost::idl::any<Calculator> s = CalculatorService();

    printf( "Result: %f\n", s.result() );

    cli  m_cli;
    m_cli.start_visit(s);

    std::string line;
    std::string cmd;
    std::string args;

    while( true )
    {
        std::cerr << "Enter Method: ";
        std::getline( std::cin, line );
        cmd = line.substr( 0, line.find('(') );
        args = line.substr( cmd.size(), line.size() );
        std::cerr << m_cli[cmd](args) << std::endl;
    }

    return 0;
}
