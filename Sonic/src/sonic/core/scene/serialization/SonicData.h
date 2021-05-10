#pragma once

#include "sonic/util/DynamicArray.h"
#include "sonic/util/String.h"
#include "sonic/util/Map.h"

namespace sonic
{
    class SonicData
    {
        class Element
        {
            using Dictionary = Map<String, Element>;
            using List = DynamicArray<Element>;

            enum Type
            {
                DICTIONARY,
                LIST,
                STRING,
                NUMBER,
            };

            ~Element();

            bool isNumber() { return mType == Type::NUMBER; }
            bool isString() { return mType == Type::STRING; }
            bool isDictionary() { return mType == Type::DICTIONARY; }
            bool isList() { return mType == Type::LIST; }
            Type getType() { return mType; }

            template<typename T> T as() { return T(mString); }

            template<> Dictionary as<Dictionary>() { return mDictionary; }
            template<> List as<List>() { return mList; }
            template<> bool as<bool>() { return mInteger; }
            template<> char as<char>() { return mInteger; }
            template<> unsigned char as<unsigned char>() { return mInteger; }
            template<> short as<short>() { return mInteger; }
            template<> unsigned as<unsigned>() { return mInteger; }
            template<> int as<int>() { return mInteger; }
            template<> unsigned int as<unsigned int>() { return mInteger; }
            template<> long as<long>() { return mInteger; }
            template<> unsigned long as<unsigned long>() { return mInteger; }
            template<> long long as<long long>() { return mInteger; }
            template<> unsigned long long as<unsigned long long>() { return mInteger; }
            template<> float as<float>() { return mFloat; }
            template<> double as<double>() { return mFloat; }
            template<> long double as<long double>() { return mFloat; }

            Type mType;

            union
            {
                Dictionary mDictionary;
                List mList;
                String mString;

                struct
                {
                    long double mFloat;
                    unsigned long long mInteger;
                };
            };
        };

        SonicData(const String& filePath);

        DynamicArray<Element> elements;
        Map<String, SonicData> subSections;
    };

} // namespace sonic
