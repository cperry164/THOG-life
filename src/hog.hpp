#ifndef __HOG_H_
#define __HOG_H_

#include <list>
#include "svm.hpp"

/**
 * \brief Une seule cellule
 */
class Histogram
{
public:
	Histogram();

	/**
	 * \brief Calcule l'histogramme de la cellule
	 * \param norm Norme du gradient du premier pixel de la cellule
	 * \param angle Angle du gradient du premier pixel de la cellule
	 * \param col_size Saut à faire pour changer de ligne dans le tableau
	 */
	void calculate_hist(const float* norm,
						const float* angle,
						int col_size);

	float hist[9];
private:
};

/**
 * \brief Un histogramme normalisé sur 4 cellules forment un bloc
 */
class NormalizedHistogram
{
public:
	NormalizedHistogram();

	/**
	 * \brief Calcule l'histogramme normalisé du bloc
	 * \param hists un tableau de 4 histogrammes
	 */
	void calculate_normedhist(std::list<const Histogram*> hists);

	float hist[36];
	float l2hys;
};

/**
 * \brief Une fenêtre contient 7 blocs de large par 15 de haut
 */
class HogWindow
{
public:
	HogWindow();

	/**
	 * \brief batit la SVM à partir d'un ensemble d'histogrammes normalisés
	 */
	void build_svm(std::list<const NormalizedHistogram*> nhists);

	/**
	 * \detecte la présence d'un objet à partir d'une SVM de référence
	 */
	bool detect(SVM* reference);

	// liste de 105 histogrammes normalisées
	std::list<const NormalizedHistogram*> nhists;
};

class HOG {
public:
	HOG(char* image, int row_size, int col_size);
	~HOG();

	/**
	 * \brief Calcul des tableaux xGrad,yGrad et norm,angle représentant
	 * 		  respectivement le gradient en coordonnées cartésiennes et
	 * 		  polaires
	 */
	void calculate_gradient();

	/**
	 * \brief Rempli le tableau cells qui contient l'histogramme de
	 *		  chacune des cellules
	 */
	void calculate_cells();

	/**
	 * \brief Calculs les blocs normalisées
	 */
	void calculate_blocks();

	/**
	 * \brief Calcul la presence d'un objet selon une svm de reference
	 */
	void calculate_windows(SVM* reference);

	//Nombre de ligne de l'image (direction y)
	unsigned int row_size;

	//Nombre de colonne de l'image (direction x)
	unsigned int col_size;

	//Taille de l'image en pixel
	unsigned int img_size;

	//Nombre de cellules en x,y et total de l'image
	unsigned int cell_size_y;
	unsigned int cell_size_x;
	unsigned int cell_size;

	//Nombre de blocs en x,y et total de l'image
	unsigned int block_size_y;
	unsigned int block_size_x;
	unsigned int block_size;

	//Nombre de fenêtres en x,y et total de l'image
	unsigned int win_size_y;
	unsigned int win_size_x;
	unsigned int win_size;

	char* image;
	char *xGrad, *yGrad;
	float *norm, *angle;
	Histogram *cells;
	NormalizedHistogram* blocks;
private:

};



#endif
