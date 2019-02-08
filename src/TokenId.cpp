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
constexpr int TokenId::CHARACTER;
constexpr int TokenId::KEYWORD;
constexpr int TokenId::OTHER_TOKEN;
constexpr int TokenId::NUMBER_START;
constexpr int TokenId::NUMBER_ZERO;
constexpr int TokenId::NUMBER_INFINITE;
constexpr int TokenId::NUMBER_NAN;
constexpr int TokenId::NUMBER_END;
constexpr int TokenId::IDENTIFIER;
