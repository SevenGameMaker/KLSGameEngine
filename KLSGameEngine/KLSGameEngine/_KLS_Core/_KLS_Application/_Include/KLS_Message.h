// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// unclutter the global namespace
namespace KLS
{
    class KLS_Message
    {
    public:
        int Type;
        int Source;
        int Dest;
        double DeliveryTime;

    public:
        KLS_Message() {}
        virtual ~KLS_Message() {}

        // Copy Constructor
        KLS_Message(const KLS_Message& other)
        {
            // Copy each member variable from 'other' to 'this'
            this->Type = other.Type;
            this->Source = other.Source;
            this->Dest = other.Dest;
            this->DeliveryTime = other.DeliveryTime;
        }

        // Assignment Operator
        KLS_Message& operator=(const KLS_Message& other)
        {
            if (this != &other) // Check for self-assignment
            {
                // Assign each member variable from 'other' to 'this'
                this->Type = other.Type;
                this->Source = other.Source;
                this->Dest = other.Dest;
                this->DeliveryTime = other.DeliveryTime;
            }
            return *this;
        }
    };

} // end namespace
