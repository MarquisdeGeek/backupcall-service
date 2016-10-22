<?php
include_once 'lib/class.iCalReader.php';
include_once 'inc/settings.inc';
include_once 'inc/db.inc';

// TODO FIXME
date_default_timezone_set('UTC');

$v = new Vodka();

include_once 'app/start.inc';
include_once 'app/username.inc';
include_once 'app/password.inc';
include_once 'app/menu.inc';
include_once 'app/contacts.inc';
include_once 'app/record.inc';
include_once 'app/broadcast.inc';
include_once 'app/data.inc';
include_once 'app/error.inc';
	
?>
