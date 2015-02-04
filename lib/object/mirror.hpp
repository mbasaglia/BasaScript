/**
 \ file                    *

 \author Mattia Basaglia

 \section License

 Copyright (C)  Mattia Basaglia

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
#ifndef OBJECT_MIRROR_HPP
#define OBJECT_MIRROR_HPP

#include <functional>
#include <map>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../util/any.hpp"

namespace object {

/**
 * \brief Provides reflection
 */
class Mirror {
public:
    typedef std::map<std::string, util::Any> Properties;
    typedef std::vector<util::Any> Arguments;

protected:
    class Magic_Mirror
    {
    public:
        static Magic_Mirror& instance();

        virtual bool can_get( const std::string& property) const;
        virtual util::Any get(const Mirror& obj, const std::string& property) const;
        virtual void get_all(const Mirror& obj, Properties& out, const std::string& prefix) const;

        virtual bool can_set( const std::string& property) const;
        virtual void set(Mirror& obj, const std::string& property, const util::Any& value) const;

        virtual bool can_call(const std::string& name) const;
        virtual util::Any call(Mirror& obj, const std::string& name, const Arguments& args) const;

    protected:
        Magic_Mirror() {}
        Magic_Mirror(const Magic_Mirror &) = delete;
        Magic_Mirror& operator= (const Magic_Mirror&) = delete;
    };

public:

    /**
     * \brief Returns the string value for the given property
     */
    std::string get_string (const std::string& name) const;

    /**
     * \brief Returns the util::Any value for the given property
     */
    util::Any get_any (const std::string& name) const;

    /**
     * \brief Returns a property as a custom type
     */
    template<class T>
        T get (const std::string& name) const
        {
            util::Any any = get_any(name);
            return any.empty() ? T() : any.cast<T>();
        }

    void set(const std::string& name, const util::Any& value);

    /**
     * \brief Whether the property can be read
     */
    bool can_get(const std::string& name) const;

    /**
     * \brief Whether the property can be written
     */
    bool can_set(const std::string& name) const;

    /**
     * \brief Gets all properties as a map
     * \param prefix Prefix top prepend to property names
     */
    Properties properties(const std::string& prefix = "") const;

    /**
     * \brief Gets all properties as a map
     * \param[out] out Property container to be filled
     * \param   prefix Prefix top prepend to property names
     */
    void properties(Properties& out, const std::string& prefix = "") const;

    /**
     * \brief Whether the object has a callable method with the given name
     */
    bool can_call(const std::string& name) const;

    /**
     * \brief Calls a method by name
     * \param name  Method name
     * \param args  Arguments
     * \return What the method returned
     */
    util::Any call(const std::string& name, const Arguments& args = Arguments());

    /**
     * \brief Calls a method by name
     * \param name  Method name
     * \param args  Arguments
     * \return What the method returned
     */
    template<class... Types>
    util::Any call(const std::string& name, Types... args)
    {
        return call(name, util::pack_to_vector(args...));
    }


    /**
     * \brief Cast to a different class of the Object hierarchy
     * \tparam T Pointer type
     * \return A casted object or nullptr if types mismatch
     */
    template<class T>
        T cast()
        {
            if ( !this || !has_type_id(std::remove_pointer<T>::type::static_type_id()) )
                return nullptr;
            return reinterpret_cast<T>(this);
        }

    /**
     * \brief Type identifier
     * \note Use the value returned by register_object_type()
     */
    virtual int type_id() const = 0;

    /**
     * \brief Get whether the object has the given type id in the hierarchy above it
     * \param id    Type id
     */
    virtual int has_type_id(int id) const;

    /**
     * \brief Get object class type id, define the same method in derived classes (using REGISTER_OBJECT)
     */
    static int static_type_id();

protected:
    /**
     * \brief Method used internally to get the dynamic magic object
     */
    virtual const Magic_Mirror& get_magic() const = 0;

    /**
     * \brief Override if you want some extra properties
     */
    virtual util::Any get_extra(const std::string& name) const;
    /**
     * \brief Override if you want some extra properties
     */
    virtual bool can_get_extra(const std::string& name) const;

    /**
     * \brief Override if you want some extra writable properties
     */
    virtual void set_extra(const std::string& name, const util::Any& value);

    /**
     * \brief Override if you want some extra writable properties
     */
    virtual bool can_set_extra(const std::string& name) const;

    /**
     * \brief Override if you want some extra readable properties
     * \param[out] out  Output property map
     * \param   prefix  Prefix to prepend to property names
     */
    virtual void get_all_extra(Properties& out, const std::string& prefix = "") const;

