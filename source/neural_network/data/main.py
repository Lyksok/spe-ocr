import string
from urllib.request import urlopen, urlretrieve
from PIL import Image, ImageDraw, ImageFont
from sys import argv
import random
import requests
import os
from multiprocessing import Pool

CACHE_FOLDER = "cache/fonts"

# Créer le dossier de cache s'il n'existe pas
if not os.path.exists(CACHE_FOLDER):
    os.makedirs(CACHE_FOLDER, exist_ok=True)


def get_font_path(font_url):
    """
    Check if the font is already cached locally. If not, download it.
    :param font_url: URL of the TTF font file.
    :return: Path to the locally cached TTF file.
    """
    font_name = font_url.split("/")[-1]  # Extract the font filename from the URL
    local_path = os.path.join(CACHE_FOLDER, font_name)

    if not os.path.exists(local_path):
        print(f"Downloading font: {font_name}")
        urlretrieve(font_url, local_path)  # Download and save the file locally
    else:
        print(f"Using cached font: {font_name}")

    return local_path


def getRandomFont(apiKey: str, usedFont: list = None) -> str:
    """
    Function to fetch a random font from a list of available fonts
    provided by Google Font
    :param apiKey: API Key used to connect to Google Fonts.
    :param usedFont: List of already used fonts. Set to None by default
    :return: Local path to the cached TTF file.
    """
    if usedFont is None:
        usedFont = []
    url = f"https://www.googleapis.com/webfonts/v1/webfonts?key={apiKey}"
    response = requests.get(url)
    if response.status_code != 200:
        raise Exception(f"Failed to fetch fonts: {response.status_code} - {response.reason}")

    fonts_data = response.json()
    if "items" not in fonts_data:
        raise Exception("No fonts found in the API response.")
    available_fonts = fonts_data["items"]
    unused_fonts = [font for font in available_fonts if font["family"] not in usedFont]

    if not unused_fonts:
        raise Exception("No unused fonts available to select from.")
    chosen_font = random.choice(unused_fonts)

    usedFont.append(chosen_font["family"])

    font_files = chosen_font.get("files", {})
    ttf_link = font_files.get("regular") or next(iter(font_files.values()), None)

    if not ttf_link:
        raise Exception(f"No TTF link found for the font: {chosen_font['family']}")

    print(f"Selected font: {chosen_font['family']}")
    return get_font_path(ttf_link)


def create_letter_image(letter, output_path, font_path):
    """
    Function to create an image for a letter passed as an argument
    :param letter: the letter
    :param output_path: the path to save the image
    :param font_path: local path to the TTF font file
    :return: print a message and save the image
    """
    width, height = 13, 13
    font_size = 12

    image = Image.new('L', (width, height), color=255)  # Fond blanc
    draw = ImageDraw.Draw(image)

    try:
        font = ImageFont.truetype(font_path, font_size)
    except IOError:
        raise Exception(f"Cannot load font file: {font_path}")

    bbox = draw.textbbox((0, 0), letter, font=font)
    text_width, text_height = bbox[2] - bbox[0], bbox[3] - bbox[1]

    text_x = (width - text_width) // 2
    text_y = (height - text_height) // 2

    draw.text((text_x, text_y), letter, fill=0, font=font)  # Texte noir

    image.save(output_path)
    print(f"Image saved at {output_path}")


def createImageForEachLetter(args):
    """
    Function to create an image for each of the letters passed as an argument
    :param args: Tuple containing output folder, font path, index, and type (training/validation)
    """
    output_folder, font_path, index, dataset_type = args
    alphabet = string.ascii_letters  # Inclut à la fois les lettres minuscules et majuscules

    base_folder = os.path.join(output_folder, dataset_type)
    os.makedirs(base_folder, exist_ok=True)

    for element in alphabet:
        letter_folder = os.path.join(base_folder, element)
        os.makedirs(letter_folder, exist_ok=True)

        create_letter_image(element, f"{letter_folder}/{element}-{index}.png", font_path)


if __name__ == "__main__":
    if len(argv) < 3:
        raise Exception("Usage: " + argv[0] + " <number-of-iterations> <api-key>")
    if input(f"This script will generate {argv[1]} images for each letter in the alphabet. Continue? Y/n ") != "Y":
        print("Exiting...")
        exit(0)

    # Configuration
    num_iterations = int(argv[1])
    api_key = argv[2]
    usedFont = []

    # Fetch fonts and cache them
    fonts = [getRandomFont(api_key, usedFont) for _ in range(num_iterations)]

    # Split into training and validation datasets
    num_training = int(num_iterations * 0.7)
    num_validation = num_iterations - num_training

    training_args = [("dataset", fonts[i], i, "training") for i in range(num_training)]
    validation_args = [("dataset", fonts[i], i, "validation") for i in range(num_training, num_iterations)]

    # Use multiprocessing pool
    with Pool() as pool:
        pool.map(createImageForEachLetter, training_args + validation_args)
