#include <Windows.h>
#include "resource.h"
#include <CommDlg.h>
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <regex>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <Commctrl.h>

int restaraproducto;
int restahechaproducto;
int cantidadarestar;
char productoseleccionado[50];

char envioaborrar[50];
char productoaborrar[50];

int editarfoto1;
int editarfoto2;

char nomuser[50];
char nompass[50];

char checarnombreproducto[50];
int nombreproductorepedito;

char nombreusuario[50];
char alias[50];
char fotografia[500];

char usuarioactual[50];

char copiarfoto[500];
char copiarfotoproducto1[500];
char copiarfotoproducto2[500];
int nousuariosregistrados;
int usuarionoencontrado;
int contrasenaincorrecta;
int usuariorepetido;



int diaactuali;
int mesactuali;
int anioactuali;

struct fechaactual {
	fechaactual* siguiente;
	fechaactual* anterior;
	char diaactual[3];
	char mesactual[3];
	char anioactual[5];
};

struct fecha {
	char dia[3];
	char mes[3];
	char anio[5];
};


struct user {
	char nombre[50];
	char password[50];
	char aka[50];
	char alias[50];
	char foto[250];
	user* siguiente;
	user* anterior;
};

struct envios {
	envios* siguiente;
	envios* anterior;
	fecha fechaCita;
	int cantidad;
	char costototal[50];
	char calle[50];
	char colonia[50];
	char ciudad[50];
	char estado[50];
	char mensaje[50];
	int estadoenvio;
	char usuario[50];
	char producto[50];
};

struct productospersonales {
	productospersonales* siguiente;
	productospersonales* anterior;
	char usuario[50];
	char nombre[50];
	int cantidadinventario;
	char foto1[500];
	char foto2[500];
	int codigo;
	char marca[50];
	char descripcion[200];
	char monto[10];
};

user* inicio, * aux, * fin = nullptr;
envios* iniciop, * auxp, * finp = nullptr;
productospersonales* iniciopp, * auxpp, * finpp = nullptr;
fechaactual* ini,* au,* fi = nullptr;

void escribirUsuarios();
void leerUsuarios();
void escribirProductos();
void leerProductos();
void escribirEnvios();
void leerEnvios();

void AgregarUsuario(user* nuevo);
void BuscarUsuarioRepetido(char usuario[50]);
void BuscarUsuario(char usuario[50]);
void BuscarContrasena(char usuario[50]);
void AgregarProducto(productospersonales* nuevo);
void BuscarProductoRepetido(char producto[50]);
void AgregarEnvio(envios* nuevo);
void AgregarFechaActual(fechaactual* nuevo);

void quickSort(envios* iniL, envios* finL);


BOOL CALLBACK INICIARSESION(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK REGISTRO(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK INFVENDEDOR(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK NUEVOPRODUCTO(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ELIMINARPRODUCTO(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EDITARPRODUCTO(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MISPRODUCTOS(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK COMPRARPRODUCTOS(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK CANCELARENVIO(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EDITARENVIO(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MISENVIOS(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MyEditCallBckProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HBITMAP bmp;
HBITMAP bmp2;
HINSTANCE instGlobal;
WNDPROC lpEditWndProc;
HWND hWnd;

using namespace std;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {

	leerUsuarios();
	leerProductos();
	leerEnvios();

	HWND hVentana1 = CreateDialog(hInst, MAKEINTRESOURCE(IDD_INISIARSESION), NULL, INICIARSESION);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	ShowWindow(hVentana1, cShow);

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

BOOL CALLBACK INICIARSESION(HWND hwnd, UINT msg , WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: {
			fechaactual* nodouse = new fechaactual;
			SYSTEMTIME fechaActual;
			ZeroMemory(&fechaActual, sizeof(fechaActual));
			SendDlgItemMessage(hwnd, IDC_DATETIMEPICKER1, DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaActual);
			sprintf_s(nodouse->diaactual, "%d", fechaActual.wDay);
			sprintf_s(nodouse->mesactual, "%d", fechaActual.wMonth);
			sprintf_s(nodouse->anioactual, "%d", fechaActual.wYear);
			AgregarFechaActual(nodouse);

		}break;
		case BTN__INIS_INICIARSESION: {
			GetDlgItemText(hwnd, IDC_INIS_USUARIO, nomuser, 50);
			GetDlgItemText(hwnd, IDC_INIS_CONTRA, nompass, 50);
			BuscarUsuario(nomuser);
			BuscarContrasena(nompass);
			if (nousuariosregistrados == 0) {
				MessageBox(hwnd, "No hay usuarios registrados", "AVISO", MB_OK | MB_ICONERROR);
			}
			else if ( usuarionoencontrado == 0) {
				MessageBox(hwnd, "Usuario incorrecto", "AVISO", MB_OK | MB_ICONERROR);
			}
			else if(contrasenaincorrecta==0) {
				MessageBox(hwnd, "Contrasena incorrecta", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				strcpy_s(usuarioactual, nomuser);
				EndDialog(hwnd, 0);
				DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
			}

		}break;
		case IDC_INIS_REGISTRARSE: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal,MAKEINTRESOURCE(IDD_REGISTRO), hwnd, REGISTRO);
		}break;
		case IDC_INIS_SALIR: {
			int opc = MessageBox(0, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES:
				escribirProductos();
				escribirUsuarios();
				escribirEnvios();
				DestroyWindow(hwnd);
				PostQuitMessage(0);
				break;
			}
		}break;
		}
	}
	}
	return FALSE;
}
BOOL CALLBACK REGISTRO(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	user* nodous = new user;
	switch (msg) {
	case WM_INITDIALOG: {
		lpEditWndProc = (WNDPROC)SetWindowLong(GetDlgItem(hwnd, IDC_REG_NOMBREUSUARIO), GWL_WNDPROC, (LONG)MyEditCallBckProcedure);
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case BTN_REG_REGISTRARUSUARIO: {
			GetDlgItemText(hwnd, IDC_REG_NOMBREUSUARIO, nodous->aka, 50);
			GetDlgItemText(hwnd, IDC_REG_NOMBREUSUARIO, usuarioactual, 50);
			GetDlgItemText(hwnd, IDC_REG_CONTRA, nodous->password, 50);
			BuscarUsuarioRepetido(nodous->aka);
			if (usuariorepetido == 1) {
				MessageBox(hwnd, "Nombre de usuario repetido favor de ingresar otro", "AVISO", MB_OK | MB_ICONERROR);
			}
			else if (usuariorepetido==0) {
				AgregarUsuario(nodous);
				MessageBox(hwnd, "Usuario registrado", "AVISO", MB_OK | MB_ICONEXCLAMATION);
				EndDialog(hwnd, 0);
				DialogBox(instGlobal, MAKEINTRESOURCE(IDD_INFVENDEDOR), hwnd, INFVENDEDOR);
			}
		}break;

		case IDC_REG_VOLVER: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_INISIARSESION), hwnd, INICIARSESION);
		}break;

		}
	}
	}
	return FALSE;
}

BOOL CALLBACK INFVENDEDOR(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		lpEditWndProc = (WNDPROC)SetWindowLong(GetDlgItem(hwnd, IDC__INFV_NOMBRECOMPLETO), GWL_WNDPROC, (LONG)MyEditCallBckProcedure);
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case BTN_INFV_CARGAR: {
			aux = inicio;
			while (aux->siguiente != nullptr && strcmp(usuarioactual,aux->aka)!=0) {
				aux = aux->siguiente;
			}
			SetDlgItemText(hwnd, IDC__INFV_NOMBRECOMPLETO, aux->nombre);
			SetDlgItemText(hwnd, IDC_INFV_ALIAS, aux->alias);
			bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
			SendDlgItemMessage(hwnd, IDC_INFV_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		}break;
		case BTN_INFV_APLICAR: {

			GetDlgItemText(hwnd, IDC__INFV_NOMBRECOMPLETO, nombreusuario, 50);
			GetDlgItemText(hwnd, IDC_INFV_ALIAS, alias, 50);
			strcpy_s(fotografia, copiarfoto);

			aux = inicio;
			while (aux->siguiente != nullptr && aux->aka != usuarioactual) {
				aux = aux->siguiente;
			}
			strcpy_s(aux->nombre,nombreusuario);
			strcpy_s(aux->alias, alias);
			strcpy_s(aux->foto, fotografia);

			
		}break;
		case BTN_INFV_CARGARFOTO: {
			OPENFILENAME ofn;

			char zFile[250]; //260 caracteres

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.lpstrFileTitle = copiarfoto;
			ofn.nMaxFileTitle = sizeof(copiarfoto);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = copiarfoto;

			if (GetOpenFileName(&ofn) == TRUE) {
				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_INFV_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}
			else {
				MessageBox(hwnd, "No seleccionó una imagen", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		case ID_MENU_INFORMACIONVENDEDOR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_INFVENDEDOR), hwnd, INFVENDEDOR);
		}
		case ID_MENU_SALIR: {
			int opc = MessageBox(0, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES:
				escribirEnvios();
				escribirProductos();
				escribirUsuarios();
				DestroyWindow(hwnd);
				PostQuitMessage(0);
				break;
			}
		}break;
		case ID_MENU_PRODCUTOS_NUEVO: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_NUEVOPRODUCTO), hwnd, NUEVOPRODUCTO);
		}break;
		case ID_MENU_PRODCUTOS_ELIMINAR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_ELIMINARPRODUCTO), hwnd, ELIMINARPRODUCTO);
		}break;
		case ID_MENU_PRODCUTOS_EDITAR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_EDITARPRODUCTO), hwnd, EDITARPRODUCTO);
		}break;
		case ID_MENU_PRODCUTOS_MISPRODUCTOS: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISPRODUCTOS), hwnd, MISPRODUCTOS);
		}break;
		case ID_MENU_ENVIOS_COMPRAR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_COMPRARPRODUCTOS), hwnd, COMPRARPRODUCTOS);
		}break;
		case ID_MENU_ENVIOS_CANCELAR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_CANCELARENVIO), hwnd, CANCELARENVIO);
		}break;
		case ID_MENU_ENVIOS_EDITAR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_EDITARENVIO), hwnd, EDITARENVIO);
		}break;
		case ID_MENU_ENVIOS_MISENVIOS: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;
		case ID_MENU_CERRARSESION: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_INISIARSESION), hwnd, INICIARSESION);
		}break;
		}
	}

	}
	return FALSE;
}


