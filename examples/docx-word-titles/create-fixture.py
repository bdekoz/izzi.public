#!/usr/bin/env python3
"""Regenerate plain and titled example DOCX files as deterministic OOXML packages."""

from pathlib import Path
from zipfile import ZIP_DEFLATED, ZipFile, ZipInfo


PARTS = {
    '[Content_Types].xml': '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Types xmlns="http://schemas.openxmlformats.org/package/2006/content-types">
  <Default Extension="rels" ContentType="application/vnd.openxmlformats-package.relationships+xml"/>
  <Default Extension="xml" ContentType="application/xml"/>
  <Override PartName="/word/document.xml" ContentType="application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml"/>
</Types>
''',
    '_rels/.rels': '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
  <Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument" Target="word/document.xml"/>
</Relationships>
''',
    'word/document.xml': '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<w:document xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main">
  <w:body>
    <w:p><w:r><w:t>Every word gets one sec</w:t></w:r><w:r><w:t>ond.</w:t></w:r></w:p>
    <w:p/>
    <w:p><w:r><w:t>Then black.</w:t></w:r></w:p>
    <w:sectPr><w:pgSz w:w="12240" w:h="15840"/><w:pgMar w:top="1440" w:right="1440" w:bottom="1440" w:left="1440"/></w:sectPr>
  </w:body>
</w:document>
''',
}


def main():
    for filename, with_title in (('fixture.docx', False), ('fixture-with-title.docx', True)):
        target = Path(__file__).with_name(filename)
        with ZipFile(target, 'w') as archive:
            for name, text in PARTS.items():
                if with_title and name == 'word/document.xml':
                    text = text.replace('<w:body>', '<w:body>\n'
                                        '    <w:p><w:r><w:t>Word title demo</w:t></w:r></w:p>', 1)
                entry = ZipInfo(name, date_time=(1980, 1, 1, 0, 0, 0))
                entry.compress_type = ZIP_DEFLATED
                entry.external_attr = 0o100644 << 16
                archive.writestr(entry, text.encode('utf-8'))
        print(target)


if __name__ == '__main__':
    main()
