#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class DecisionTree {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        if (x[0] <= 39.85294151306152) {
                            if (x[1] <= 28.5) {
                                if (x[2] <= 33.5) {
                                    return 1;
                                }

                                else {
                                    return 0;
                                }
                            }

                            else {
                                return 1;
                            }
                        }

                        else {
                            if (x[2] <= 34.5) {
                                if (x[0] <= 49.901960372924805) {
                                    return 1;
                                }

                                else {
                                    return 0;
                                }
                            }

                            else {
                                return 0;
                            }
                        }
                    }

                    /**
                    * Predict readable class name
                    */
                    const char* predictLabel(float *x) {
                        return idxToLabel(predict(x));
                    }

                    /**
                    * Convert class idx to readable name
                    */
                    const char* idxToLabel(uint8_t classIdx) {
                        switch (classIdx) {
                            case 0:
                            return "NO_RIEGO";
                            case 1:
                            return "RIEGO";
                            default:
                            return "Houston we have a problem";
                        }
                    }

                protected:
                };
            }
        }
    }