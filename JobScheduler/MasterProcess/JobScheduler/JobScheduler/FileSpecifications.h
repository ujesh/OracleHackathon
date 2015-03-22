#if !defined(_FILESPECIFICATIONS_H)
#define _FILESPECIFICATIONS_H

namespace FileCategory{
  enum Category{
    DOCUMENT,
    IMAGE,
    MOVIE,
    REPORT,
    NONE
  };
}

namespace FileType{
  enum Type{
    BINARY,
    TEXT,
    NONE
  };
}

#endif
