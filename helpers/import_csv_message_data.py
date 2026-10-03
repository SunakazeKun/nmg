from tools.dataio import read_json, write_json
from argparse import ArgumentParser
from pathlib import Path

import csv


########################################################################################################################

SRC_MESSAGE_DATA_PATH = Path("data/MessageData")
SHEET_LANGUAGES = [
    "UsEnglish",
    "EuEnglish",
    "UsSpanish",
    "EuSpanish",
    "UsFrench",
    "EuFrench",
    "EuGerman",
    "EuItalian",
    "EuDutch",
    "UsPortuguese",
    "EuPortuguese",
    "EuRussian",
    "JpJapanese",
    "KrKorean",
    "CnSimpChinese",
    "AsTradChinese",
]


def import_message_data_from_csv(csv_path: Path):
    with open(csv_path, encoding="utf-8") as f:
        reader = csv.DictReader(f, delimiter=",", quotechar='"')

        for row in reader:
            archive, bank, label = row["ID"].split("/")

            json_path = SRC_MESSAGE_DATA_PATH / f'{archive}/{bank}.json'
            json_data = read_json(json_path)

            messages = {m["label"]: m["languages"] for m in json_data}
            message = messages[label]

            for language in SHEET_LANGUAGES:
                text = row[language]

                if len(text) == 0:
                    message[language] = None
                else:
                    message[language] = text.split("\n")

                if "default" in message and message["default"] == message[language]:
                    del message[language]

            write_json(json_path, json_data)


########################################################################################################################

def __main():
    parser = ArgumentParser()
    parser.add_argument("csv_path", type=Path)
    args = parser.parse_args()

    import_message_data_from_csv(args.csv_path)


if __name__ == '__main__':
    __main()
