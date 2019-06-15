#ifndef OBJ_H
#define OBJ_H

#include <memory>
#include "../matrices/face_list.h"
#include "command.h"

std::shared_ptr<FaceList> parse_obj_file(const std::string &filename);

class OBJ_FileParsingException : public std::runtime_error {
public:
    explicit OBJ_FileParsingException(const std::string &message);
};

#endif //OBJ_H
