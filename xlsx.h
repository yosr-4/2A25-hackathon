// Petit écrivain de fichiers Excel (.xlsx) sans bibliothèque externe.
// Un .xlsx est une archive ZIP qui contient quelques fichiers XML : on écrit donc
// les XML à la main puis on les range dans un ZIP « stocké » (sans compression).
// Ce fichier n'utilise que le C++ standard (aucune classe Qt).
#ifndef XLSX_H
#define XLSX_H

#include <cstdint>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

namespace xlsx {

// Une cellule : texte, nombre ou date (numéro de série Excel).
struct Cell {
    enum Type { Text, Number, Date };
    Type type = Text;
    std::string text;      // UTF-8
    double number = 0;

    static Cell str(const std::string &s) { Cell c; c.type = Text; c.text = s; return c; }
    static Cell num(double v) { Cell c; c.type = Number; c.number = v; return c; }
    // serial = nombre de jours depuis le 30/12/1899 (convention Excel)
    static Cell date(double serial) { Cell c; c.type = Date; c.number = serial; return c; }
};
typedef std::vector<Cell> Row;

namespace detail {

inline std::string esc(const std::string &s) {
    std::string o;
    for (unsigned char ch : s) {
        switch (ch) {
        case '&': o += "&amp;"; break;
        case '<': o += "&lt;"; break;
        case '>': o += "&gt;"; break;
        case '"': o += "&quot;"; break;
        default:
            if (ch >= 0x20 || ch == '\n' || ch == '\t') o += char(ch);  // caractères de contrôle interdits en XML
        }
    }
    return o;
}

// 0 -> A, 1 -> B, ... 26 -> AA
inline std::string colName(int index) {
    std::string name;
    for (int n = index + 1; n > 0; n = (n - 1) / 26)
        name.insert(name.begin(), char('A' + (n - 1) % 26));
    return name;
}

inline std::string numStr(double v) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.15g", v);
    return buf;
}

inline uint32_t crc32(const std::string &data) {
    static uint32_t table[256];
    static bool ready = false;
    if (!ready) {
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t c = i;
            for (int k = 0; k < 8; ++k) c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            table[i] = c;
        }
        ready = true;
    }
    uint32_t crc = 0xFFFFFFFFu;
    for (unsigned char ch : data) crc = table[(crc ^ ch) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFFu;
}

inline void put16(std::string &o, uint32_t v) { o += char(v & 0xFF); o += char((v >> 8) & 0xFF); }
inline void put32(std::string &o, uint32_t v) { put16(o, v & 0xFFFF); put16(o, (v >> 16) & 0xFFFF); }

// Archive ZIP sans compression (méthode 0).
inline std::string zip(const std::vector<std::pair<std::string, std::string>> &files) {
    const uint32_t dosTime = 0;                              // 00:00:00
    const uint32_t dosDate = ((2026 - 1980) << 9) | (1 << 5) | 1;   // 01/01/2026
    std::string out, central;
    for (const auto &f : files) {
        const std::string &name = f.first, &data = f.second;
        const uint32_t crc = crc32(data), size = uint32_t(data.size()), offset = uint32_t(out.size());
        // en-tête local
        put32(out, 0x04034b50); put16(out, 20); put16(out, 0x0800); put16(out, 0);
        put16(out, dosTime); put16(out, dosDate);
        put32(out, crc); put32(out, size); put32(out, size);
        put16(out, uint32_t(name.size())); put16(out, 0);
        out += name; out += data;
        // entrée du répertoire central
        put32(central, 0x02014b50); put16(central, 20); put16(central, 20); put16(central, 0x0800);
        put16(central, 0); put16(central, dosTime); put16(central, dosDate);
        put32(central, crc); put32(central, size); put32(central, size);
        put16(central, uint32_t(name.size())); put16(central, 0); put16(central, 0);
        put16(central, 0); put16(central, 0); put32(central, 0); put32(central, offset);
        central += name;
    }
    const uint32_t cdOffset = uint32_t(out.size()), cdSize = uint32_t(central.size());
    out += central;
    put32(out, 0x06054b50); put16(out, 0); put16(out, 0);
    put16(out, uint32_t(files.size())); put16(out, uint32_t(files.size()));
    put32(out, cdSize); put32(out, cdOffset); put16(out, 0);
    return out;
}

} // namespace detail