BOOL CALLBACK NUEVOPRODUCTO(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char cantidad[4];
	char codigo[10];
	char monto[10];
	productospersonales* nodouse = new productospersonales;
	switch (msg) {
	case WM_INITDIALOG: {
		aux = inicio;
		while (aux->siguiente != nullptr && strcmp(usuarioactual, aux->aka) != 0) {
			aux = aux->siguiente;
		}
		SetDlgItemText(hwnd, nombreusuario_nuevop, aux->nombre);
		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		SendDlgItemMessage(hwnd, fotousuario_nuevoproducto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_NUEVOPRODUCTO_VOLVER: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;
		case IDC_NUEVOPRODUCTO_FOTO: {
			OPENFILENAME ofn;

			char zFile[500]; //260 caracteres

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_NUEVOPRODUCTO_IMAGEN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				strcpy_s(copiarfotoproducto1, zFile);
			}
			else {
				MessageBox(hwnd, "No seleccionó una imagen", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;

		case IDC_NUEVOPRODUCTO_FOTO2: {
			OPENFILENAME ofn;

			char zFile[500]; //260 caracteres

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_NUEVOPRODUCTO_IMAGEN2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				strcpy_s(copiarfotoproducto2, zFile);
			}
			else {
				MessageBox(hwnd, "No seleccionó una imagen", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;

		case IDC_NUEVOPRODUCTO_REGISTRAR: {
			GetDlgItemText(hwnd, IDC_NUEVOPRODUCTO_NOMBRE, checarnombreproducto, 50);
			BuscarProductoRepetido(checarnombreproducto);
			if (nombreproductorepedito == 0) {
				MessageBox(hwnd, "Producto duplicado", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				strcpy_s(nodouse->usuario, usuarioactual);
				GetDlgItemText(hwnd, IDC_NUEVOPRODUCTO_NOMBRE, nodouse->nombre, 50);
				nodouse->cantidadinventario = GetDlgItemInt(hwnd, IDC_NUEVOPRODUCTO_CANTIDAD, NULL, FALSE);
				strcpy_s(nodouse->foto1, copiarfotoproducto1);
				strcpy_s(nodouse->foto2, copiarfotoproducto2);
				nodouse->codigo = GetDlgItemInt(hwnd, IDC_NUEVOPRODUCTO_CODIGO, NULL, FALSE);
				GetDlgItemText(hwnd, IDC_NUEVOPRODUCTO_MARCA, nodouse->marca, 50);
				GetDlgItemText(hwnd, IDC_NUEVOPRODUCTO_DESCRIPCION, nodouse->descripcion, 50);
				GetDlgItemText(hwnd, IDC_NUEVOPRODUCTO_MONTO, nodouse->monto, 10);
				AgregarProducto(nodouse);
				MessageBox(hwnd, "Producto registrado", "AVISO", MB_OK | MB_ICONEXCLAMATION);
				EndDialog(hwnd, 0);
				DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISPRODUCTOS), hwnd, MISPRODUCTOS);
			}
		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK ELIMINARPRODUCTO(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		auxpp = iniciopp;
		while (auxpp != nullptr) {
			if (strcmp(usuarioactual, auxpp->usuario) != 0) {
				auxpp = auxpp->siguiente;
			}
			else {
				SendMessage(GetDlgItem(hwnd, IDC_ELIMINARPRODUCTO_LISTA), LB_INSERTSTRING, 0, (LPARAM)auxpp->nombre);
				auxpp = auxpp->siguiente;
			}
		}

		aux = inicio;
		while (aux->siguiente != nullptr && strcmp(usuarioactual, aux->aka) != 0) {
			aux = aux->siguiente;
		}
		SetDlgItemText(hwnd, nombreusuarioeliminar, aux->nombre);
		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		SendDlgItemMessage(hwnd, fotoeliminarusuario, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {

		case IDC_ELIMINARPRODUCTO_LISTA: {

			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				char textolist[50] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_ELIMINARPRODUCTO_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_ELIMINARPRODUCTO_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				auxpp = iniciopp;
				while (auxpp->siguiente != nullptr && strcmp(textolist, auxpp->nombre) != 0) {
					auxpp = auxpp->siguiente;
				}
				strcpy_s(productoaborrar, textolist);
				SetDlgItemText(hwnd, IDC_ELIMINARPRODUCTO_NOMBRE, auxpp->nombre);
				SetDlgItemInt(hwnd, IDC_ELIMINARPRODUCTO_CANTIDAD, (UINT)auxpp->cantidadinventario, FALSE);
				SetDlgItemInt(hwnd, IDC_ELIMINARPRODUCTO_CODIGO, (UINT)auxpp->codigo, FALSE);
				SetDlgItemText(hwnd, IDC_ELIMINARPRODUCTO_MARCA, auxpp->marca);
				SetDlgItemText(hwnd, IDC_ELIMINARPRODUCTO_DESCRIPCION, auxpp->descripcion);
				SetDlgItemText(hwnd, IDC_ELIMINARPRODUCTO_MONTO, auxpp->monto);
				bmp = (HBITMAP)LoadImage(NULL, auxpp->foto1, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_ELIMINARPRODUCTO_FOTO1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				bmp2 = (HBITMAP)LoadImage(NULL, auxpp->foto2, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_ELIMINARPRODUCTO_FOTO2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
			}break;
			}

		}break;

		case IDC_ELIMINARPRODUCTO_ELIMINAR: {
			auxpp = iniciopp;
			productospersonales* ant = nullptr;
			while (auxpp != nullptr && strcmp(auxpp->nombre, productoaborrar)) {
				ant = auxpp;
				auxpp = auxpp->siguiente;
			}
			if (auxpp == iniciopp) {
				iniciopp = auxpp->siguiente;
				delete auxpp;
				auxpp = iniciopp;
			}
			else {
				ant->siguiente = auxpp->siguiente;
				delete auxpp;
				auxpp = iniciopp;
			}
			MessageBox(hwnd, "Producto eliminado", "AVISO", MB_OK | MB_ICONEXCLAMATION);
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;

		case IDC_ELIMINARPRODUCTO_VOLVER: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK EDITARPRODUCTO(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	editarfoto1 = 0;
	editarfoto2 = 0;
	switch (msg) {

	case WM_INITDIALOG: {
		auxpp = iniciopp;
		while (auxpp != nullptr) {
			if (strcmp(usuarioactual, auxpp->usuario) != 0) {
				auxpp = auxpp->siguiente;
			}
			else {
				SendMessage(GetDlgItem(hwnd, IDC_EDITARPRODUCTO_LISTA), LB_INSERTSTRING, 0, (LPARAM)auxpp->nombre);
				auxpp = auxpp->siguiente;
			}
		}


		aux = inicio;
		while (aux->siguiente != nullptr && strcmp(usuarioactual, aux->aka) != 0) {
			aux = aux->siguiente;
		}
		SetDlgItemText(hwnd, nombreusuarioeditarproducto, aux->nombre);
		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		SendDlgItemMessage(hwnd, fotoeditarproductousuario, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {

		case IDC_EDITARPRODUCTO_EDITARFOTO1: {

			editarfoto1 = 1;

			OPENFILENAME ofn;

			char zFile[500]; //260 caracteres

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_EDITARPRODUCTO_FOTO1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				strcpy_s(copiarfotoproducto1, zFile);
			}
			else {
				MessageBox(hwnd, "No seleccionó una imagen", "AVISO", MB_OK | MB_ICONERROR);
			}

		}break;

		case IDC_EDITARPRODUCTO_EDITARFOTO2: {

			editarfoto2 = 1;

			OPENFILENAME ofn;

			char zFile[500]; //260 caracteres

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_EDITARPRODUCTO_FOTO2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				strcpy_s(copiarfotoproducto2, zFile);
			}
			else {
				MessageBox(hwnd, "No seleccionó una imagen", "AVISO", MB_OK | MB_ICONERROR);
			}

		}break;

		case IDC_EDITARPRODUCTO_LISTA: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				char textolist[50] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_EDITARPRODUCTO_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_EDITARPRODUCTO_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				auxpp = iniciopp;
				while (auxpp->siguiente != nullptr && strcmp(textolist, auxpp->nombre) != 0) {
					auxpp = auxpp->siguiente;
				}
				SetDlgItemText(hwnd, IDC_EDITARPRODUCTO_NOMBRE, auxpp->nombre);
				SetDlgItemInt(hwnd, IDC_EDITARPRODUCTO_CANTIDADINVENTARIO, (UINT)auxpp->cantidadinventario, FALSE);
				SetDlgItemInt(hwnd, IDC_EDITARPRODUCTO_CODIGOPRODUCTO, (UINT)auxpp->codigo, FALSE);
				SetDlgItemText(hwnd, IDC_EDITARPRODUCTO_MARCA, auxpp->marca);
				SetDlgItemText(hwnd, IDC_EDITARPRODUCTO_DESCRIPCION, auxpp->descripcion);
				SetDlgItemText(hwnd, IDC_EDITARPRODUCTO_MONTO, auxpp->monto);
				bmp = (HBITMAP)LoadImage(NULL, auxpp->foto1, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_EDITARPRODUCTO_FOTO1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				bmp2 = (HBITMAP)LoadImage(NULL, auxpp->foto2, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_EDITARPRODUCTO_FOTO2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
			}break;
			}

		}break;

		case IDC_EDITARPRODUCTO_ACEPTAR: {

			GetDlgItemText(hwnd, IDC_EDITARPRODUCTO_NOMBRE, auxpp->nombre, 50);
			if (editarfoto1 == 1) {
				strcpy_s(auxpp->foto1, copiarfotoproducto1);
			}
			if (editarfoto2 == 1) {
				strcpy_s(auxpp->foto2, copiarfotoproducto2);
			}
			auxpp->codigo = GetDlgItemInt(hwnd, IDC_EDITARPRODUCTO_CODIGOPRODUCTO, NULL, FALSE);
			GetDlgItemText(hwnd, IDC_EDITARPRODUCTO_MARCA, auxpp->marca, 50);
			GetDlgItemText(hwnd, IDC_EDITARPRODUCTO_DESCRIPCION, auxpp->descripcion, 50);
			GetDlgItemText(hwnd, IDC_EDITARPRODUCTO_MONTO, auxpp->monto, 10);
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;

		case IDC_EDITARPRODUCTO_VOLVER: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK MISPRODUCTOS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		auxpp = iniciopp;
		while (auxpp != nullptr) {
			if (strcmp(usuarioactual, auxpp->usuario) != 0) {
				auxpp = auxpp->siguiente;
			}
			else {
				SendMessage(GetDlgItem(hwnd, IDC_MISPRODUCTOS_LISTA), LB_INSERTSTRING, 0, (LPARAM)auxpp->nombre);
				auxpp = auxpp->siguiente;
			}
		}

		aux = inicio;
		while (aux->siguiente != nullptr && strcmp(usuarioactual, aux->aka) != 0) {
			aux = aux->siguiente;
		}
		SetDlgItemText(hwnd, nombremispusuario, aux->nombre);
		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		SendDlgItemMessage(hwnd, mispusuariofoto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_MISPRODUCTOS_VOLVER: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;

		case IDC_MISPRODUCTOS_GLOBAL: {
			SendMessage(GetDlgItem(hwnd, IDC_MISPRODUCTOS_LISTA), LB_RESETCONTENT, 0, 0);

			auxpp = iniciopp;
			while (auxpp != nullptr) {
				if (auxpp->cantidadinventario > 0) {
					SendMessage(GetDlgItem(hwnd, IDC_MISPRODUCTOS_LISTA), LB_INSERTSTRING, 0, (LPARAM)auxpp->nombre);
					auxpp = auxpp->siguiente;
				}
				else {
					auxpp = auxpp->siguiente;
				}
			}
		}break;

		case IDC_MISPRODUCTOS_LISTA: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				char textolist[50] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_MISPRODUCTOS_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_MISPRODUCTOS_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				auxpp = iniciopp;
				while (auxpp->siguiente != nullptr && strcmp(textolist, auxpp->nombre)!=0) {
					auxpp = auxpp->siguiente;
				}
				SetDlgItemText(hwnd, IDC_MISPRODUCTOS_NOMBRE, auxpp->nombre);
				SetDlgItemInt(hwnd, IDC_MISPRODUCTOS_CANTIDAD, (UINT)auxpp->cantidadinventario, FALSE);
				SetDlgItemInt(hwnd, IDC_MISPRODUCTOS_CODIGO, (UINT)auxpp->codigo, FALSE);
				SetDlgItemText(hwnd, IDC_MISPRODUCTOS_MARCA, auxpp->marca);
				SetDlgItemText(hwnd, IDC_MISPRODUCTOS_DESCRIPCION, auxpp->descripcion);
				SetDlgItemText(hwnd, IDC_MISPRODUCTOS_MONTO, auxpp->monto);
				bmp = (HBITMAP)LoadImage(NULL, auxpp->foto1, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_MISPRODUCTOS_IMAGEN2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				bmp2 = (HBITMAP)LoadImage(NULL, auxpp->foto2, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_MISPRODUCTOS_IMAGEN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);

			}break;
			}
		}break;



		}
	}break;
	}
	return FALSE;
}

BOOL CALLBACK COMPRARPRODUCTOS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		auxpp = iniciopp;
		while (auxpp != nullptr) {
			if (auxpp->cantidadinventario > 0) {
				SendMessage(GetDlgItem(hwnd, IDC_COMPRARPRODUCTOS_ARTICULOSDISPONIBLES), LB_INSERTSTRING, 0, (LPARAM)auxpp->nombre);
				auxpp = auxpp->siguiente;
			}
			else {
				auxpp = auxpp->siguiente;
			}
		}

		aux = inicio;
		while (aux->siguiente != nullptr && strcmp(usuarioactual, aux->aka) != 0) {
			aux = aux->siguiente;
		}
		SetDlgItemText(hwnd, comprarmiusuarioproducto, aux->nombre);
		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		SendDlgItemMessage(hwnd, fotocomprarpro, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_COMPRARPRODUCTOS_VOLVER: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;
		case IDC_COMPRARPRODUCTOS_ARTICULOSDISPONIBLES: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				char textolist[50] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_COMPRARPRODUCTOS_ARTICULOSDISPONIBLES, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_COMPRARPRODUCTOS_ARTICULOSDISPONIBLES, LB_GETTEXT, indice, (LPARAM)textolist);
				strcpy_s(productoseleccionado, textolist);
				SetDlgItemText(hwnd, IDC_COMPRARPRODUCTOS_PRODUCTOACOMPRAR, productoseleccionado);
			}break;
			}
		}break;

		case IDC_COMPRARPRODUCTOS_CALCULAR: {
			float montototal;
			char precio[20];
			float cantidadacomprar;
			cantidadacomprar = GetDlgItemInt(hwnd, IDC_COMPRARPRODUCTOS_CANTIDAD, NULL, FALSE);
			auxpp = iniciopp;
			while (auxpp->siguiente != nullptr && strcmp(productoseleccionado,auxpp->nombre)!=0) {
				auxpp = auxpp->siguiente;
			}
			if (auxpp == nullptr) {
				MessageBox(hWnd, "No encontro el producto", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				float precioproductoseleccionado = atof(auxpp->monto);
				montototal = cantidadacomprar * precioproductoseleccionado;
				sprintf_s(precio, "%f", montototal);
				SetDlgItemText(hwnd, IDC_COMPRARPRODUCTOS_MONTO, precio);
			}
		}break;
		case IDC_COMPRARPRODUCTOS_ORDENAR: {
			envios* nodouse = new envios;
			SYSTEMTIME fechaCita;
			ZeroMemory(&fechaCita, sizeof(fechaCita));
			SendDlgItemMessage(hwnd, IDC_COMPRARPRODUCTOS_FECHAENVIO, DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaCita);
			sprintf_s(nodouse->fechaCita.dia,"%d",fechaCita.wDay);
			sprintf_s(nodouse->fechaCita.mes, "%d", fechaCita.wMonth);
			sprintf_s(nodouse->fechaCita.anio, "%d", fechaCita.wYear);


			GetDlgItemText(hwnd, IDC_COMPRARPRODUCTOS_CALLE, nodouse->calle, 50);
			GetDlgItemText(hwnd, IDC_COMPRARPRODUCTOS_COLONIA, nodouse->colonia, 50);
			GetDlgItemText(hwnd, IDC_COMPRARPRODUCTOS_CIUDAD, nodouse->ciudad, 50);
			GetDlgItemText(hwnd, IDC_COMPRARPRODUCTOS_ESTADO, nodouse->estado, 50);
			GetDlgItemText(hwnd, IDC_COMPRARPRODUCTOS_MENSAJE, nodouse->mensaje, 50);
			GetDlgItemText(hwnd, IDC_COMPRARPRODUCTOS_MONTO, nodouse->costototal, 50);
			nodouse->cantidad= GetDlgItemInt(hwnd, IDC_COMPRARPRODUCTOS_CANTIDAD, NULL, FALSE);
			strcpy_s(nodouse->usuario, usuarioactual);
			strcpy_s(nodouse->producto, productoseleccionado);

			AgregarEnvio(nodouse);
			MessageBox(hWnd, "Envio registrado", "AVISO", MB_OK | MB_ICONINFORMATION);
			auxpp = iniciopp;
			while (auxpp->siguiente != nullptr && strcmp(productoseleccionado, auxpp->nombre) != 0) {
				auxpp = auxpp->siguiente;
			}
			restaraproducto=auxpp->cantidadinventario;
			cantidadarestar = auxp->cantidad;
			restahechaproducto = restaraproducto - cantidadarestar;
			auxpp->cantidadinventario = restahechaproducto;
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;

		}
	}break;
	}
	return FALSE;
}

BOOL CALLBACK CANCELARENVIO(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		auxp = iniciop;
		while (auxp != nullptr) {
			if (strcmp(usuarioactual, auxp->usuario) != 0) {
				auxp = auxp->siguiente;
			}
			else {
				if (auxp->estadoenvio == 1) {
					auxp = auxp->siguiente;
				}
				else {
					SendMessage(GetDlgItem(hwnd, IDC_CANCELARENVIO_LISTA), LB_INSERTSTRING, 0, (LPARAM)auxp->producto);
					auxp = auxp->siguiente;
				}
			}
		}

		aux = inicio;
		while (aux->siguiente != nullptr && strcmp(usuarioactual, aux->aka) != 0) {
			aux = aux->siguiente;
		}
		SetDlgItemText(hwnd, cancelarenvusuarioact, aux->nombre);
		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		SendDlgItemMessage(hwnd, fotocancelarenvusuario, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_CANCELARENVIO_VOLVER: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;
		case IDC_CANCELARENVIO_LISTA: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[50] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_CANCELARENVIO_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_CANCELARENVIO_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				auxp = iniciop;
				while (auxp->siguiente != nullptr && strcmp(textolist, auxp->producto) != 0) {
					auxp = auxp->siguiente;
				}
				strcpy_s(envioaborrar, textolist);
				SetDlgItemText(hwnd, IDC_CANCELARENVIO_CALLE, auxp->calle);
				SetDlgItemText(hwnd, IDC_CANCELARENVIO_COLONIA, auxp->colonia);
				SetDlgItemText(hwnd, IDC_CANCELARENVIO_CIUDAD, auxp->ciudad);
				SetDlgItemText(hwnd, IDC_CANCELARENVIO_ESTADO, auxp->estado);
				SetDlgItemText(hwnd, IDC_CANCELARENVIO_MENSAJE, auxp->mensaje);
				SetDlgItemInt(hwnd, IDC_CANCELARENVIO_CANTIDAD, (UINT)auxp->cantidad, FALSE);
				SetDlgItemText(hwnd, IDC_CANCELARENVIO_ANIO, auxp->fechaCita.anio);
				SetDlgItemText(hwnd, IDC_CANCELARENVIO_MES, auxp->fechaCita.mes);
				SetDlgItemText(hwnd, IDC_CANCELARENVIO_DIA, auxp->fechaCita.dia);
			}break;
			}
		}break;

		case IDC_CANCELARENVIO_ACEPTAR: {
			auxp = iniciop;
			envios* ant = nullptr;
			while (auxp != nullptr && strcmp(auxp->producto, envioaborrar)) {
				ant = auxp;
				auxp = auxp->siguiente;
			}
			if (auxp == iniciop) {
				iniciop = auxp->siguiente;
				delete auxp;
				auxp = iniciop;
			}
			else {
				ant->siguiente = auxp->siguiente;
				delete auxp;
				auxp = iniciop;
			}
			MessageBox(hwnd, "Envio cancelado", "AVISO", MB_OK | MB_ICONEXCLAMATION);
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK EDITARENVIO(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		auxp = iniciop;
		while (auxp != nullptr) {
			if (strcmp(usuarioactual, auxp->usuario) != 0) {
				auxp = auxp->siguiente;
			}
			else {
				if (auxp->estadoenvio == 1) {
					auxp = auxp->siguiente;
				}
				else {
					SendMessage(GetDlgItem(hwnd, IDC_EDITARENVIO_LISTA), LB_INSERTSTRING, 0, (LPARAM)auxp->producto);
					auxp = auxp->siguiente;
				}
			}
		}

		aux = inicio;
		while (aux->siguiente != nullptr && strcmp(usuarioactual, aux->aka) != 0) {
			aux = aux->siguiente;
		}
		SetDlgItemText(hwnd, usuarioeditarenvi, aux->nombre);
		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		SendDlgItemMessage(hwnd, fotoeditenv, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_EDITARENVIO_VOLVER: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;


		case IDC_EDITARENVIO_LISTA: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[50] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_EDITARENVIO_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_EDITARENVIO_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				auxp = iniciop;
				while (auxp->siguiente != nullptr && strcmp(textolist, auxp->producto) != 0) {
					auxp = auxp->siguiente;
				}
				SetDlgItemText(hwnd, IDC_EDITARENVIO_CALLE, auxp->calle);
				SetDlgItemText(hwnd, IDC_EDITARENVIO_COLONIA, auxp->colonia);
				SetDlgItemText(hwnd, IDC_EDITARENVIO_CIUDAD, auxp->ciudad); 
				SetDlgItemText(hwnd, IDC_EDITARENVIO_ESTADO, auxp->estado); 
				SetDlgItemText(hwnd, IDC_EDITARENVIO_MENSAJE, auxp->mensaje);
				SetDlgItemInt(hwnd, IDC_EDITARENVIO_CANTIDAD, (UINT)auxp->cantidad, FALSE);
				SetDlgItemText(hwnd, IDC_EDITARENVIO_ANIO, auxp->fechaCita.anio);
				SetDlgItemText(hwnd, IDC_EDITARENVIO_MES, auxp->fechaCita.mes);
				SetDlgItemText(hwnd, IDC_EDITARENVIO_DIA, auxp->fechaCita.dia);
			}break;
			}

		}break;
		case IDC_EDITARENVIO_EDITAR: {
			SYSTEMTIME fechaCita;
			ZeroMemory(&fechaCita, sizeof(fechaCita));
			SendDlgItemMessage(hwnd, IDC_EDITARENVIO_FECHAENVIO, DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaCita);
			sprintf_s(auxp->fechaCita.dia, "%d", fechaCita.wDay);
			sprintf_s(auxp->fechaCita.mes, "%d", fechaCita.wMonth);
			sprintf_s(auxp->fechaCita.anio, "%d", fechaCita.wYear);
			MessageBox(hWnd, "Envio modificado", "AVISO", MB_OK | MB_ICONINFORMATION);
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK MISENVIOS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		quickSort(iniciop, finp);
		auxp = iniciop;
		while (auxp != nullptr) {
			if (strcmp(auxp->usuario, usuarioactual)) {
				auxp = auxp->siguiente;
			}
			else {
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_INSERTSTRING, 0, (LPARAM)auxp->producto);
				auxp = auxp->siguiente;
			}
		}
		au = ini;
		diaactuali =atoi(au->diaactual);
		mesactuali= atoi(au->mesactual);
		anioactuali= atoi(au->anioactual);
		SetDlgItemInt(hwnd, IDC_EDIT10, (UINT)diaactuali, FALSE);
		SetDlgItemInt(hwnd, IDC_EDIT11, (UINT)mesactuali, FALSE);
		SetDlgItemInt(hwnd, IDC_EDIT12, (UINT)anioactuali,FALSE);


		aux = inicio;
		while (aux->siguiente != nullptr && strcmp(usuarioactual, aux->aka) != 0) {
			aux = aux->siguiente;
		}
		SetDlgItemText(hwnd, usuario_misenvios, aux->nombre);
		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		SendDlgItemMessage(hwnd, usuario_misenvios_foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);


	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {

		case IDC_MISENVIOS_GLOBAL: {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
			auxp = iniciop;
			while (auxp != nullptr) {
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_INSERTSTRING, 0, (LPARAM)auxp->producto);
				auxp = auxp->siguiente;
			}

		}break;

		case IDC_LIST1: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[50] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textolist);
				auxp = iniciop;
				while (auxp->siguiente != nullptr && strcmp(textolist, auxp->producto) != 0) {
					auxp = auxp->siguiente;
				}
				SetDlgItemText(hwnd, IDC_MISENVIOS_DIRECCION_CALLE, auxp->calle);
				SetDlgItemText(hwnd, IDC_MISENVIOS_DIRECCION_COLONIA, auxp->colonia);
				SetDlgItemText(hwnd, IDC_MISENVIOS_DIRECCION_CIUDAD, auxp->ciudad);
				SetDlgItemText(hwnd, IDC_MISENVIOS_DIRECCION_ESTADO, auxp->estado);
				SetDlgItemText(hwnd, IDC_MISENVIOS_FECHAENVIO_DIA, auxp->fechaCita.dia);
				SetDlgItemText(hwnd, IDC_MISENVIOS_FECHAENVIO_MES, auxp->fechaCita.mes);
				SetDlgItemText(hwnd, IDC_MISENVIOS_FECHAENVIO_ANIO, auxp->fechaCita.anio);
				SetDlgItemText(hwnd, IDC_MISENVIOS_PRODUCTO, auxp->producto);
				SetDlgItemInt(hwnd, IDC_MISENVIOS_CANTIDAD, (UINT)auxp->cantidad, FALSE);
				int diaproducto = atoi(auxp->fechaCita.dia);
				int mesproducto = atoi(auxp->fechaCita.mes);
				int anioproducto = atoi(auxp->fechaCita.anio);
				if (anioproducto <= anioactuali && mesproducto <= mesactuali && diaproducto <= diaactuali) {
					auxp->estadoenvio = 1;
				}
				else {
					auxp->estadoenvio = 0;
				}
				if (auxp->estadoenvio == 1) {
					SetDlgItemText(hwnd, IDC_MISENVIOS_STATUS, "ENVIADO");
				}
				else if (auxp->estadoenvio == 0) {
					SetDlgItemText(hwnd, IDC_MISENVIOS_STATUS, "PENDIENTE DE ENVIO");
				}

			}break;
			}

		}break;

		case ID_MENU_INFORMACIONVENDEDOR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_INFVENDEDOR), hwnd, INFVENDEDOR);
		}
		case ID_MENU_SALIR: {
			int opc = MessageBox(0, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc) {
			case IDYES:
				escribirEnvios();
				escribirProductos();
				escribirUsuarios();
				DestroyWindow(hwnd);
				PostQuitMessage(0);
				break;
			}
		}break;
		case ID_MENU_PRODCUTOS_NUEVO: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_NUEVOPRODUCTO), hwnd, NUEVOPRODUCTO);
		}break;
		case ID_MENU_PRODCUTOS_ELIMINAR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_ELIMINARPRODUCTO), hwnd, ELIMINARPRODUCTO);
		}break;
		case ID_MENU_PRODCUTOS_EDITAR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_EDITARPRODUCTO), hwnd, EDITARPRODUCTO);
		}break;
		case ID_MENU_PRODCUTOS_MISPRODUCTOS: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISPRODUCTOS), hwnd, MISPRODUCTOS);
		}break;
		case ID_MENU_ENVIOS_COMPRAR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_COMPRARPRODUCTOS), hwnd, COMPRARPRODUCTOS);
		}break;
		case ID_MENU_ENVIOS_CANCELAR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_CANCELARENVIO), hwnd, CANCELARENVIO);
		}break;
		case ID_MENU_ENVIOS_EDITAR: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_EDITARENVIO), hwnd, EDITARENVIO);
		}break;
		case ID_MENU_ENVIOS_MISENVIOS: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MISENVIOS), hwnd, MISENVIOS);
		}break;
		case ID_MENU_CERRARSESION: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_INISIARSESION), hwnd, INICIARSESION);
		}break;
		}
	}break;

	}
	return FALSE;
}

