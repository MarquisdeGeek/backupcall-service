# backupcall-service
The winning Tropo project from TADHack 2016, so provide emergency contact details to any phone, via DTMF.

## About

This is only the service component of the hack, as detailed at http://marquisdegeek.com/code_tadhack2016

Use the username '123456' and the password '1234', otherwise nothing much will appear to work.


## Installation

* Copy the service folder to the root of your web server.
* Amend the settings in service/inc/settings.inc accordingly
* Ensure the service/db folder is write-able by your web server.
* Ensure the service/recording.php script has write access to the media folder specific in settings

* Remember to setup and configure Tropo as appropriate. This includes preparing the callback as tropo_in.php?uri=start

You may test the install with the Vokda virtual phone type typing

```
cd service
php vphone.php
```

## Notes

The load.php and save.php are for interfacing with the front end (not included). There is
not security on these calls! (Hey - it's a hackathon, what did you expect?!)

