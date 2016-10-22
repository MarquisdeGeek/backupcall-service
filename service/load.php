<?php
require_once 'inc/db.inc';

	$id = json_decode($_GET["id"]);

	$db = new VDB('users');
	$user = $db->getRecord($id);

	echo json_encode($user);
?>
