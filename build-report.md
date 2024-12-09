# Report
Date: Mon 09 Dec 2024 04:00:31 AM WET  
Repo: git@gitlab.rnl.tecnico.ulisboa.pt:rc/rc-24-25/ist151948-proj1.git  
Commit: d7def567c829a6872557b7ae92273219a40d4a8e  

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
| Go Back N. No Loss | **FAIL** |
| Go Back N. Loss | **FAIL** |
| Selective Repeat. No Loss | **FAIL** |
| Selective Repeat. Loss | **FAIL** |
| Message format | **FAIL** |

## Submission
* **`project1-submission` tag missing. Project not yet submitted.**
