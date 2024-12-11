# Report
Date: Wed 11 Dec 2024 06:00:33 PM WET  
Repo: git@gitlab.rnl.tecnico.ulisboa.pt:rc/rc-24-25/ist151948-proj1.git  
Commit: d0a0b25202254a1b140f4c61a79063d3a91e7a84  

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
| Go Back N. No Loss | **FAIL** |
| Go Back N. Loss | **FAIL** |
| Selective Repeat. No Loss | **FAIL** |
| Selective Repeat. Loss | **FAIL** |
| Message format | **FAIL** |

## Submission
* **`project1-submission` tag missing. Project not yet submitted.**