void AgregarUsuario(user* nuevo) {
	if (inicio == nullptr) {
		inicio = new user;
		aux = inicio;
		aux->anterior = nullptr;
		aux->siguiente = nullptr;
		strcpy_s(aux->password, nuevo->password);
		strcpy_s(aux->aka, nuevo->aka);
	}
	else {
		aux = inicio;
		while (aux->siguiente != nullptr) {
			aux = aux->siguiente;
		}
		aux->siguiente = new user;
		aux->siguiente->siguiente = nullptr;
		aux->siguiente->anterior = aux;
		aux = aux->siguiente;
		fin = aux;
		strcpy_s(aux->password, nuevo->password);
		strcpy_s(aux->aka, nuevo->aka);
	}
}

void BuscarUsuarioRepetido(char usuario[50]) {
	usuariorepetido = 0;
	aux = inicio;
	if (inicio == nullptr) {
		usuariorepetido = 0;
	}
	while (aux != nullptr && strcmp(aux->aka, usuario)!=0) {
		aux = aux->siguiente;
	}
	if (aux == nullptr) {
		usuariorepetido = 0;
	}
	else {
		usuariorepetido = 1;
	}
}

LRESULT CALLBACK MyEditCallBckProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if ((uMsg == WM_CHAR) && (wParam >= '0' && wParam <= '9'))
	{
		MessageBox(hWnd, "No se aceptan numeros.", "AVISO", MB_OK | MB_ICONSTOP);

		return 0;
	}


	return CallWindowProc(lpEditWndProc, hWnd, uMsg, wParam, lParam);
}



