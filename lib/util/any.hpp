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

#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <type_traits>
#include <utility>

namespace util {

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
     * \brief Converts the contained value to a string
     * \pre The contained type has operator<<
     */
    std::string to_string() const;

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

            virtual Any_Base* clone() const override
            {
                return new Any_Holder(value);
            }

            virtual std::ostream& stream_output(std::ostream& stream) const override
            {
                return stream << value;
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

} // namespace util
#endif // UTIL_ANY_HPP
