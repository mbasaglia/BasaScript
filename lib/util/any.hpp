/**
 \file

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
#ifndef UTIL_ANY_HPP
#define UTIL_ANY_HPP

#include <list>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/type_traits.hpp>

namespace object { class Mirror; }

namespace util {

namespace detail {
template<bool can_compare>
    struct Compare
    {
        template<class T>
            static bool compare ( const T&, const T& ) { return false; }
    };

template<>
    struct Compare<true>
    {
        template<class T>
            static bool compare ( const T& a, const T& b ) { return a == b; }
    };


template<bool can_stream>
    struct Stream
    {
        template<class T>
            static std::ostream& print ( std::ostream& stream, const T& ) { return stream; }
        /// \todo read() as well?
    };

template<>
    struct Stream<true>
    {
        template<class T>
            static std::ostream& print ( std::ostream& stream, const T&a )
            {
                stream << a;
                return stream;
            }
    };

} // namespace (util::)detail

/**
 * \brief Contains traits of the type stored by an Any object
 */
struct Any_Traits
{
    bool is_void       :1;  ///< Only true for empty Any objects
    bool is_pointer    :1; ///< Whether it's a pointer to something
    bool is_arithmetic :1; ///< int, float and the like
    bool is_class      :1; ///< Whether it's an object or a pointer to one
    bool is_polymorphic:1; ///< Whether is a pointer to a polymorphic class
    bool is_reflective :1; ///< Object inheriting object::Mirror

    Any_Traits() :
        is_void(1), is_pointer(0), is_arithmetic(0), is_class(0),
        is_polymorphic(0), is_reflective(0)
    {}
    Any_Traits(
        bool is_void,
        bool is_pointer,
        bool is_arithmetic,
        bool is_class,
        bool is_polymorphic,
        bool is_reflective
    ) :
        is_void(is_void),
        is_pointer(is_pointer),
        is_arithmetic(is_arithmetic),
        is_class(is_class),
        is_polymorphic(is_polymorphic),
        is_reflective(is_reflective)
    {}

    bool operator== (const Any_Traits& other) const
    {
        return
            is_void == other.is_void &&
            is_pointer == other.is_pointer &&
            is_arithmetic == other.is_arithmetic &&
            is_class == other.is_class &&
            is_polymorphic == other.is_polymorphic &&
            is_reflective == other.is_reflective;
    }

    bool operator!= (const Any_Traits& other) const
    {
        return ! (*this == other);
    }
};

/**
 * \brief Can contain any type that is DefaultConstructible and CopyConstructible
 */
class Any
{
public:
    /**
     * \brief Initialize an empty value
     */
    Any() : content(nullptr) {}

    /**
     * \brief Initialize from any value
     */
    template<typename T>
        Any(const T & value)
          : content(new Any_Holder<typename std::decay<const T>::type>(value)) {}

    Any(const Any & other)
        : content(other.content ? other.content->clone() : 0) {}

    Any(Any&& other)
        : content(other.content)
    {
        other.content = nullptr;
    }

    ~Any()
    {
        delete content;
    }

    Any& operator=(const Any& other);

    Any& operator=(Any&& other);

    /**
     * \brief Efficient swap;
     */
    Any& swap(Any& other);

    /**
     * \brief Whether this Any doesn't contain anything
     */
    bool empty() const
    {
        return !content;
    }

    /**
     * \brief Clears the value
     */
    void clear()
    {
        delete content;
        content = nullptr;
    }

    /**
     * \brief Returns the type info of the contained type (or void if empty)
     */
    const std::type_info & type_info() const
    {
        return content ? content->type_info() : typeid(void);
    }

    /**
     * \brief Returns the traits of the contained type
     */
    Any_Traits type_traits() const
    {
        return content ? content->type_traits() : Any_Traits();
    }

    /**
     * \brief Cast the value according to a lexical interpretation
     * \pre the contained type to have operator<<
     * and \c T to be DefaultConstructible and have operator>>
     */
    template<class T>
        typename std::decay<T>::type lexical_cast() const
        {
            typedef typename std::decay<T>::type Target;
            if ( !content )
                return Target();
            std::stringstream ss;
            content->stream_output(ss);
            Target temp;
            ss >> temp;
            return temp;
        }
    /**
     * \brief Cast to the contained type
     * \pre T CopyConstructible
     * \return The contained value or T() if it doesn't match the contained type
     */
    template<class T>
        typename std::decay<T>::type cast() const
        {
            typedef typename std::decay<T>::type Target;
            Target* pointer = cast_pointer<Target>();
            return pointer ? *pointer : Target();
        }

