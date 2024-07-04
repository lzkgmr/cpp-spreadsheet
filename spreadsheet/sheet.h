#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

class Sheet : public SheetInterface {
public:
    ~Sheet() = default;

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

private:
    struct PosHasher {
        size_t operator()(const Position &pos) const {
            return pos.row + pos.col * 43;
        }
    };
    
    static void CheckPositionValidity(Position pos);
    
    void UpdateSize(); 
    
    std::unordered_map<Position, std::unique_ptr<Cell>, PosHasher> sheet_;
    Size size_ = {0, 0};
};