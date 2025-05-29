module;

module main;
import std;
import boost.ut;

int main(int _argc, char* _argv[])
{
    using namespace boost::ut;
    using namespace boost::ut::spec;

    expect(true);

    describe("vector") = []
    {
        std::vector<int> v(5);
        expect(5u == v.size());

        v.resize(10);
        expect(10u == v.size());
    };

    return 0;
}