    /**
     * \brief Cast to the contained type
     * \return The contained value
     * \throws domain_error if the types mismatch
     */
    template<class T>
        typename std::decay<T>::type cast_throw() const
        {
            typedef typename std::decay<T>::type Target;
            Target* pointer = cast_pointer<Target>();
            if (pointer)
                return *pointer;
            throw std::domain_error(std::string("Bad Any cast from ")
                +type_info().name()+" to "+typeid(Target).name());
        }
    /**
     * \brief Get pointer to the contained data
     */
    const void* pointer() const
    {
        return content ? content->pointer() : nullptr;
    }
    void* pointer()
    {
        return content ? content->pointer() : nullptr;
    }

    /**
     * \brief Converts the contained value to a string
     * \pre The contained type has operator<<
     */
    std::string to_string() const;

    bool operator== (const Any& other) const;
    bool operator!= (const Any& other) const;

    friend std::ostream& operator<< ( std::ostream& stream, const Any& any);

private:

    /**
     * \brief Virtual base for the value holders
     */
    class Any_Base
    {
    public:
        Any_Base() = default;
        Any_Base (const Any_Base&) = delete;
        Any_Base& operator= (const Any_Base&) = delete;
        virtual ~Any_Base() {}

        virtual const std::type_info& type_info() const = 0;
        virtual Any_Base* clone() const = 0;
        virtual std::ostream& stream_output(std::ostream& stream) const = 0;
        virtual Any_Traits type_traits() const = 0;
        virtual const void* pointer() const = 0;
        virtual void* pointer() = 0;
        virtual bool compare(const Any_Base* other) const = 0;
    };


    /**
     * \brief Template value holder
     * \tparam T Contained type
     * \pre T CopyConstructible
     */
    template<typename T>
        class Any_Holder : public Any_Base
        {
        public:
            typedef T value_type;

            Any_Holder (const value_type& value) : value(value) {}

            const std::type_info& type_info() const override
            {
                return typeid(T);
            }

            Any_Base* clone() const override
            {
                return new Any_Holder(value);
            }

            std::ostream& stream_output(std::ostream& stream) const override
            {
                return detail::Stream<boost::has_left_shift<std::ostream&,T>::value>::print(stream,value);
            }

            Any_Traits type_traits() const override
            {
                typedef typename std::remove_pointer<T>::type referenced_type;
                return {
                    false, // is_void
                    std::is_pointer<T>::value,
                    std::is_arithmetic<T>::value,
                    std::is_class<referenced_type>::value,
                    std::is_pointer<T>::value && std::has_virtual_destructor<referenced_type>::value,
                    std::is_base_of<object::Mirror,referenced_type>::value
                };
            }

            const void* pointer() const override
            {
                return &value;
            }

            void* pointer() override
            {
                return &value;
            }

            bool compare(const Any_Base* other) const override
            {
                if ( type_info() != other->type_info() )
                    return false;
                const Any_Holder* other_sameclass = static_cast<const Any_Holder*>(other);
                return detail::Compare<boost::has_equal_to<T>::value>::compare(value,other_sameclass->value);
            }

            value_type value;
        };

    /**
     * \brief Returns a pointer to T* if it matches the contained type
     */
    template<class T>
        T* cast_pointer () const
        {
            return type_info() == typeid(T) ?
                &static_cast<Any_Holder<T>*>(content)->value :
                nullptr;
        }

    Any_Base* content;
};

/**
 * \brief Makes Any Swappable
 */
inline void swap ( Any& a, Any& b )
{
    a.swap(b);
}

/**
 * \brief Prints the contained value to stream
 * \pre The contained type has operator<<
 */
std::ostream& operator<< ( std::ostream& stream, const Any& any);

namespace detail {
inline std::list<util::Any> pack_to_vector_impl() { return {}; }

template<class Head, class... Tail>
    std::list<util::Any> pack_to_vector_impl(const Head& head, Tail... tail)
    {
        auto list = pack_to_vector_impl(tail...);
        list.push_front(head);
        return list;
    }
} // namespace detail

template<class... Params>
    std::vector<util::Any> pack_to_vector(Params... params)
    {
        auto list = detail::pack_to_vector_impl(params...);
        return std::vector<util::Any>(list.begin(),list.end());
    }

} // namespace util
#endif // UTIL_ANY_HPP
