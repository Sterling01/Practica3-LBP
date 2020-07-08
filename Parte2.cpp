#include <string>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <fstream>

#include <math.h> 

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>

#include "LBPDescriptor.h"

using namespace std;
using namespace cv;

namespace fs = std::experimental::filesystem;

void WriteFile(vector<int> histograma, string nombreArchivo);
vector<int> ReadFile(string nombreArchivo);
double chiDistance(vector<int> hClase, vector<int> hImagen);
double euclideanDistance(vector<int> hClase, vector<int> hImagen);

int main(int argc, char *argv) {

	//Clase 1
	Mat imagen = imread("wall.jpg", IMREAD_COLOR);
	Mat CIELab;
	Mat gris;

	cvtColor(imagen, CIELab, COLOR_BGR2Lab);
	cvtColor(CIELab, gris, COLOR_BGR2GRAY);
	resize(gris, gris, Size(626, 417));
	resize(CIELab, CIELab, Size(626, 417));

	//Clase 2
	Mat imagen2 = imread("wood.jpg", IMREAD_COLOR);
	Mat CIELab2;
	Mat gris2;
	

	cvtColor(imagen2, CIELab2, COLOR_BGR2Lab);
	cvtColor(CIELab2, gris2, COLOR_BGR2GRAY);
	resize(gris2, gris2, Size(626, 417));
	resize(CIELab2, CIELab2, Size(626, 417));

	LBPDescriptor *lbp = new LBPDescriptor();
	vector<int> histograma = lbp->lbp(gris);
	vector<int> histograma2 = lbp->lbp(gris2);

	//cout << "Original" << endl;
	//for (int i = 0; i < 256; i++)
		//cout << histograma[i] << " ";
	//cout << endl;

	//Almacenar el histograma de la primera y segunda clase.
	WriteFile(histograma, "histograma1.txt");
	WriteFile(histograma2, "histograma2.txt");
		
	//Leer el histograma de la primera clase.
	cout << "Precision" << endl;
	cout << endl;

	vector<int> hClase1 = ReadFile("histograma1.txt");
	vector<int> hClase2 = ReadFile("histograma2.txt");

	//cout << "Lectura" << endl;
	//for (int i = 0; i < 256; i++)
		//cout << valores[i] << " ";
	//cout << endl;

	//Cargar nueva imagen
	Mat imagen3 = imread("woodPrueba.jpg", IMREAD_COLOR);
	Mat CIELab3;
	Mat gris3;
	

	cvtColor(imagen3, CIELab3, COLOR_BGR2Lab);
	cvtColor(CIELab3, gris3, COLOR_BGR2GRAY);
	resize(gris3, gris3, Size(626, 417));
	resize(CIELab3, CIELab3, Size(626, 417));

	vector<int> histograma3 = lbp->lbp(gris3);

	//Obtener la distancia chi cuadrada de la nueva imagen en relacion a las dos clases
	double chi1 = chiDistance(hClase1, histograma3);
	double chi2 = chiDistance(hClase2, histograma3);

	if (chi1 < chi2)
		cout << "CHI La imagen pertenece a la clase 1" << endl;
	else
		cout << "CHI La imagen pertenece a la clase 2" << endl;

	cout << endl;

	double euclidean1 = euclideanDistance(hClase1, histograma3);
	double euclidean2 = euclideanDistance(hClase2, histograma3);
	
	if (euclidean1 < euclidean2)
		cout << "EUCLIDEAN La imagen pertenece a la clase 1" << endl;
	else
		cout << "EUCLIDEAN La imagen pertenece a la clase 2" << endl;

	imshow("CIELab 1", CIELab);
	imshow("CIELab 2", CIELab2);
	imshow("CIELab 3", CIELab3);

	waitKey(0);
	delete lbp;
	destroyAllWindows();
	return 0;
}

//Medir la distancia CHI Cuadrada
double chiDistance(vector<int> hClase, vector<int> hImagen) {
	double value = 0.0;
	for (int i = 0; i < 256; i++) {
		double divisor = hClase[i] + hImagen[i];
		if (divisor == 0)
			break;
		value += pow((hClase[i] - hImagen[i]), 2.0) / divisor;
	}
	return value;
}

//Medir la distancia Euclidea
double euclideanDistance(vector<int> hClase, vector<int> hImagen) {
	double value = 0.0;
	for (int i = 0; i < 256; i++) {
		double a = hClase[i] - hImagen[i];
		value += pow(a, 2.0);
	}
	return value;
}

void WriteFile(vector<int> histograma, string nombreArchivo) {

	ofstream archivo;
	archivo.open("Histogramas/"+ nombreArchivo, ios::trunc);

	for (int i = 0; i < 256; i++) {
		archivo << histograma[i] << " ";
		//cout << histograma[i] << "|";
	}

	archivo << "\n";
	archivo.close();

}

vector<int> ReadFile(string nombreArchivo) {

	//Abrir el archivo
	ifstream inFile;
	inFile.open("Histogramas/"+ nombreArchivo);

	vector<int> valores(256);
	int x;

	if (!inFile) {
		cerr << "Error al abrir el archivo";
		exit(1);   // call system to stop
	}

	int cont = 0;
	while (inFile >> x) {
		valores[cont] = x;
		cont += 1;
	}

	inFile.close();

	return valores;
	
}