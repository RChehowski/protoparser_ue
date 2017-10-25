
#include <google/protobuf/compiler/parser.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <sstream>
#include <iostream>
#include <unordered_map>

using google::protobuf::compiler::Parser;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::FileInputStream;
using google::protobuf::FileDescriptorProto;
using google::protobuf::io::Tokenizer;
using google::protobuf::io::ErrorCollector;
using google::protobuf::DescriptorProto;
using google::protobuf::FieldDescriptorProto;
using google::protobuf::FieldDescriptorProto_Type;
using google::protobuf::RepeatedPtrField;
using google::protobuf::ServiceDescriptorProto;
using google::protobuf::MethodDescriptorProto;

using namespace google::protobuf;
using namespace google::protobuf::io;

struct VizorErrorCollector : public ErrorCollector
{
    std::vector<std::tuple<int, int, std::string>> Warnings;
    std::vector<std::tuple<int, int, std::string>> Errors;

    void AddError(int line, int column, const std::string& message) override
    {
        Errors.push_back(std::make_tuple(line, column, message));
    }

    void AddWarning(int line, int column, const std::string& message) override
    {
        Warnings.push_back(std::make_tuple(line, column, message));
    }

    inline const std::vector<std::tuple<int, int, string>>& GetWarnings() const { return Warnings; }
    inline const std::vector<std::tuple<int, int, string>>& GetErrors() const { return Errors; }
};


class CppTranslator
{
    inline bool IsTypeAtomic(FieldDescriptorProto_Type type) const
    {
        auto it = FieldDescType_to_CppType.find((std::uint8_t)type);
        return it != FieldDescType_to_CppType.end();
    }

public:
    CppTranslator()
    {
        FieldDescType_to_CppType = {
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_DOUBLE, "double" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_FLOAT, "float" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_INT64, "int64" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_UINT64, "uint64" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_INT32, "int32" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_FIXED64, "uint64" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_FIXED32, "uint32" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_BOOL, "bool" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_STRING, "FString" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_UINT32, "uint32" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_SFIXED32, "int32" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_SFIXED64, "int64" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_SINT32, "int32" },
            { (std::uint8_t)FieldDescriptorProto_Type_TYPE_SINT64, "int64" },
//        std::make_pair(FieldDescriptorProto_Type_TYPE_GROUP, ),
//        std::make_pair(FieldDescriptorProto_Type_TYPE_MESSAGE, ),
//        std::make_pair(FieldDescriptorProto_Type_TYPE_BYTES, ),
//        std::make_pair(FieldDescriptorProto_Type_TYPE_ENUM, ),
        };
    }

    /**
     * Retrieves c cpp pseudonym for
     * @param fieldDesc
     * @param cppTypeName
     * @return
     */
    bool GetCppStringType(const FieldDescriptorProto& fieldDesc, std::string& cppTypeName) const
    {
        if (fieldDesc.has_type() && IsTypeAtomic(fieldDesc.type()))
        {
            cppTypeName = FieldDescType_to_CppType.at((std::uint8_t)fieldDesc.type());
            return true;
        }

        // TODO: Compile type
        return false;
    }



    void ReadFile(const std::string& path)
    {
        FILE* const fileDesc = fopen(path.c_str(), "rb");
        assert(fileDesc);

        std::unique_ptr<ZeroCopyInputStream> input(new FileInputStream(fileno(fileDesc)));
        std::unique_ptr<FileDescriptorProto> fileDescriptorProto(new FileDescriptorProto());
        std::unique_ptr<ErrorCollector> errorCollector(new VizorErrorCollector());

        std::unique_ptr<Parser> parser(new Parser());
        bool parseResult = parser->Parse(new Tokenizer(input.get(), errorCollector.get()), fileDescriptorProto.get());


        const google::protobuf::RepeatedPtrField<ServiceDescriptorProto>& services = fileDescriptorProto->service();
        for (const ServiceDescriptorProto& serviceDescriptor : services)
        {
            const google::protobuf::RepeatedPtrField<MethodDescriptorProto>& methodDescriptors = serviceDescriptor.method();
            for (const MethodDescriptorProto& MethodDesc : methodDescriptors)
            {
                std::cout << MethodDesc.DebugString() << std::endl;
            }
        }

        const google::protobuf::RepeatedPtrField<DescriptorProto>& messageTypes = fileDescriptorProto->message_type();
        for (const DescriptorProto& descProto : messageTypes)
        {
            const google::protobuf::RepeatedPtrField<FieldDescriptorProto>& fieldDescriptors = descProto.field();
            for (const FieldDescriptorProto& fieldDesc : fieldDescriptors)
            {
                std::cout << fieldDesc.DebugString() << std::endl;
            }
        }

        fclose(fileDesc);
    }

private:
    std::unordered_map<std::uint8_t, std::string> FieldDescType_to_CppType;
};


int main()
{

//    std::shared_ptr<CppTranslator> service(new CppTranslator());
//    service->ReadFile("/Users/netherwire/Desktop/auth.proto");

    system("cd /Users/netherwire/Projects/grpc/third_party/protobuf && ./autogen.sh");
    system("cd /Users/netherwire/Projects/grpc/third_party/protobuf && ./configure");
    system("cd /Users/netherwire/Projects/grpc/third_party/protobuf && make");

    // cd /Users/netherwire/Projects/grpc/third_party/protobuf && ./autogen.sh
    // cd /Users/netherwire/Projects/grpc/third_party/protobuf && ./autogen.sh


    return 0;
}