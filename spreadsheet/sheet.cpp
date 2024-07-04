#include "sheet.h"
#include "cell.h"
#include "common.h"

using namespace std::literals;

void Sheet::SetCell(Position pos, std::string text) {
    CheckPositionValidity(pos);
    if (size_.cols < pos.col + 1) {
        size_.cols = pos.col + 1;
    }
    if (size_.rows < pos.row + 1) {
        size_.rows = pos.row + 1;
    }
    if (!sheet_.count(pos)) {
        sheet_[pos] = std::make_unique<Cell>(*this);
    }
    sheet_[pos]->Set(text);
}

const CellInterface* Sheet::GetCell(Position pos) const {
    CheckPositionValidity(pos);
    if (!sheet_.count(pos)) {
        return nullptr;
    }
    return sheet_.at(pos).get();
}

CellInterface* Sheet::GetCell(Position pos) {
    CheckPositionValidity(pos);
    if (!sheet_.count(pos)) {
        return nullptr;
    }
    return sheet_.at(pos).get();
}

void Sheet::ClearCell(Position pos) {
    CheckPositionValidity(pos);
    if (!GetCell(pos)) return;
    sheet_.erase(pos);
    if (size_.rows == pos.row + 1 || size_.cols == pos.col + 1) {
        UpdateSize();
    }
}

Size Sheet::GetPrintableSize() const {
    return size_;
}

void Sheet::PrintValues(std::ostream& output) const {
    for (int i = 0; i < size_.rows; ++i) {
        for (int j = 0; j < size_.cols; ++j) {
            auto cell = GetCell({i, j});
            Cell::Value value;
            if (cell) {
                value = cell->GetValue();
            }
            switch (value.index()) {
                case 0: {
                    output << std::get<0>(value) << (j + 1 == size_.cols ? "" : "\t");
                    break;
                }
                case 1: {
                    output << std::get<1>(value) << (j + 1 == size_.cols ? "" : "\t");
                    break;
                }
                case 2: {
                    output << std::get<2>(value) << (j + 1 == size_.cols ? "" : "\t");
                    break;
                }
                default:
                    assert(false);
            }
        }
        output << '\n';
    }
}

void Sheet::PrintTexts(std::ostream& output) const {
    for (int i = 0; i < size_.rows; ++i) {
        for (int j = 0; j < size_.cols; ++j) {
            auto cell = GetCell({i, j});
            std::string text;
            if (cell) {
                text = cell->GetText();
            }
            output << text << (j + 1 == size_.cols ? "" : "\t");
        }
        output << '\n';
    }
}

void Sheet::CheckPositionValidity(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Pos is invalid"s);
    }
}

void Sheet::UpdateSize() {
    Size new_size{-1, -1};
    for (const auto &[pos, cell]: sheet_) {
        if (new_size.rows < pos.row) {
            new_size.rows = pos.row;
        }
        if (new_size.cols < pos.col) {
            new_size.cols = pos.col;
        }
    }
    size_ = {new_size.rows + 1, new_size.cols + 1};
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}
