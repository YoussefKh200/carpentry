# FullPI - Smart Carpentry Management

Modernized Qt desktop rebuild of the integration project, fully modularized in `fullpi`:
- single shared sidebar
- separate page files per gestion
- `.ui` forms for main window and pages
- Oracle (QODBC) CRUD integration
- charts, scrolling pages, PDF export (materials)

## Project Structure

- `main.cpp`
- `mainwindow.h/.cpp/.ui`
- `widgets/sidebarwidget.h/.cpp`
- `pages/`
  - `basegestionpage.h/.cpp`
  - `userspage.h/.cpp` + `userspageform.ui`
  - `clientspage.h/.cpp` + `clientspageform.ui`
  - `commandespage.h/.cpp` + `commandespageform.ui`
  - `fournisseurspage.h/.cpp` + `fournisseurspageform.ui`
  - `materiauxpage.h/.cpp` + `materiauxpageform.ui`
- `models/`
  - `usercrud.h/.cpp`
  - `clientcrud.h/.cpp`
  - `commandecrud.h/.cpp`
  - `fournisseurcrud.h/.cpp`
  - `materiauxcrud.h/.cpp`
- `db/connection.h/.cpp`
- `fullpi.pro`

## Qt Modules Used

Defined in `fullpi.pro`:
- `core`
- `gui`
- `widgets`
- `sql`
- `charts`
- `printsupport`

## Build and Run (Qt Creator - Recommended)

1. Open Qt Creator.
2. Open `fullpi/fullpi.pro`.
3. Select a valid Desktop kit (Qt 6 + compiler).
4. Build project.
5. Run.

## Build and Run (CLI)

Use a Qt-enabled terminal (for example: **Qt 6.x for Desktop (MinGW)** command prompt).

From `fullpi` directory:

```bash
qmake fullpi.pro
mingw32-make
./release/fullpi.exe
```

If `qmake` is not recognized, your terminal PATH does not include Qt tools.

## Database Configuration

Current DB connection is in `db/connection.cpp`:
- Driver: `QODBC`
- DSN/DatabaseName: `PROJET`
- User: `projet1`
- Password: `abc123`

Adjust these values to match your machine/ODBC setup.

## Notes

- Sidebar active page highlighting is enabled.
- All pages are scrollable via `BaseGestionPage`.
- Materials page includes PDF export and two charts.
- If Oracle column/table naming differs on your environment, update SQL queries in corresponding `models/*.cpp`.