void BuscarUsuario(char usuario[50]) {
	nousuariosregistrados = 1;
	usuarionoencontrado = 1;
	aux = inicio;
	char moduser[50];
	char modaka[50];
	char modcontra[50];
	if (aux == nullptr) {
		nousuariosregistrados = 0;
	}
	else {
		while (aux != nullptr && strcmp(aux->aka, usuario) != 0) {
			aux = aux->siguiente;
		}
		if (aux == nullptr) {
			usuarionoencontrado = 0;
		}
	}
}

void BuscarContrasena(char usuario[50]) {
	contrasenaincorrecta = 1;
	aux = inicio;
	char moduser[50];
	char modaka[50];
	char modcontra[50];
	if (aux == nullptr) {
		nousuariosregistrados = 0;
	}
	else {
		while (aux != nullptr && strcmp(aux->password, usuario) != 0) {
			aux = aux->siguiente;
		}
		if (aux == nullptr) {
			contrasenaincorrecta = 0;
		}
	}
}
void AgregarProducto(productospersonales* nuevo) {
	if (iniciopp == nullptr) {
		iniciopp = new productospersonales;
		auxpp = iniciopp;
		auxpp->siguiente = nullptr;
		auxpp->anterior = nullptr;
		
		strcpy_s(auxpp->usuario, nuevo->usuario);
		strcpy_s(auxpp->nombre, nuevo->nombre);
		auxpp->cantidadinventario = nuevo->cantidadinventario;
		strcpy_s(auxpp->foto1, nuevo->foto1);
		strcpy_s(auxpp->foto2, nuevo->foto2);
		auxpp->codigo = nuevo->codigo;
		strcpy_s(auxpp->marca, nuevo->marca);
		strcpy_s(auxpp->descripcion, nuevo->descripcion);
		strcpy_s(auxpp->monto, nuevo->monto);
	}
	else {
		auxpp = iniciopp;
		while (auxpp->siguiente != nullptr) {
			auxpp = auxpp->siguiente;
		}
		auxpp->siguiente = new productospersonales;
		auxpp->siguiente->siguiente = nullptr;
		auxpp->siguiente->anterior = auxpp;
		auxpp = auxpp->siguiente;
		finpp = auxpp;
		strcpy_s(auxpp->usuario, nuevo->usuario);
		strcpy_s(auxpp->nombre, nuevo->nombre);
		auxpp->cantidadinventario = nuevo->cantidadinventario;
		strcpy_s(auxpp->foto1, nuevo->foto1);
		strcpy_s(auxpp->foto2, nuevo->foto2);
		auxpp->codigo = nuevo->codigo;
		strcpy_s(auxpp->marca, nuevo->marca);
		strcpy_s(auxpp->descripcion, nuevo->descripcion);
		strcpy_s(auxpp->monto, nuevo->monto);
	}
}


