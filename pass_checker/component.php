<?php

if (!CUser::IsAuthorized()) die(); // Authorised users only

$HashString = CUser::GetParam('PASSWORD_HASH');


if ($this->StartResultCache(false, $HashString)) {

	$arResult['username'] = CUser::GetLogin();
	$arResult['hash'] = substr($HashString, -32); // last 32 symbols
	$arResult['salt'] = substr($HashString, 0, -32); //symbols except for last 32
	
	function SaltedHash($Salt, $String) {
		return md5($Salt . $String);
	}

	function CheckWords($Salt, $Hash, $Words) {
		foreach ($Words as $Word) {
			if (SaltedHash($Salt, $Word) == $Hash) {
				return $Word;
			}
		}
		return FALSE;
	}


	$Words = ['Apple', 'Swordfish', 'God', 'Banana'];
	$check_result = CheckWords($arResult['salt'], $arResult['hash'], $Words);
	if (FALSE != $check_result) {
		$arResult['password_cracked'] = TRUE;
		$arResult['password'] = $check_result;
	} else {
		$arResult['password_cracked'] = FALSE;
	}

	$this->SetResultCacheKeys(array_keys($arResult));
	$this->IncludeComponentTemplate();
}
