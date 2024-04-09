#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

#define RESPONSE(DATA_TYPE, DATA_DESCRIPTION, RESPONSE_POSTFIX) \
class Response##RESPONSE_POSTFIX : public oatpp::DTO \
{ \
public: \
	DTO_INIT(Response##RESPONSE_POSTFIX, DTO); \
\
	DTO_FIELD_INFO(status) \
	{ \
		info->description = "Status of operation"; \
	} \
	DTO_FIELD(String, status); \
\
	DTO_FIELD_INFO(data) \
	{ \
		info->description = DATA_DESCRIPTION; \
	} \
	DTO_FIELD(DATA_TYPE, data); \
};

#include OATPP_CODEGEN_END(DTO)