    /**
     * \brief Override if you want some extra callable methods
     */
    virtual util::Any call_extra(const std::string& name, const Arguments&args);
    /**
     * \brief Override if you want some extra callable methods
     */
    virtual bool can_call_extra(const std::string& name) const;

    /**
     * \brief Returns a unique type id
     */
    static int new_type_id();
};

namespace detail {

/**
 * \brief Easier to get the address of this
 */
template<class T>
inline typename std::decay<T>::type any_cast(const util::Any& a) { return a.cast<T>(); }

/**
 * \brief Does nothing but move type information around
 */
template<class Functor, class Return, class Param1, class Param2, class... OtherParams>
Param2 second_argument_return(Return (Functor::*)(Param1, Param2, OtherParams...) const);

template<class Functor, class Return, class Param1, class... OtherParams>
Param1 first_argument_return(Return (Functor::*)(Param1, OtherParams...));

/**
 * \brief Get the second argument of a function object
 */
template<class Functor>
struct second_argument
{
    typedef decltype( second_argument_return(&Functor::operator()) ) type;
};

/**
 * \brief Get the second argument of a function pointer
 */
template<class Class, class... Params>
struct second_argument<void (Class::*)(Params...)>
{
private:
    typedef void (Class::*fptr)(Params...);
public:
    typedef decltype(first_argument_return(fptr())) type;
};


} // namespace detail



/**
 * \brief Enable property magic
 * \param c     Class
 * \param base  Base class, must have Mirror as ancestor
 */
#define MIRROR(c,base) \
    public: \
    struct Magic_Mirror : base::Magic_Mirror { \
    typedef std::function<util::Any (const c&)>             Getter; \
    typedef std::function<void (c&, const util::Any&)>      Setter; \
    typedef std::function<util::Any (c&, const Arguments&)> Caller; \
    std::unordered_map<std::string,Getter> getters; \
    std::unordered_map<std::string,Setter> setters; \
    std::unordered_map<std::string,Caller> methods; \
    static base::Magic_Mirror& base_instance() { return base::Magic_Mirror::instance(); } \
    util::Any get(const ::object::Mirror& obj, const std::string& property) const override { \
        auto it = getters.find(property); \
        return it != getters.end() ? it->second(reinterpret_cast<const c&>(obj)) : base_instance().get(obj,property); } \
    bool can_get(const std::string& property) const override { \
        return getters.find(property) != getters.end() || base_instance().can_get(property); } \
    void set(::object::Mirror& obj, const std::string& property, const util::Any& value) const override {\
        auto it = setters.find(property); \
        return it != setters.end() ? it->second(reinterpret_cast<c&>(obj),value) : \
        base_instance().set(obj,property,value);  }\
    bool can_set(const std::string& property) const override { \
        return setters.find(property) != setters.end() || base_instance().can_set(property); } \
    void get_all(const ::object::Mirror& obj, Properties& out, const std::string& prefix) const override { \
        base_instance().get_all(obj,out,prefix); \
        for ( const auto&p : getters ) \
            out[prefix+p.first] = p.second(reinterpret_cast<const c&>(obj)); } \
    bool can_call(const std::string& name) const override { \
        return methods.find(name) != methods.end() || base_instance().can_call(name); } \
    util::Any call(::object::Mirror& obj, const std::string& name,const Arguments& args) const override {\
        auto it = methods.find(name); \
        return it != methods.end() ? it->second(reinterpret_cast<c&>(obj),args) : base_instance().call(obj,name,args);  }\
    static Magic_Mirror& instance() { static Magic_Mirror s; return s; } \
    protected: \
        Magic_Mirror() { c::static_type_id(); } \
        Magic_Mirror(const Magic_Mirror&) = delete; \
        Magic_Mirror& operator= (const Magic_Mirror&) = delete; \
    }; /*Magic_Mirror*/ \
    const ::object::Mirror::Magic_Mirror& get_magic() const override { \
        return Magic_Mirror::instance(); } \
    static Magic_Mirror& get_static_magic() { return Magic_Mirror::instance(); } \
    static int static_type_id() { \
        static int id = ::object::Mirror::new_type_id(); \
        return id; } \
    int type_id() const override { return static_type_id(); } \
    int has_type_id(int id) const override { return id == static_type_id() || base::has_type_id(id); } \
    private:

/**
 * \brief Declare a property read using a functor
 * \param c         Class
 * \param name      Property name, as accessed via strings
 * \param symbol    Property name as a C++ symbol
 * \param functor   Functor returning a Value
 */
#define DECLARE_PROPERTY_GET_ADVANCED(c,name,symbol,functor) \
    namespace detail { \
    struct Magic_Mirror_GET_##c##_##symbol { \
        Magic_Mirror_GET_##c##_##symbol() { \
        c::get_static_magic().getters[name] = functor; }\
    }; static Magic_Mirror_GET_##c##_##symbol Object_Magic_Mirror_GET_##c##_##symbol; }

