#pragma once

#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <Vector.hpp>
#include <Typing.hpp>

namespace Melon::Test
{
        TARWI_MODULE(VectorModule)
        {
                TARWI_SET_NAME("VectorModule");

                TARWI_UNIT(unitCheckRaii)
                {
                        Vector::Vector<Typing::Uint8> vec;
                        
                        TARWI_EXPECT(
                                vec.length() == 0
                                and vec.capacity() == 1
                        );
                }

                TARWI_UNIT(unitVectorOfInts)
                {
                        Vector::Vector<int> vec({1, 2, 3, 4, 5});

                        TARWI_EXPECT(
                                vec[0] == 1 and vec[1] == 2 and vec[2] == 3 and vec[3] == 4
                                and vec[4] == 5
                        );
                }

                TARWI_UNIT(unitPushBack)
                {
                        Vector::Vector<int> vec({250, 500, 750, 1000});

                        vec.pushBack(1250);
                        vec.pushBack(1500);
                        vec.pushBack(1750);
                        vec.pushBack(2000);

                        TARWI_EXPECT(
                                vec[0] == 250 and vec[1] == 500 and vec[2] == 750
                                and vec[3] == 1000 and vec[4] == 1250 and vec[5] == 1500
                                and vec[6] == 1750 and vec[7] == 2000
                        );
                }

                TARWI_UNIT(unitIsEmpty)
                {
                        Vector::Vector<int> vec_with_elems({2, 4, 6, 8, 10, 12});
                        Vector::Vector<int> empty_vec;

                        TARWI_EXPECT(
                                not vec_with_elems.isEmpty()
                                and empty_vec.isEmpty()
                        );
                }

                TARWI_UNIT(unitEmplaceBack)
                {
                        struct Person
                        {
                                const char *name;
                                unsigned int age;

                                Person() = default;

                                Person(const char *name, unsigned int age)
                                        : name(name), age(age)
                                {}
                        };

                        Vector::Vector<Person> persons;
                        persons.emplaceBack("John", 42);
                        persons.emplaceBack("Tom", 24);
                        persons.emplaceBack("Harry", 11);
                        persons.emplaceBack("Lea", 35);
                        persons.emplaceBack("Martine", 76);

                        TARWI_EXPECT(
                                strcmp(persons[0].name, "John") == 0 and persons[0].age == 42 and
                                strcmp(persons[1].name, "Tom") == 0 and persons[1].age == 24 and
                                strcmp(persons[2].name, "Harry") == 0 and persons[2].age == 11 and
                                strcmp(persons[3].name, "Lea") == 0 and persons[3].age == 35 and
                                strcmp(persons[4].name, "Martine") == 0 and persons[4].age == 76
                        );
                }

                TARWI_UNIT(unitPopBack)
                {
                        Vector::Vector<float> vec({0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 2.5f});
                        float f5 = vec.popBack();
                        float f4 = vec.popBack();
                        float f3 = vec.popBack();

                        TARWI_EXPECT(
                                f5 == 2.5f and f4 == 2.0f and f3 == 1.5f
                                and vec.length() == 3
                        );
                }

                TARWI_UNIT(unitClear)
                {
                        Vector::Vector<int> vec({12, 24, 48, 96});
                        vec.clear();

                        TARWI_EXPECT(vec.isEmpty());
                }

                TARWI_UNIT(unitAssign)
                {
                        Vector::Vector<int> vec = {{0, 1, 2}};
                        Vector::Vector<int> vec2 = vec;
                        TARWI_EXPECT(vec2 == vec);
                }

                TARWI_UNIT(unitErase)
                {
                        Vector::Vector<int> vec = {{42, 58, 72}};
                        vec.erase(1); // erase 58
                        TARWI_EXPECT(vec[0] == 42 and vec[1] == 72);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitCheckRaii);
                        TARWI_CALL_UNIT(unitVectorOfInts);
                        TARWI_CALL_UNIT(unitPushBack);
                        TARWI_CALL_UNIT(unitIsEmpty);
                        TARWI_CALL_UNIT(unitEmplaceBack);
                        TARWI_CALL_UNIT(unitPopBack);
                        TARWI_CALL_UNIT(unitClear);
                        TARWI_CALL_UNIT(unitAssign);
                        TARWI_CALL_UNIT(unitErase);
                }
        };
} // namespace Melon::Test