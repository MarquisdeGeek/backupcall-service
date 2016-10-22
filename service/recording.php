<?php
include_once 'inc/settings.inc';

	$user = $_GET['user_id'];

	$url = $EmergencySettings['media_path_local'];
	$url .= $user . '.wav';

	unlink($url);

	error_log("Trying to record to $url ");

  if(move_uploaded_file($_FILES['filename']['tmp_name'], $url)) {
    error_log("$url [{$_FILES['filename']['size']} bytes] was saved");
	//chmod($url, 0755);

  } else {
    error_log("$url could not be saved.");
  }

?>
