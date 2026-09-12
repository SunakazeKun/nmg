from pathlib import Path

import json

__all__ = ["read_bin", "write_bin",
           "JSONContainer", "JSONValue", "read_json" ,"write_json",
           "read_lines", "write_lines"]


# ----------------------------------------------------------------------------------------------------------------------

def read_bin(file_path: str | Path) -> bytearray:
    if isinstance(file_path, str):
        file_path = Path(file_path)
    return bytearray(file_path.read_bytes())


def write_bin(file_path: str | Path, data: bytes | bytearray):
    if isinstance(file_path, str):
        file_path = Path(file_path)
    file_path.write_bytes(data)


# ----------------------------------------------------------------------------------------------------------------------

JSONContainer = dict[str, 'JSONValue'] | list['JSONValue']
JSONValue = str | int | float | bool | JSONContainer | None


def read_json(file_path: str | Path, encoding: str = "utf-8") -> JSONContainer:
    if isinstance(file_path, str):
        file_path = Path(file_path)
    text = file_path.read_text(encoding=encoding)
    return json.loads(text)


def write_json(file_path: str | Path, data: JSONContainer, encoding: str = "utf-8") -> None:
    if isinstance(file_path, str):
        file_path = Path(file_path)
    text = json.dumps(data, indent=2, ensure_ascii=False)
    file_path.write_text(text, encoding=encoding, newline='\n')


# ----------------------------------------------------------------------------------------------------------------------

def read_lines(txt_path: str | Path, encoding: str = "utf-8") -> list[str]:
    lines = []
    with open(txt_path, "r", encoding=encoding) as f:
        for line in f:
            lines.append(line.strip("\r\n"))
    return lines


def write_lines(txt_path: str | Path, lines: list[str], encoding: str = "utf-8"):
    with open(txt_path, "w", encoding=encoding, newline='\n') as f:
        for line in lines:
            f.write(line)
            f.write('\n')
