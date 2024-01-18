#!/bin/bash

# Spezifische Vendor- und Manufacturer-ID einstellen
VENDOR_ID="0bda"
MANUFACTURER_ID="5830"

# Farbdefinitionen für die Ausgabe
GREEN='\033[0;32m'
NC='\033[0m'

# Alle eindeutigen /dev/video-Einträge abrufen
video_devices=( $(ls -1 /dev/video*) )

# Schleife über alle /dev/video-Einträge
for device in "${video_devices[@]}"; do
  # USB-Informationen über das Gerät abrufen
  usb_info=$(udevadm info -q property -n "$device")

  # Prüfen, ob die spezifische Vendor- und Manufacturer-ID vorhanden ist
  if echo "$usb_info" | grep -q "ID_VENDOR_ID=$VENDOR_ID" &&
    echo "$usb_info" | grep -q "ID_MODEL_ID=$MANUFACTURER_ID"; then
    echo -e "Kamera gefunden auf Gerät: ${GREEN}$device${NC}"
    ffmpeg -input_format yuyv422 -video_size 256x384 -i "$device" -vf 'crop=h=(ih/2):y=(ih/2)' -pix_fmt yuyv422 -f rawvideo - | ffplay -pixel_format gray16le -video_size 256x192 -f rawvideo -i - -vf 'signalstats, split [main][secondary]; [main] normalize=smoothing=10, format=pix_fmts=rgb48, pseudocolor=p=inferno, scale=w=2*iw:h=2*ih, drawtext=x=3:y=3:borderw=1:bordercolor=white:fontfile=FreeSerif.ttf:text=MIN\\: %{metadata\\:lavfi.signalstats.YMIN}    MAX\\: %{metadata\\:lavfi.signalstats.YMAX} [thermal]; [secondary] drawgraph=m1=lavfi.signalstats.YMIN:fg1=0xFFFF9040:m2=lavfi.signalstats.YMAX:fg2=0xFF0000FF:bg=0x303030:min=18500:max=24500:slide=scroll:size=512x64 [graph]; [thermal][graph] vstack'
    return 1
  fi
done

# Wenn keine passende Kamera gefunden wurde
echo "Keine Kamera mit den spezifischen IDs gefunden."
return 0
