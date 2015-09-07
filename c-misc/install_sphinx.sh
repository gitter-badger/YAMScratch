#every sphinx package expects to be in the same root folder
# SPHINX_ROOT/
# |_ sphinxbase/
# |_ pocketsphinx
SPHINX_ROOT=/home/shivaebola/Documents/GitHub/speech/
#assuming we are not in SPHINX_ROOT
git clone git@github.com:cmusphinx/sphinxbase.git
git clone git@github.com:cmusphinx/pocketsphinx.git
# you will need at least bison and swig
cd sphinxbase/ 
mkdir linux-default-build
BASE_INSTALL_LOCATION=$SPHINX_ROOT/sphinxbase/linux-default-build
./autogen.sh --prefix=$BASE_INSTALL_LOCATION
make
make install
#now go and setup pocketsphinx
cd ../pocketsphinx
./autogen.sh
mkdir $SPHINX_ROOT/pocketsphinx/linux-default-build
export SPHINXBASE_LIBS=$BASE_INSTALL_LOCATION
POCKET_INSTALL_LOCATION=$SPHINX_ROOT/pocketsphinx/linux-default-build
./configure --prefix=$POCKET_INSTALL_LOCATION
make clean
make check
make install
#return to base
cd ../
#if virtualenv is not allready installed, install install
pip install virtualenv
#create a project folder to work in
mkdir project
cd project/
#create the virtual environment
virtualenv venv
source venv/bin/activate
#now install the python version of sphinx
pip install pocketsphinx
