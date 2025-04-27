# ChessEncryption
An encryption tool that converts text into FEN chess notation. And decrypts FEN chess notation into text.

# How?
I made a custom 5 bit character mapping, chunked each character into 29 bit blocks, then converted those blocks into base-13, mapping it onto chess pieces following fen notation, and reversing it for decryption