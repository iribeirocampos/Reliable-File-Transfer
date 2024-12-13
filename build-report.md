# Report
Date: Fri 13 Dec 2024 12:00:50 PM WET  
Repo: git@gitlab.rnl.tecnico.ulisboa.pt:rc/rc-24-25/ist151948-proj1.git  
Commit: 6293361bee77a1e43b1ca0e5baf63d0e86379646  

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
| Stop & Wait. Loss | OK |
| Go Back N. No Loss | OK |
| Go Back N. Loss | OK |
| Selective Repeat. No Loss | OK |
| Selective Repeat. Loss | **FAIL** |
| Message format | **FAIL** |

## Submission
* **`project1-submission` tag missing. Project not yet submitted.**
