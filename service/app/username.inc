<?php

$v->registerState('username', function() {
global $v;

	$username = trim($v->getLastInput());

	$udb = new VDB('users');
	$user_record = $udb->getRecord($username);

	if (!$user_record) {
		$v->tropo->say("I didn't get that. Please re-enter your number");
		generateUsername($v);

	} else {

		$v->tropo->say("Thank you ". $user_record->name);

		$db = new VDB('calls');
		$db->setRecord($v->getCallID(), array('user'=>$username));

		generatePasscode($v);
	}

	return $v->tropo->RenderJson();
});


?>
