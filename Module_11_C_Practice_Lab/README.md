# Module 11 – C Programming Practice Lab

Four independent C programs for the Module 11 practice lab.

| File | Topic |
| --- | --- |
| `question1_student_records.c` | Nested structures: three students, four courses, averages |
| `question2_student_pointers.c` | Pointers to structures for the same student records |
| `question3_month_enum.c` | Enumeration and month-day calculation (including leap years) |
| `question4_shape_area.c` | Enumeration, structure, union and shape-area calculation |

## Open in Visual Studio Code

1. Extract the ZIP.
2. Open VS Code and select **File > Open Folder**.
3. Choose the `Module_11_C_Practice_Lab` folder.
4. Open **Terminal > New Terminal**.
5. Compile and run any program as shown below. You'll need GCC installed and on PATH.

### Windows (PowerShell + GCC / MinGW)

```powershell
gcc question1_student_records.c -o question1.exe
.\question1.exe

gcc question2_student_pointers.c -o question2.exe
.\question2.exe

gcc question3_month_enum.c -o question3.exe
.\question3.exe

gcc question4_shape_area.c -o question4.exe -lm
.\question4.exe
```

### Linux / macOS (GCC)

```bash
gcc question1_student_records.c -o question1 && ./question1
gcc question2_student_pointers.c -o question2 && ./question2
gcc question3_month_enum.c -o question3 && ./question3
gcc question4_shape_area.c -o question4 -lm && ./question4
```

**Note:** The worksheet example labels choice `2` as rectangle, but this conflicts with its own menu. This code consistently uses Circle = 0, Rectangle = 1, Triangle = 2.
