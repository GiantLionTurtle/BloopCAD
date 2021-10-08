# This simple script copies the fonts used in BloopCAD into the user font folder, it is not global on the machine

echo "Installing OpenDyslexic font family..."
mkdir -p ~/.fonts # Make the directory if it doesn't already exists
cp ../GUI/Fonts/OpenDyslexic2/OpenDyslexic-Regular.otf ~/.fonts
cp ../GUI/Fonts/OpenDyslexic2/OpenDyslexic-Bold.otf ~/.fonts
cp ../GUI/Fonts/OpenDyslexic2/OpenDyslexic-Italic.otf ~/.fonts
cp ../GUI/Fonts/OpenDyslexic2/OpenDyslexic-BoldItalic.otf ~/.fonts