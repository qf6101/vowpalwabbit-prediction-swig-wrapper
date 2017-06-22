# A Belief Description

This project wraps online prediction and timing update of vw trained linear model. It assumes a linear model has been trained by learning algorithms of [Vowpal Wabbit](https://github.com/JohnLangford/vowpal_wabbit/wiki) (e.g., FTRL, BFGS). One straightforward application is online module of click-through rate prediction. 

# Support Swig Interface

We use [Swig](http://www.swig.org/) as an interface generator. Hence a wide range of languages can use this wrapper. An example of Java invoking the model daemon and prediction is given in the swig folder.

# Support Vowpal Wabbit Feature Hash

The project uses the same feature hash algorithms as Vowpal Wabbit (i.e., murmurhash3). This means if you specified the same namespace and feature name as the training processes do, it will encode the feature into exactly the same hash code. The hash of quadratic features is also supported. 

# Prerequisites

Require Eigen (>= 3.3.3), Boost (>= 1.64.0) spdlog (>= 0.13.0), and googletest (>= 1.8.0).

Copy the include directories to /usr/local/include/ and the libraries to /usr/local/lib/ in advance.

# Build Project

```
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build  # build again since java swig files were generated in the first round and needed in this round
make test
./build/test/pctr_test  # run test cases
```