/**
 * \file Aleatorio.hpp
 * \brief Declaraci&oacute;n de la funcionalidad para generar n&uacute;meros aleatorios.
 * \date 10/02/2026
 */

#ifndef ALEATORIO_HPP_INCLUDED
#define ALEATORIO_HPP_INCLUDED

/**
 * \brief Genera un n&uacute;mero entero aleatorio dentro de un rango espec&iacute;fico.
 *
 * Utiliza la funci&oacute;n rand() del sistema para obtener un valor pseudoaleatorio.
 *
 * \param inicio L&iacute;mite inferior del rango.
 * \param fin L&iacute;mite superior del rango.
 * \return Un n&uacute;mero entero aleatorio entre [inicio, fin].
 */
int aleatorio(int inicio, int fin);

#endif // ALEATORIO_HPP_INCLUDED