// Construit le contenu binaire d'un classeur .xlsx à une feuille.
// La première ligne de « rows » est l'en-tête (gras, fond bleu, figée, avec filtres).
// colWidths : largeur de chaque colonne (en caractères).
inline std::string build(const std::vector<Row> &rows, const std::string &sheetName,
                         const std::vector<double> &colWidths) {
    using namespace detail;
    const char *XMLH = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    size_t nCols = 0;
    for (const Row &r : rows) if (r.size() > nCols) nCols = r.size();

    // ---- feuille
    // styles : 0 = normal, 1 = en-tête, 2 = date, 3 = nombre centré
    std::string sheet = XMLH;
    sheet += "<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">";
    sheet += "<sheetViews><sheetView workbookViewId=\"0\">"
             "<pane ySplit=\"1\" topLeftCell=\"A2\" activePane=\"bottomLeft\" state=\"frozen\"/>"
             "</sheetView></sheetViews>";
    if (!colWidths.empty()) {
        sheet += "<cols>";
        for (size_t i = 0; i < colWidths.size(); ++i)
            sheet += "<col min=\"" + std::to_string(i + 1) + "\" max=\"" + std::to_string(i + 1)
                   + "\" width=\"" + numStr(colWidths[i]) + "\" customWidth=\"1\"/>";
        sheet += "</cols>";
    }
    sheet += "<sheetData>";
    for (size_t r = 0; r < rows.size(); ++r) {
        sheet += "<row r=\"" + std::to_string(r + 1) + "\"" + (r == 0 ? " ht=\"22\" customHeight=\"1\"" : "") + ">";
        for (size_t c = 0; c < rows[r].size(); ++c) {
            const Cell &cell = rows[r][c];
            const std::string ref = colName(int(c)) + std::to_string(r + 1);
            if (r == 0)
                sheet += "<c r=\"" + ref + "\" s=\"1\" t=\"inlineStr\"><is><t>" + esc(cell.text) + "</t></is></c>";
            else if (cell.type == Cell::Number)
                sheet += "<c r=\"" + ref + "\" s=\"3\"><v>" + numStr(cell.number) + "</v></c>";
            else if (cell.type == Cell::Date)
                sheet += "<c r=\"" + ref + "\" s=\"2\"><v>" + numStr(cell.number) + "</v></c>";
            else
                sheet += "<c r=\"" + ref + "\" t=\"inlineStr\"><is><t xml:space=\"preserve\">" + esc(cell.text) + "</t></is></c>";
        }
        sheet += "</row>";
    }
    sheet += "</sheetData>";
    if (rows.size() > 1 && nCols > 0)
        sheet += "<autoFilter ref=\"A1:" + colName(int(nCols) - 1) + std::to_string(rows.size()) + "\"/>";
    sheet += "</worksheet>";

    // ---- styles
    std::string styles = XMLH;
    styles += "<styleSheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">"
              "<fonts count=\"2\">"
              "<font><sz val=\"11\"/><name val=\"Calibri\"/></font>"
              "<font><b/><sz val=\"11\"/><color rgb=\"FFFFFFFF\"/><name val=\"Calibri\"/></font>"
              "</fonts>"
              "<fills count=\"3\">"
              "<fill><patternFill patternType=\"none\"/></fill>"
              "<fill><patternFill patternType=\"gray125\"/></fill>"
              "<fill><patternFill patternType=\"solid\"><fgColor rgb=\"FF0B2A6B\"/><bgColor indexed=\"64\"/></patternFill></fill>"
              "</fills>"
              "<borders count=\"1\"><border><left/><right/><top/><bottom/><diagonal/></border></borders>"
              "<cellStyleXfs count=\"1\"><xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"0\"/></cellStyleXfs>"
              "<cellXfs count=\"4\">"
              "<xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"0\" xfId=\"0\"/>"
              "<xf numFmtId=\"0\" fontId=\"1\" fillId=\"2\" borderId=\"0\" xfId=\"0\" applyFont=\"1\" applyFill=\"1\" applyAlignment=\"1\">"
              "<alignment horizontal=\"center\" vertical=\"center\"/></xf>"
              "<xf numFmtId=\"14\" fontId=\"0\" fillId=\"0\" borderId=\"0\" xfId=\"0\" applyNumberFormat=\"1\" applyAlignment=\"1\">"
              "<alignment horizontal=\"center\"/></xf>"
              "<xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"0\" xfId=\"0\" applyAlignment=\"1\">"
              "<alignment horizontal=\"center\"/></xf>"
              "</cellXfs>"
              "<cellStyles count=\"1\"><cellStyle name=\"Normal\" xfId=\"0\" builtinId=\"0\"/></cellStyles>"
              "</styleSheet>";

    // ---- classeur + relations + types
    std::string workbook = XMLH;
    workbook += "<workbook xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" "
                "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">"
                "<sheets><sheet name=\"" + esc(sheetName.substr(0, 31)) + "\" sheetId=\"1\" r:id=\"rId1\"/></sheets>"
                "</workbook>";
    std::string wbRels = XMLH;
    wbRels += "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
              "<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet\" Target=\"worksheets/sheet1.xml\"/>"
              "<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>"
              "</Relationships>";
    std::string rels = XMLH;
    rels += "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
            "<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"xl/workbook.xml\"/>"
            "</Relationships>";
    std::string types = XMLH;
    types += "<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">"
             "<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>"
             "<Default Extension=\"xml\" ContentType=\"application/xml\"/>"
             "<Override PartName=\"/xl/workbook.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml\"/>"
             "<Override PartName=\"/xl/worksheets/sheet1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml\"/>"
             "<Override PartName=\"/xl/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml\"/>"
             "</Types>";

    return zip({{"[Content_Types].xml", types},
                {"_rels/.rels", rels},
                {"xl/workbook.xml", workbook},
                {"xl/_rels/workbook.xml.rels", wbRels},
                {"xl/styles.xml", styles},
                {"xl/worksheets/sheet1.xml", sheet}});
}

} // namespace xlsx

#endif // XLSX_H
