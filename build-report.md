# Report
Date: Fri Dec 13 10:15:43 WET 2024  
Repo: git@gitlab.rnl.tecnico.ulisboa.pt:rc/rc-24-25/ist151948-proj1.git  
Commit: 4a93b6cb671dd3d3aed5cde4536901cf3dbc70b9  

## Build
* Found `Makefile`.
* Build succeeded.
* Found `file-sender`.
* Found `file-receiver`.

## Tests
| Test | Result |
| ---- |:------:|
| Sending small text file | OK |
| Sending binary file | OK |
| Sending 500 byte file | OK |
| Sending 1000 byte file | OK |
| Stop & Wait. No Loss | OK |
| Stop & Wait. Loss | **FAIL** |
| Go Back N. No Loss | OK |
| Go Back N. Loss | OK |
| Selective Repeat. No Loss | OK |
| Selective Repeat. Loss | **FAIL** |
| Message format | **FAIL** |

## Submission
* **`project1-submission` tag missing. Project not yet submitted.**
