/*-
 * Copyright 2023 Diomidis Spinellis
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
 *
 * Read-only views of collections
 */

#pragma once

// A constant view of a collection
template <typename C>
class ConstCollectionView {
    const C& col;
public:
    ConstCollectionView(const C& col) : col(col) {}

    typename C::const_iterator begin() const {
        return col.begin();
    }
    typename C::const_iterator end() const {
        return col.end();
    }
};
