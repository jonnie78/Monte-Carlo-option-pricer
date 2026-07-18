#include<algorithm> 
#include<cmath>
#include<random>
#include<iostream>

using namespace std;

//Defining struct so that put and call price can be calculated in one function
struct OptionPrices {
    double call_price;
    double put_price;
    double call_err;
    double put_err;
};
 //Standar normal CDF
double norm_cdf(const double& x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

//Function to calculate option prices via Black-Scholes as a benchmark
OptionPrices black_scholes_prices(const double& S, const double& X, const double& r, const double& v, const double& T) {

    double d1 = (std::log(S / X) + (r + 0.5 * v * v) * T) / (v * std::sqrt(T));
    double d2 = d1 - v * std::sqrt(T);

    OptionPrices result;
        result.call_price = S * norm_cdf(d1) - X * std::exp(-r * T) * norm_cdf(d2);
        result.put_price = X * std::exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1); 

    return result;
}

//Function to calculate option prices via monte carlo method (single thread)
OptionPrices monte_carlo_option_prices(const double& S, const double& X, const double& r, const double& v, const double& T, const int& num_sims) {

    double scaled_S = S * exp(T*(r - (0.5*v*v)));
    double future_S = 0;
    double call_payoff_sum = 0;
    double call_payoff_sq = 0;
    double put_payoff_sum = 0;
    double put_payoff_sq = 0;

    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < num_sims; i++) {
  
        double x = dist(gen); 

        future_S = scaled_S * exp(v*sqrt(T)*x);
        double call_payoff = max(future_S - X, 0.0);
        double put_payoff = max(X-future_S, 0.0);

        call_payoff_sum += call_payoff;
        call_payoff_sq += call_payoff * call_payoff;

        put_payoff_sum += put_payoff;
        put_payoff_sq += put_payoff * put_payoff;
    }

    double N = static_cast<double>(num_sims);
    double discount = exp(-r * T);

    double call_mean = call_payoff_sum / N;
    double call_variance = (call_payoff_sq / N) - (call_mean * call_mean);
    double call_err = discount * sqrt(call_variance / N);

    double put_mean = put_payoff_sum / N;
    double put_variance = (put_payoff_sq / N) - (put_mean * put_mean);
    double put_err = discount * sqrt(put_variance / N);

    OptionPrices result;
    result.call_price = call_mean * discount;
    result.put_price = put_mean * discount;
    result.call_err = call_err;
    result.put_err = put_err;

    return result;
}

int main() {
    int num_sims = 10000000;   //Number of simulated paths                                                      
    double S = 100;  //Underlying asset price                                                                                  
    double X = 100;  //Strike price                                                                                  
    double r = 0.05;   //Risk-free rate                                                                           
    double v = 0.2;    //Volatility of the underlying asset                                                           
    double T = 1.0;    //Time till expiry (years)

    OptionPrices monte_carlo_prices = monte_carlo_option_prices(S, X, r, v, T, num_sims);
    OptionPrices bs_prices = black_scholes_prices(S, X, r, v, T);
    
    cout<<"Monte Carlo Call price: "<<monte_carlo_prices.call_price<<" +/- "<<monte_carlo_prices.call_err<<endl;
    cout<<"Monte Carlo Put price: "<<monte_carlo_prices.put_price<<" +/- "<<monte_carlo_prices.put_err<<endl;
    cout<<"Black-Scholes Call price: "<<bs_prices.call_price<<endl;
    cout<<"Black-Scholes Put price: "<<bs_prices.put_price<<endl;

    return 0;
}