void escribirUsuarios() {
	aux = inicio;
	ofstream escribirUsuario;
	escribirUsuario.open("Usuarios.bin", ios::out | ios::binary | ios::trunc);
	if (escribirUsuario.is_open()) {
		while (aux != nullptr) {
			escribirUsuario.write((char*)aux, sizeof(user));
			aux = aux->siguiente;
		}
		escribirUsuario.close();
		MessageBox(hWnd, "Archivo guardado", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		MessageBox(hWnd, "No se pudo abrir el archivo", "AVISO", MB_OK | MB_ICONERROR);
	}
}

void leerUsuarios() {
	ifstream leerUsuarios;
	leerUsuarios.open("Usuarios.bin", ios::in | ios::binary);

	aux = inicio;
	if (leerUsuarios.is_open()) {
		user* userleido = new user;

		while (!leerUsuarios.read((char*)userleido, sizeof(user)).eof()) {

			while (aux != nullptr && aux->siguiente != nullptr) {
				aux = aux->siguiente;
			}
			if (aux == nullptr) {
				inicio = userleido;
				inicio->siguiente = nullptr;
				inicio->anterior = nullptr;
				aux = inicio;
			}
			else {
				aux->siguiente = userleido;
				aux->siguiente->anterior = aux;
				aux = aux->siguiente;
				aux->siguiente = nullptr;
			}

			userleido = new user;

		}
		leerUsuarios.close();
	}
}


void escribirProductos() {
	auxpp = iniciopp;
	ofstream escribirProduct;
	escribirProduct.open("Productos.bin", ios::out | ios::binary | ios::trunc);
	if (escribirProduct.is_open()) {
		while (auxpp != nullptr) {
			escribirProduct.write((char*)auxpp, sizeof(productospersonales));
			auxpp = auxpp->siguiente;
		}
		escribirProduct.close();
	}
	else {
		MessageBox(hWnd, "No se pudo abrir el archivo", "AVISO", MB_OK | MB_ICONERROR);
	}
}

void leerProductos() {
	ifstream leerProductos;
	leerProductos.open("Productos.bin", ios::in | ios::binary);

	auxpp = iniciopp;
	if (leerProductos.is_open()) {
		productospersonales* productospersonalesleido = new productospersonales;

		while (!leerProductos.read((char*)productospersonalesleido, sizeof(productospersonales)).eof()) {

			while (auxpp != nullptr && auxpp->siguiente != nullptr) {
				auxpp = auxpp->siguiente;
			}
			if (auxpp == nullptr) {
				iniciopp = productospersonalesleido;
				iniciopp->siguiente = nullptr;
				iniciopp->anterior = nullptr;
				auxpp = iniciopp;
			}
			else {
				auxpp->siguiente = productospersonalesleido;
				auxpp->siguiente->anterior = auxpp;
				auxpp= auxpp->siguiente;
				auxpp->siguiente = nullptr;
			}

			productospersonalesleido = new productospersonales;

		}
		leerProductos.close();
	}
}

void BuscarProductoRepetido(char producto[50]) {
	nombreproductorepedito = 1;
	auxpp = iniciopp;
	if (iniciopp == nullptr) {
		nombreproductorepedito = 1;
	}
	else {
		while (auxpp != nullptr && strcmp(auxpp->nombre, producto)!=0)  {
			auxpp = auxpp->siguiente;
		}
		if (auxpp == nullptr) {
			nombreproductorepedito = 1;
		}
		else {
			if (strcmp(auxpp->usuario, usuarioactual) != 0) {
				nombreproductorepedito = 1;
			}
			else {
				nombreproductorepedito = 0;
			}
		}
	}
}

void AgregarEnvio(envios* nuevo) {
	if (iniciop == nullptr) {
		iniciop = new envios;
		auxp = iniciop;
		auxp->siguiente = nullptr;
		auxp->anterior = nullptr;

		strcpy_s(auxp->calle, nuevo->calle);
		strcpy_s(auxp->colonia, nuevo->colonia);
		strcpy_s(auxp->ciudad, nuevo->ciudad);
		strcpy_s(auxp->estado, nuevo->estado);
		strcpy_s(auxp->mensaje, nuevo->mensaje);
		strcpy_s(auxp->costototal, nuevo->costototal);
		strcpy_s(auxp->fechaCita.dia, nuevo->fechaCita.dia);
		strcpy_s(auxp->fechaCita.mes, nuevo->fechaCita.mes);
		strcpy_s(auxp->fechaCita.anio, nuevo->fechaCita.anio); 
		strcpy_s(auxp->usuario, nuevo->usuario);
		strcpy_s(auxp->producto, nuevo->producto);
		auxp->cantidad = nuevo->cantidad;

		finp = iniciop;
	}
	else {
		auxp = iniciop;
		while (auxp->siguiente != nullptr) {
			auxp = auxp->siguiente;
		}
		auxp->siguiente = new envios;
		auxp->siguiente->siguiente = nullptr;
		auxp->siguiente->anterior = auxp;
		auxp = auxp->siguiente;
		strcpy_s(auxp->calle, nuevo->calle);
		strcpy_s(auxp->colonia, nuevo->colonia);
		strcpy_s(auxp->ciudad, nuevo->ciudad);
		strcpy_s(auxp->estado, nuevo->estado);
		strcpy_s(auxp->mensaje, nuevo->mensaje);
		strcpy_s(auxp->costototal, nuevo->costototal);
		strcpy_s(auxp->fechaCita.dia, nuevo->fechaCita.dia);
		strcpy_s(auxp->fechaCita.mes, nuevo->fechaCita.mes);
		strcpy_s(auxp->fechaCita.anio, nuevo->fechaCita.anio);
		strcpy_s(auxp->usuario, nuevo->usuario);
		strcpy_s(auxp->producto, nuevo->producto);
		auxp->cantidad = nuevo->cantidad;

		finp = auxp;
	}
}

void AgregarFechaActual(fechaactual* nuevo) {
	if (ini == nullptr) {
		ini = new fechaactual;
		au = ini;
		au->siguiente = nullptr;
		au->anterior = nullptr;

		strcpy_s(au->diaactual, nuevo->diaactual);
		strcpy_s(au->mesactual, nuevo->mesactual);
		strcpy_s(au->anioactual, nuevo->anioactual);

		fi = ini;
	}
	else {
		au = ini;
		while (au->siguiente != nullptr) {
			au = au->siguiente;
		}
		au->siguiente = new fechaactual;
		au->siguiente->siguiente = nullptr;
		au->siguiente->anterior = au;
		au = au->siguiente;
		strcpy_s(au->diaactual, nuevo->diaactual);
		strcpy_s(au->mesactual, nuevo->mesactual);
		strcpy_s(au->anioactual, nuevo->anioactual);
		fi = au;
	}
}


void escribirEnvios() {
	auxp = iniciop;
	ofstream escribirEnvios;
	escribirEnvios.open("Envios.bin", ios::out | ios::binary | ios::trunc);
	if (escribirEnvios.is_open()) {
		while (auxp != nullptr) {
			escribirEnvios.write((char*)auxp, sizeof(envios));
			auxp = auxp->siguiente;
		}
		escribirEnvios.close();
	}
	else {
		MessageBox(hWnd, "No se pudo abrir el archivo", "AVISO", MB_OK | MB_ICONERROR);
	}
}

void leerEnvios() {
	ifstream leerEnvios;
	leerEnvios.open("Envios.bin", ios::in | ios::binary);
	auxp = iniciop;
	if (leerEnvios.is_open()) {
		envios* enviosleidos = new envios;

		while (!leerEnvios.read((char*)enviosleidos, sizeof(envios)).eof()) {
			while (auxp != nullptr && auxp->siguiente != nullptr) {
				auxp = auxp->siguiente;
			}
			if (auxp == nullptr) {
				iniciop = enviosleidos;
				iniciop->siguiente = nullptr;
				iniciop->anterior = nullptr;
				auxp = iniciop;
			}
			else {
				auxp->siguiente = enviosleidos;
				auxp->siguiente->anterior = auxp;
				auxp = auxp->siguiente;
				auxp->siguiente = nullptr;
			}
			enviosleidos = new envios;
		}
		leerEnvios.close();
	}
}

void swap(envios* a, envios* b)
{
	envios t = *a;
	*a = *b;
	*b = t;

	b->siguiente = a->siguiente; b->anterior = a->anterior;
	a->siguiente = t.siguiente; a->anterior = t.anterior;

}



/* Considers last element as pivot,
places the pivot element at its
correct position in sorted array,
and places all smaller (smaller than
pivot) to left of pivot and all greater
elements to right of pivot */
envios* partition(envios* l, envios* h)
{
	// set pivot as h element
	SYSTEMTIME DateSh;
	ZeroMemory(&DateSh, sizeof(DateSh));

	DateSh.wDay = atoi(h->fechaCita.dia);
	DateSh.wMonth = atoi(h->fechaCita.mes);
	DateSh.wYear = atoi(h->fechaCita.anio);

	//Con i nos vamos ir moviendo 
	envios* i = l->anterior;

	// Similar to "for (int j = l; j <= h- 1; j++)"
	for (envios* j = l; j != h; j = j->siguiente)
	{
		SYSTEMTIME DateSj;
		ZeroMemory(&DateSj, sizeof(DateSj));
		FILETIME DateFj;
		ZeroMemory(&DateFj, sizeof(DateFj));

		DateSj.wDay = atoi(j->fechaCita.dia);
		DateSj.wMonth = atoi(j->fechaCita.mes);
		DateSj.wYear = atoi(j->fechaCita.anio);

		if (DateSj.wYear <= DateSh.wYear && DateSj.wMonth <= DateSh.wMonth && DateSj.wDay < DateSh.wDay)
		{
			if (i == NULL) {
				i = l;
			}
			else {
				i = i->siguiente;
			}
			//i = (i == NULL) ? l : i->sig;

			swap((i), (j));
		}
	}
	if (i == NULL) {
		i = l;
	}
	else {
		i = i->siguiente;
	}
	//i = (i == NULL) ? l : i->sig; // Similar to i++
	swap((i), (h));
	return i;
}

// The main function to sort a linked list.
// It mainly calls _quickSort()
void quickSort(envios* iniL, envios* finL)
{
	if (finL != NULL && iniL != finL && iniL != finL->siguiente)
	{
		envios* p = partition(iniL, finL);
		quickSort(iniL, p->anterior);
		quickSort(p->siguiente, finL);
	}

}