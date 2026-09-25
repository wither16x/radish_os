#pragma once

#include "Internal/Memory/Buffer.hpp"
#include "Internal/Memory/CString.hpp"
#include <Tarwi.hpp>
#include <TarwiGlobals.hpp>

#include <Internal/FileSystem/File.hpp>
#include <Internal/FileSystem/StdStreams.hpp>
#include <Internal/Exceptions/FileNotFound.hpp>
#include <Internal/Exceptions/NullStream.hpp>

#define RESOURCES_PATH   "Test/FileSystemPackage/Resources"

namespace Melon::Test
{
        TARWI_MODULE(FileModule)
        {
                TARWI_SET_NAME("FileModule");

                TARWI_UNIT(unitCheckRaii)
                {
                        FileSystem::File file(RESOURCES_PATH"/Empty.txt", "r");
                        TARWI_EXPECT(file.isOpen());
                }

                TARWI_UNIT(unitOpenClose)
                {
                        FileSystem::File file(RESOURCES_PATH"/Empty.txt", "r");
                        bool is_open_before_close = file.isOpen();
                        file.close();
                        bool is_open_after_close = file.isOpen();
                        TARWI_EXPECT(is_open_before_close and not is_open_after_close);
                }

                TARWI_UNIT(unitFileNotFound)
                {
                        bool caught = false;

                        try {
                                FileSystem::File file(RESOURCES_PATH"/Invisible.txt", "r");
                        } catch (const Exceptions::FileNotFound &e) {
                                TARWI_OUTPUT("caught exception: %s\n", e.what());
                                caught = true;
                        }

                        TARWI_EXPECT(caught);
                }

                TARWI_UNIT(unitNullStream)
                {
                        bool caught = false;

                        FILE *stream = nullptr;

                        try {
                                FileSystem::File file(stream);
                        } catch (const Exceptions::NullStream &e) {
                                TARWI_OUTPUT("caught exception: %s\n", e.what());
                                caught = true;
                        }

                        TARWI_EXPECT(caught);
                }

                TARWI_UNIT(unitReadLine)
                {
                        FileSystem::File file(RESOURCES_PATH"/Line.txt", "r");
                        auto buf = file.readLine(0);
                        TARWI_EXPECT(buf == "Hello! I am a file!");
                }

                TARWI_UNIT(unitReadBytes)
                {
                        FileSystem::File file(RESOURCES_PATH"/Message.txt", "r");
                        Memory::Buffer<char> content = file.readBytes(512);
                        TARWI_EXPECT(strcmp(content.get(),
                        "Hey!\n"
                        "\n"
                        "If you read this, know that...\n"
                        "Uh...\n"
                        "I don't know T-T\n") == 0
                        );
                }

                TARWI_UNIT(unitReadSeveralLines)
                {
                        FileSystem::File file(RESOURCES_PATH"/Lines.txt", "r");

                        auto line0 = file.readLine(0);
                        auto line4 = file.readLine(4);
                        auto line2 = file.readLine(2);
                        auto line1 = file.readLine(1);
                        auto line3 = file.readLine(3);

                        TARWI_EXPECT(
                                line0 == "First line." and
                                line1 == "Second line." and
                                line2 == "Third line." and
                                line3 == "Fourth line." and
                                line4 == "Fifth line."
                        );
                }

                TARWI_UNIT(unitWrite)
                {
                        FileSystem::File file(RESOURCES_PATH"/ToWrite.txt", "r+");

                        Memory::CString data = "I wrote this!";
                        file.write({data.get(), data.length()}, data.length());

                        TARWI_EXPECT(strcmp(file.readBytes(data.length()).get(), data.get()) == 0);
                }

                TARWI_UNIT(unitRead)
                {
                        FileSystem::File file(RESOURCES_PATH"/Message.txt", "r");
                        Memory::Buffer<char> data = file.read();
                        TARWI_EXPECT(strcmp(data.get(),
                        "Hey!\n"
                        "\n"
                        "If you read this, know that...\n"
                        "Uh...\n"
                        "I don't know T-T\n") == 0);
                }

                TARWI_MODULE_MAIN()
                {
                        TARWI_CALL_UNIT(unitCheckRaii);
                        TARWI_CALL_UNIT(unitOpenClose);
                        TARWI_CALL_UNIT(unitFileNotFound);
                        TARWI_CALL_UNIT(unitNullStream);
                        TARWI_CALL_UNIT(unitReadLine);
                        TARWI_CALL_UNIT(unitReadBytes);
                        TARWI_CALL_UNIT(unitReadSeveralLines);
                        TARWI_CALL_UNIT(unitWrite);
                        TARWI_CALL_UNIT(unitRead);
                }
        };
} // namespace Melon::Test