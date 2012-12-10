

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
 *  To create a new module, just follow this step-by-step guide:
 *
 *  <h2> Generation </h2>
 *  1) Copy and rename the modrep_default/template directory to your module's repository (or use the default directory)
 *
 *  2) Rename the files inside the src/ directory
 *
 *  3) Write the initialize() and work() functions, as illustrated in the template.c example.
 *
 *  4) If you have ALOE installed in your system, you can compile the module (using cmake) from
 *  its directory, otherwise go to the root aloe directory and run cmake, compile and install.
 *
 *  5) When you install a module, the following files are installed in the system
 *  (by default, to /usr/local/bin):
 *  	* An executable: /usr/local/bin/modulename
 *  	* A MEX-file: /usr/local/mex/am_modulename
 *  	* A shared library: /usr/local/lib/libmodulename.so
 *
 *
 *  <h2> Debugging </h2>
 *  The executable is useful in this phase to carry out the initial debugging.
 *  Just use your favorite debugger or IDE to debug and ensure the correct functionality of the
 *  module under any circumstance.
 *
 *  TODO: Parameters passed to the executable, using gnuplot, ...
 *
 *  <h2> Verification with MATLAB/Octave</h2>
 *  TODO
 *
 *  <h2> Integration in ALOE++</h2>
 *  TODO
 *
 *  More information about the template is the module documentation, under the Module section of
 *  this page
 */
