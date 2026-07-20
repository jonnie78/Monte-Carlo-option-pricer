//Defining struct so that put and call price can be calculated in one function
struct OptionPrices {
    double call_price;
    double put_price;
    double call_err;
    double put_err;
};

//Struct that allows thread worker function to output all partical sums
struct PartialSums {
    double call_sum;
    double call_sum_sq;
    double put_sum;
    double put_sum_sq;
};