/**
 * \brief Declare a property read using a functor
 * \param c         Class
 * \param name      Property name as a C++ symbol
 * \param functor   Functor returning a Value
 */
#define DECLARE_PROPERTY_GET_FUNCTOR(c,name,functor) \
        DECLARE_PROPERTY_GET_ADVANCED(c,#name,name,functor)

/**
 * \brief Declare a property read using a getter method
 * \param c         Class
 * \param name      Property name as a C++ symbol
 * \param method    Name of the method used to access the property
 * \note \c c::method must return a type which overload the stream operator <<
 */
#define DECLARE_PROPERTY_GETTER(c,name,method) \
    DECLARE_PROPERTY_GET_FUNCTOR(c,name, \
        [](const c& obj){ return util::Any(obj.method()); } )

/**
 * \brief Declare a property written using a functor
 * \param c         Class
 * \param name      Property name, as accessed via strings
 * \param symbol    Property name as a C++ symbol
 * \param functor   Functor taking a reference to a Mirror and a value
 */
#define DECLARE_PROPERTY_SET_ADVANCED(c,name,symbol,functor) \
    namespace detail { \
    struct Magic_Mirror_SET_##c##_##symbol { \
        Magic_Mirror_SET_##c##_##symbol() { \
            auto lambda = functor; \
            typedef object::detail::second_argument<decltype(lambda)>::type Argument; \
            using namespace std::placeholders; \
            c::get_static_magic().setters[name] = \
                std::bind(lambda, _1, std::bind(object::detail::any_cast<Argument>,_2)); \
        } \
    }; static Magic_Mirror_SET_##c##_##symbol Object_Magic_Mirror_SET_##c##_##symbol; }

/**
 * \brief Declare a property written using a functor
 * \param c         Class
 * \param name      Property name as a C++ symbol
 * \param functor   Functor taking a reference to a Mirror and a value
 */
#define DECLARE_PROPERTY_SET_FUNCTOR(c,name,functor) \
    DECLARE_PROPERTY_SET_ADVANCED(c,#name,name,functor)

/**
 * \brief Declare a property written using a setter method
 * \param c         Class
 * \param name      Property name as a C++ symbol
 * \param method    Name of the method used to access the property
 * \note \c c::method must take a type which overload the stream operator >> and return void
 */
#define DECLARE_PROPERTY_SETTER(c,name,method) \
    DECLARE_PROPERTY_SET_FUNCTOR(c,name, \
        [](c& obj, ::object::detail::second_argument<decltype(&c::method)>::type value) { obj.method(value); }; )

/**
 * \brief Declare a property read and written using accessor methods
 * \param c             Class
 * \param name          Property name as a C++ symbol
 * \param getter        Getter method
 * \param setter        Setter method
 */
#define DECLARE_PROPERTY(c,name,getter,setter) DECLARE_PROPERTY_GETTER(c,name,getter) DECLARE_PROPERTY_SETTER(c,name,setter)

/**
 * \brief Declare a property read and written using accessor methods
 * \param c         Class
 * \param name      String used to access the property
 * \param symbol    Property name as a C++ identifier
 * \param getter    Getter method
 * \param setter    Setter method
 */
#define DECLARE_PROPERTY_RENAME(c,name,symbol,getter,setter) \
    DECLARE_PROPERTY_GET_ADVANCED(c,name,symbol, [](const c& obj){ return util::Any(obj.getter()); } ) \
    DECLARE_PROPERTY_SET_ADVANCED(c,name,symbol, \
        [](c& obj, ::object::detail::second_argument<decltype(&c::setter)>::type value) { obj.setter(value); }; )

/**
 * \brief Declare a property accessible directly with an attribute
 * \param c     Class
 * \param name  property/attribute name
 */
#define DECLARE_PROPERTY_ATTRIBUTE_READONLY(c,name) \
    DECLARE_PROPERTY_GET_FUNCTOR(c,name,[](const c& obj){ return util::Any(obj.name); } )

/**
 * \brief Declare a property accessible directly with an attribute
 * \param c     Class
 * \param name  property/attribute name
 */
#define DECLARE_PROPERTY_ATTRIBUTE(c,name)  \
    DECLARE_PROPERTY_ATTRIBUTE_READONLY(c,name) \
    DECLARE_PROPERTY_SET_FUNCTOR(c,name, \
    [](c&obj,const decltype(c::name) &value) { obj.name = value; })

/**
 * \brief Define a method
 * \param c         Class
 * \param name      Method name as string
 * \param symbol    Method name as C++ identifier
 * \param functor   Functor (c& obj, const ::object::Mirror::Arguments& args) -> misc::Value
 */
#define DECLARE_METHOD_ADVANCED(c,name,symbol,functor) \
    namespace detail { \
    struct Magic_Mirror_CALL_##c##_##symbol { \
        Magic_Mirror_CALL_##c##_##symbol() { \
            c::get_static_magic().methods[name] = functor; }\
    }; static Magic_Mirror_CALL_##c##_##symbol Object_Magic_Mirror_CALL_##c##_##symbol; }

