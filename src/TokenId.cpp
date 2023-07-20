/*-
 * Copyright 2018 Diomidis Spinellis
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include "TokenId.h"

/**
 * Numeric identifiers used for representing tokens.
 * These need to be defined as well as declared until C++17
 */
constexpr token_type TokenId::CHARACTER;
constexpr token_type TokenId::KEYWORD;
constexpr token_type TokenId::OTHER_TOKEN;
constexpr token_type TokenId::NUMBER_START;
constexpr token_type TokenId::NUMBER_ZERO;
constexpr token_type TokenId::NUMBER_INFINITE;
constexpr token_type TokenId::NUMBER_NAN;
constexpr token_type TokenId::NUMBER_END;
constexpr token_type TokenId::IDENTIFIER;
