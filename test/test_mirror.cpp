/**
\file

\author Mattia Basaglia

\section License

Copyright (C) 2015  Mattia Basaglia

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#define BOOST_TEST_MODULE Test_Mirror
#include "object/dynamic_mirror.hpp"
#include <boost/test/unit_test.hpp>

class TestClass : public object::Mirror
{
    MIRROR(TestClass,object::Mirror)

public:
    int foo = 5;

    int foo3() const { return foo+3; }
    void set_foo(int x) { foo = x; }
    int get_foo() const { return foo; }

    int foo_plus(int i) { return foo+i; }
    void void_method() { foo = 10; }

    util::Any get_foo_any() const { return foo; }
    std::string cat(const Mirror::Arguments& args)
    {
        std::stringstream ss;
        for ( const auto& a: args )
            ss << a;
        return ss.str();
    }
};
DECLARE_PROPERTY_GET_ADVANCED(TestClass,"f@@1",foo1,[](const TestClass& tc) { return tc.foo+1; })
DECLARE_PROPERTY_GET_FUNCTOR(TestClass, foo2, [](const TestClass& tc) { return tc.foo+2; })
DECLARE_PROPERTY_GETTER(TestClass, foo3, foo3)

DECLARE_PROPERTY_SET_ADVANCED(TestClass,"f@@1",foo1,[](TestClass& tc, int i) { tc.foo = i; })
DECLARE_PROPERTY_SET_FUNCTOR(TestClass, foo2, [](TestClass& tc, int i) { tc.foo = i; } )
DECLARE_PROPERTY_SETTER(TestClass, foo3, set_foo)

DECLARE_PROPERTY(TestClass,foo,get_foo,set_foo)
DECLARE_PROPERTY_RENAME(TestClass,"f@@",weird_foo,get_foo,set_foo)

DECLARE_METHOD_ADVANCED(TestClass,"meth@d1",method1,[](TestClass& tc, const ::object::Mirror::Arguments&) { return tc.foo+1; })
DECLARE_METHOD_FUNCTOR(TestClass,method2,[](TestClass& tc, const ::object::Mirror::Arguments&) { return tc.foo+2; })
DECLARE_METHOD(TestClass,get_foo)
DECLARE_METHOD(TestClass,void_method)
DECLARE_METHOD(TestClass,foo_plus)
DECLARE_METHOD(TestClass,set_foo)
DECLARE_METHOD(TestClass,get_foo_any)
DECLARE_METHOD(TestClass,cat)

class TestDerived : public TestClass
{
    MIRROR(TestDerived,TestClass)
public:
    int bar = 7;
};

DECLARE_PROPERTY_ATTRIBUTE(TestDerived,bar)

class TestDynamic : public object::Dynamic_Mirror
{
    MIRROR(TestDynamic,object::Dynamic_Mirror)
public:
    int member = 1;

};
DECLARE_PROPERTY_ATTRIBUTE(TestDynamic,member)

TestClass base_obj;
TestDerived derived_obj;


BOOST_AUTO_TEST_CASE( test_any )
{
    util::Any any;
    BOOST_CHECK ( any.empty() );
    BOOST_CHECK ( !any.pointer() );
    any = 5;
    BOOST_CHECK ( any.cast<int>() == 5 );
    BOOST_CHECK ( any.cast<long>() == 0 );
    BOOST_CHECK ( any.to_string() == "5" );
    BOOST_CHECK ( any == 5 );
    BOOST_CHECK ( any == any );
    BOOST_CHECK ( any != util::Any() );
    BOOST_CHECK ( util::Any() == util::Any() );
    BOOST_CHECK ( util::Any() != 5 );

    util::Any other = 1.5;

    using std::swap;
    swap(any,other);
    BOOST_CHECK ( any.cast<double>() == 1.5 );

    std::stringstream ss;
    ss << other;
    int input = 0;
    BOOST_CHECK ( ss >> input );
    BOOST_CHECK ( input == 5 );

    any = derived_obj;
    BOOST_CHECK ( dynamic_cast<TestDerived*>(reinterpret_cast<object::Mirror*>(any.pointer())) );
    BOOST_CHECK ( any != any );
}


class SimpleClass {};
class VirtualClass { public: virtual ~VirtualClass() {} };
BOOST_AUTO_TEST_CASE( test_any_traits )
{
    using namespace util;

    Any_Traits traits_void_expected = { 1, 0, 0, 0, 0, 0 };
    BOOST_CHECK ( Any().type_traits() == Any_Traits() && Any_Traits() == traits_void_expected );

    int i = 1;
    Any_Traits traits_int = Any(i).type_traits();
    Any_Traits traits_int_expected = { 0, 0, 1, 0, 0, 0 };
    BOOST_CHECK ( traits_int == traits_int_expected );

    Any_Traits traits_intstar = Any(&i).type_traits();
    Any_Traits traits_intstar_expected = { 0, 1, 0, 0, 0, 0 };
    BOOST_CHECK ( traits_intstar == traits_intstar_expected );

    SimpleClass simple_object;
    Any_Traits traits_simple_object = Any(simple_object).type_traits();
    Any_Traits traits_simple_object_expected = { 0, 0, 0, 1, 0, 0 };
    BOOST_CHECK ( traits_simple_object == traits_simple_object_expected );

    Any_Traits traits_simple_pointer = Any(&simple_object).type_traits();
    Any_Traits traits_simple_pointer_expected = { 0, 1, 0, 1, 0, 0 };
    BOOST_CHECK ( traits_simple_pointer == traits_simple_pointer_expected );

    VirtualClass virtual_object;
    Any_Traits traits_virtual_object = Any(virtual_object).type_traits();
    Any_Traits traits_virtual_object_expected = { 0, 0, 0, 1, 0, 0 };
    BOOST_CHECK ( traits_virtual_object == traits_virtual_object_expected );

    Any_Traits traits_virtual_pointer = Any(&virtual_object).type_traits();
    Any_Traits traits_virtual_pointer_expected = { 0, 1, 0, 1, 1, 0 };
    BOOST_CHECK ( traits_virtual_pointer == traits_virtual_pointer_expected );

    Any_Traits traits_mirror_object = Any(base_obj).type_traits();
    Any_Traits traits_mirrore_object_expected = { 0, 0, 0, 1, 0, 1 };
    BOOST_CHECK ( traits_mirror_object == traits_mirrore_object_expected );

    Any_Traits traits_mirror_pointer = Any(&base_obj).type_traits();
    Any_Traits traits_mirror_pointer_expected = { 0, 1, 0, 1, 1, 1 };
    BOOST_CHECK ( traits_mirror_pointer == traits_mirror_pointer_expected );
}

BOOST_AUTO_TEST_CASE( test_type_id )
{
    BOOST_CHECK ( TestDerived::static_type_id() > TestClass::static_type_id() );
    BOOST_CHECK ( TestClass::static_type_id() != object::Mirror::static_type_id() );
    BOOST_CHECK ( TestClass::static_type_id() == base_obj.type_id() );
    BOOST_CHECK ( derived_obj.type_id() != base_obj.type_id() );
    BOOST_CHECK ( derived_obj.has_type_id(base_obj.type_id()) );
    BOOST_CHECK ( (&derived_obj)->cast<TestClass*>() == &derived_obj );
}

BOOST_AUTO_TEST_CASE( test_getters )
{
    BOOST_CHECK ( base_obj.get_any("unexisting").empty() );

    BOOST_CHECK ( base_obj.get_string("foo") == std::to_string(base_obj.foo) );
    BOOST_CHECK ( base_obj.get<int>("foo") == base_obj.foo );
    BOOST_CHECK ( base_obj.get<int>("f@@") == base_obj.foo );
    BOOST_CHECK ( base_obj.get<int>("f@@1") == base_obj.foo+1 );
    BOOST_CHECK ( base_obj.get<int>("foo2") == base_obj.foo+2 );
    BOOST_CHECK ( base_obj.get<int>("foo3") == base_obj.foo+3 );
    BOOST_CHECK ( base_obj.get<int>("bar") == 0 );

    BOOST_CHECK ( derived_obj.get<int>("foo") == derived_obj.foo );
    BOOST_CHECK ( derived_obj.get<int>("f@@1") == derived_obj.foo+1 );
    BOOST_CHECK ( derived_obj.get<int>("foo2") == derived_obj.foo+2 );
    BOOST_CHECK ( derived_obj.get<int>("foo3") == derived_obj.foo+3 );
    BOOST_CHECK ( derived_obj.get<int>("bar") == derived_obj.bar );

}

BOOST_AUTO_TEST_CASE( test_setters )
{
    base_obj.set("foo",0);
    BOOST_CHECK ( base_obj.foo == 0 );
    base_obj.set("f@@1",100);
    BOOST_CHECK ( base_obj.foo == 100 );
    base_obj.set("foo2",200);
    BOOST_CHECK ( base_obj.foo == 200 );
    base_obj.set("foo3",300);
    BOOST_CHECK ( base_obj.foo == 300 );
    base_obj.set("f@@",123);
    BOOST_CHECK ( base_obj.foo == 123 );

    derived_obj.set("bar",1234);
    BOOST_CHECK ( derived_obj.bar == 1234 );

    base_obj.set("unexisting", 5);
    BOOST_CHECK ( base_obj.get_any("unexisting").empty() );
}

BOOST_AUTO_TEST_CASE( test_methods )
{
    BOOST_CHECK( base_obj.call("meth@d1", {}).cast<int>() == base_obj.get_foo()+1 );
    BOOST_CHECK( base_obj.call("method2").cast<int>() == base_obj.get_foo()+2 );
    BOOST_CHECK( base_obj.call("get_foo", {}).cast<int>() == base_obj.get_foo() );
    BOOST_CHECK( base_obj.call("get_foo_any", {}).cast<int>() == base_obj.get_foo() );
    base_obj.call("void_method");
    BOOST_CHECK ( base_obj.foo == 10 );
    BOOST_CHECK( base_obj.call("foo_plus", {5}).cast<int>() == base_obj.get_foo()+5 );
    base_obj.call("set_foo", 6);
    BOOST_CHECK ( base_obj.foo == 6 );
    BOOST_CHECK( base_obj.call("cat", {"hello",123}).to_string() == "hello123" );
}

BOOST_AUTO_TEST_CASE( test_dynamic )
{
    TestDynamic dyn_obj;

    BOOST_CHECK ( dyn_obj.get<int>("member") == dyn_obj.member );
    BOOST_CHECK ( dyn_obj.get_any("not_member").empty() );

    dyn_obj.set("member",123);
    BOOST_CHECK ( dyn_obj.member == 123 );

    dyn_obj.set("not_member",456);
    BOOST_CHECK ( dyn_obj.get<int>("not_member") == 456 );

}
BOOST_AUTO_TEST_CASE( test_misc )
{
    BOOST_CHECK ( base_obj.class_name() == "TestClass" );
    TestDynamic dyn_obj;
    util::Any any = dyn_obj;
    BOOST_CHECK ( any.to_string() == "TestDynamic{member:1,}" );
    dyn_obj.set("foo",2);
    any = dyn_obj;
    BOOST_CHECK ( any.to_string() == "TestDynamic{foo:2,member:1,}" );
}
