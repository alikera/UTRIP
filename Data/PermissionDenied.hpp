#ifndef PERMISSIONDENIED_HPP
#define PERMISSIONDENIED_HPP
#include <exception>
#define PERMISSION_DENIED "Permission Denied\n"

class PermissionDenied : public std::exception
{
    const char* what() const throw()
    {
        return PERMISSION_DENIED;
    }
};

#endif