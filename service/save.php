<?php
require_once 'inc/db.inc';

	if (true) {
		echo "Saving has been disabled in this version.";
		return;
	}

	$data = json_decode($_GET["data"]);
	$id = $data->id;

	$db = new VDB('users');
	$db->setRecord($id, $data);

	$myFile = $id;

	echo 'Your data was updated successfully';
?>
