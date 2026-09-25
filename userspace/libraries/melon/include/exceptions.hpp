#pragma once

/// @brief Contains custom exceptions.
///
/// This namespace contains a bunch of useful exceptions to handle most errors
/// as possible.
namespace Melon::Exceptions
{}

#include "internal/exceptions/exception.hpp"
#include "internal/exceptions/buffer_overflow.hpp"
#include "internal/exceptions/out_of_range.hpp"
#include "internal/exceptions/file_not_found.hpp"
#include "internal/exceptions/null_stream.hpp"