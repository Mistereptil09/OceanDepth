## File Structure

```
oceandepths/
├── documentation/
│   ├── struct.md          # Data structures
│   ├── guidelines.md      # This file
│   └── api.md             # Function documentation
├── code/                  # Main code directory (matches submission)
│   ├── config.txt         # Configuration (optional)
│   ├── src/               # All .c files
│   │   ├── core/          # Game logic (SDL-independent)
│   │   │   ├── creatures.c
│   │   │   ├── joueur.c
│   │   │   ├── combat.c
│   │   │   ├── inventaire.c
│   │   │   ├── carte.c
│   │   │   └── sauvegarde.c
│   │   ├── interface/     # Display abstraction
│   │   │   ├── display.c
│   │   │   ├── terminal_ui.c
│   │   │   └── sdl_ui.c   # (future, if we get there)
│   │   └── main.c
│   ├── include/           # All .h files
│   │   ├── core/
│   │   │   ├── creatures.h
│   │   │   ├── joueur.h
│   │   │   ├── combat.h
│   │   │   ├── inventaire.h
│   │   │   ├── carte.h
│   │   │   └── sauvegarde.h
│   │   ├── interface/      
│   │   │   ├── display.h
│   │   │   ├── terminal_ui.h
│   │   │   └── sdl_ui.h
│   │   └── common.h       # Common types/constants
│   └── saves/             # Save game files
├── CMakeLists.txt     # CMake build configuration (CLion)
├── identifiants.txt      # Student names
├── README.md             # Documentation and instructions
└── PROGRESSION.md        # Progress steps with screenshots
```