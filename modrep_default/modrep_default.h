

/**@mainpage
 *
 * This is the default repository of modules distributed with the ALOE source code.
 *
 * This repository contains a basic source and sink modules and a set of templates to facilitate
 * the generation of new DSP modules: the template and template_plain modules. The template_plain
 * uses the OESR API directly and shall be used only by experienced users or by those modules requiring
 * some special functionalities.
 *
 * Most of the modules can use the OESR SKELETON template for generating new modules. This template has
 * the following features:
 *  * Automatic interface generation
 *  * Interface for easy initialization parameters management
 *  * The user only codes two functions: initialize() and work()
 *  * Automatic generation of a <b>standalone</b> program for debugging and a
 *  <b>MATLAB/Octave MEX-file</b> for code verification.
 *
 * For a step-by-step guide on how to create a new module, visit the project web-site
 * https://github.com/flexnets/aloe/wiki/
 *
 *  More information about the template is the module documentation, under the Module section of
 *  this page
 */
