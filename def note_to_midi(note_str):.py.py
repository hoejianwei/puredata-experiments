def note_to_midi(note_str):
    """
    Converts a MIDI note string (e.g., 'C4', 'A#1', 'Bb2') to a MIDI number.
    """
    notes = {
        'C': 0, 'C#': 1, 'Db': 1, 'D': 2, 'D#': 3, 'Eb': 3,
        'E': 4, 'F': 5, 'F#': 6, 'Gb': 6, 'G': 7, 'G#': 8,
        'Ab': 8, 'A': 9, 'A#': 10, 'Bb': 10, 'B': 11
    }

    # Extract the note name and the octave
    # Handle cases where the note name is 2 characters (e.g., A# or Bb)
    if len(note_str) == 3:
        note_name = note_str[:2].capitalize()
        octave = int(note_str[2:])
    elif len(note_str) == 2:
        note_name = note_str[0].capitalize()
        octave = int(note_str[1:])
    else:
        # Handling octaves that might be negative or multi-digit (e.g., C-1 or C10)
        import re
        match = re.match(r"([A-Ga-g][#b]?)(-?\d+)", note_str)
        if match:
            note_name = match.group(1).capitalize()
            octave = int(match.group(2))
        else:
            raise ValueError(f"Invalid note format: {note_str}")

    if note_name not in notes:
        raise ValueError(f"Invalid note name: {note_name}")

    # Calculate MIDI number
    midi_number = (octave + 1) * 12 + notes[note_name]
    return midi_number

# --- Testing the script ---
test_notes = ["F3"]

print(f"{'Note':<10} | {'MIDI Number':<12}")
print("-" * 25)
for note in test_notes:
    try:
        print(f"{note:<10} | {note_to_midi(note):<12}")
    except ValueError as e:
        print(e)