/**
 * \brief Define a method
 * \param c       Class
 * \param name    Method name as C++ identifier
 * \param functor Functor
 */
#define DECLARE_METHOD_FUNCTOR(c,name,functor) \
    DECLARE_METHOD_ADVANCED(c,#name,name,functor)

namespace detail {
/**
 * \brief Helper to convert pointers to member function to the correct
 * std::function to be stored in the Magic_Mirror
 */
template<class Class>
    class Method_To_Functor
    {
    public:
        typedef Mirror::Arguments Arguments;
        typedef std::function<util::Any (Class&, const Arguments&)> Caller;

        // X foo() [const]
        template<class Return>
            static Caller convert(Return (Class::*func)())
            {
                return [func](Class& obj, const Arguments&)
                    { return util::Any((obj.*func)()); };
            }
        template<class Return>
            static Caller convert(Return (Class::*func)() const)
            {
                return [func](Class& obj, const Arguments&)
                    { return util::Any((obj.*func)()); };
            }

        // void foo() [const]
        static Caller convert(void (Class::*func)() const )
        {
            return [func](Class& obj, const Arguments&)
            { (obj.*func)(); return util::Any(); };
        }
        static Caller convert(void (Class::*func)())
        {
            return [func](Class& obj, const Arguments&)
            { (obj.*func)(); return util::Any(); };
        }

        // X foo(Y) [const]
        template<class Return, class Arg>
            static Caller convert(Return (Class::*func)(Arg) const )
        {
            typedef typename std::decay<Arg>::type ArgValue;
            return [func](Class& obj, const Arguments& arg) {
                return util::Any((obj.*func)(arg.empty() ? ArgValue() : arg[0].cast<ArgValue>()));
            };
        }
        template<class Return, class Arg>
            static Caller convert(Return (Class::*func)(Arg) )
        {
            typedef typename std::decay<Arg>::type ArgValue;
            return [func](Class& obj, const Arguments& arg) {
                return util::Any((obj.*func)(arg.empty() ? ArgValue() : arg[0].cast<ArgValue>()));
            };
        }

        // void foo(Y) [const]
        template<class Arg>
            static Caller convert(void (Class::*func)(Arg) const )
        {
            typedef typename std::decay<Arg>::type ArgValue;
            return [func](Class& obj, const Arguments& arg) {
                (obj.*func)(arg.empty() ? ArgValue() : arg[0].cast<ArgValue>());
                return util::Any();
            };
        }
        template<class Arg>
            static Caller convert(void (Class::*func)(Arg) )
        {
            typedef typename std::decay<Arg>::type ArgValue;
            return [func](Class& obj, const Arguments& arg) {
                (obj.*func)(arg.empty() ? ArgValue() : arg[0].cast<ArgValue>());
                return util::Any();
            };
        }

        // X foo(const Arguments&) [const]
        template<class Return>
            static Caller convert(Return (Class::*func)(const Arguments&))
            {
                return [func](Class& obj, const Arguments& args)
                    { return util::Any((obj.*func)(args)); };
            }
        template<class Return>
            static Caller convert(Return (Class::*func)(const Arguments&) const)
            {
                return [func](Class& obj, const Arguments& args)
                    { return util::Any((obj.*func)(args)); };
            }

        // void foo(const Arguments&) [const]
        static Caller convert(void (Class::*func)(const Arguments&) const )
        {
            return [func](Class& obj, const Arguments& args)
            { (obj.*func)(args); return util::Any(); };
        }
        static Caller convert(void (Class::*func)(const Arguments&))
        {
            return [func](Class& obj, const Arguments& args)
            { (obj.*func)(args); return util::Any(); };
        }

    };
} // namespace detail
/**
 * \brief Define a method
 * \param c       Class
 * \param method  Method name (must take const ::object::Mirror::Arguments& args)
 */
#define DECLARE_METHOD(c,method) \
    DECLARE_METHOD_FUNCTOR(c,method, ::object::detail::Method_To_Functor<c>::convert(&c::method))

} // namespace object
#endif // OBJECT_MIRROR_HPP
