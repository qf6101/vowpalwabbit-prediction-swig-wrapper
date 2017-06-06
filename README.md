# A Belief Description

This project wraps c++11 implemented linear model prediction and daemon (timing update). It assumes a linear model has been trained by learning algorithms of [Vowpal Wabbit](https://github.com/JohnLangford/vowpal_wabbit/wiki) (e.g., FTRL, BFGS). One straightforward application is online module of click-through rate prediction.

# Support Swig Interface

I use [Swig](http://www.swig.org/) as an interface generator. Hence a wide range of languages can use this wrapper. An example of Java invoking the model daemon and prediction is given in the swig folder.

# Support Vowpal Wabbit Hashing

The project uses the same feature hashing algorithms as Vowpal Wabbit. This means if you specified the same namespace and feature name as the training processes do, it will encode the feature into the exactly same hash code. The hashing of quadratic features is also supported. 

# Prerequisites

Require Eigen (>= 3.3.3), Boost (>= 1.64.0) and googletest (>= 1.8.0).

Copy the include directories to /usr/local/include/ and the libraries to /usr/local/lib/ in advance.

# Build Project

```
mkdir ${PROJECT_SOURCE_DIR}/build
cd ${PROJECT_SOURCE_DIR}/build
cmake ../ && make -j 1
cmake ../ && make -j 1 #build again since java swig files were generated in the first round and needed in this round
make test
```