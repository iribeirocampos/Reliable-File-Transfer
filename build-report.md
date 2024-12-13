# Report
Date: Fri 13 Dec 2024 06:00:26 PM WET  
Repo: git@gitlab.rnl.tecnico.ulisboa.pt:rc/rc-24-25/ist151948-proj1.git  
Commit: 85689d3c962aa07e75a04151893c61dd4f20958a  

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
| Sending 500 byte file | **FAIL** |
| Sending 1000 byte file | **FAIL** |
| Stop & Wait. No Loss | OK |
| Stop & Wait. Loss | OK |
| Go Back N. No Loss | OK |
| Go Back N. Loss | OK |
| Selective Repeat. No Loss | OK |
| Selective Repeat. Loss | OK |
| Message format | OK |

## Submission
* **`project1-submission` tag missing. Project not yet submitted.**
