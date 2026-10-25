#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <utility>

class SqlSelectQueryBuilder {
public:
    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        columns_.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept {
        for (const auto& column : columns) {
            columns_.push_back(column);
        }
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) {
        table_ = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& key,
                                    const std::string& value) {
        where_.emplace_back(key, value);
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        for (const auto& [key, value] : kv) {
            where_.emplace_back(key, value);
        }
        return *this;
    }

    std::string BuildQuery() const {
        std::ostringstream query;

        query << "SELECT ";
        if (columns_.empty()) {
            query << "*";
        } else {
            for (std::size_t i = 0; i < columns_.size(); ++i) {
                if (i != 0) query << ", ";
                query << columns_[i];
            }
        }

        if (!table_.empty()) {
            query << " FROM " << table_;
        }

        if (!where_.empty()) {
            query << " WHERE ";
            for (std::size_t i = 0; i < where_.size(); ++i) {
                if (i != 0) query << " AND ";
                query << where_[i].first << "=" << where_[i].second;
            }
        }

        query << ";";
        return query.str();
    }

private:
    std::vector<std::string> columns_;
    std::string table_;
    std::vector<std::pair<std::string, std::string>> where_;
};
