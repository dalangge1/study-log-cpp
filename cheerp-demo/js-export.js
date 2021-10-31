"use strict";
/*Compiled using Cheerp (R) by Leaning Technologies Ltd*/
var __imul=Math.imul;
var __fround=Math.fround;
var oSlot=0;var nullArray=[null];var nullObj={d:nullArray,o:0};
function __ZN8JsStruct4testEv(Larg0){
	console.log(_cheerpCreate_ZN6client6StringC2EPKc());
}
function _cheerpCreate_ZN6client6StringC2EPKc(){
	return String(__ZN6client6String11fromCharPtrIcEEPS0_PKT_());
}
function __ZN6client6String11fromCharPtrIcEEPS0_PKT_(){
	var tmp0=null;
	tmp0=String();
	tmp0=tmp0.concat(String.fromCharCode(73));
	tmp0=tmp0.concat(String.fromCharCode(110));
	tmp0=tmp0.concat(String.fromCharCode(118));
	tmp0=tmp0.concat(String.fromCharCode(111));
	tmp0=tmp0.concat(String.fromCharCode(107));
	tmp0=tmp0.concat(String.fromCharCode(101));
	tmp0=tmp0.concat(String.fromCharCode(100));
	tmp0=tmp0.concat(String.fromCharCode(32));
	tmp0=tmp0.concat(String.fromCharCode(116));
	tmp0=tmp0.concat(String.fromCharCode(101));
	tmp0=tmp0.concat(String.fromCharCode(115));
	return tmp0.concat(String.fromCharCode(116));
}
function __ZN8JsStructC1Efi(Larg0,Larg1,Larg2){
	Larg0.d0=Larg1;
	Larg0.i1=Larg2;
}
function JsStruct(a0,a1){
	this.d0=__fround(0.);
	this.i1=0;
	;
	this.d=[this];
	if (arguments.length===1&&arguments[0]===undefined){
		return;
	}
	__ZN8JsStructC1Efi(this,a0,a1);
}
JsStruct.prototype.test=function (){
	return __ZN8JsStruct4testEv(this);
};
JsStruct.promise=
Promise.resolve();
