#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <cmath>
#include <cstdlib>

using namespace std;

// Binomial coefficient nCk
long double binomial_coeff(int n, int k) {
    if (k < 0 || k > n) return 0.0L;
    if (k == 0 || k == n) return 1.0L;
    if (k > n - k) k = n - k;
    long double res = 1.0L;
    for (int i = 1; i <= k; ++i) {
        res *= (long double)(n - k + i);
        res /= (long double)i;
    }
    return res;
}

// Binomial distribution: returns PMF values for k = 0..n
vector<long double> binomial_distr(int n) {
    vector<long double> p(n + 1);
    for (int k = 0; k <= n; ++k) p[k] = binomial_coeff(n, k) / powl(2.0L, (long double)n);
    return p;
}

// Normal distribution: returns PDF values at integer k = 0..n
// μ = n/2, σ = sqrt(n)/2
vector<long double> normal_distr(int n) {
    vector<long double> f(n + 1);
    const long double pi = acosl(-1.0L);
    long double mu = n / 2.0L;
    long double sigma = sqrtl((long double)n) / 2.0L;
    for (int k = 0; k <= n; ++k) {
        long double exponent = -((k - mu) * (k - mu)) / (2.0L * sigma * sigma);
        f[k] = (1.0L / (sigma * sqrtl(2.0L * pi))) * expl(exponent);
    }
    return f;
}

// Mean Squared Error between two distributions
long double mean_squared_error(const vector<long double>& a, const vector<long double>& b) {
    int n = a.size();
    if (b.size() != n) {
        cerr << "Error: distributions must have the same size.\n";
        exit(1);
    }
    long double mse = 0.0L;
    for (int i = 0; i < n; ++i) {
        long double diff = a[i] - b[i];
        mse += diff * diff;
    }
    return mse / n;
}

struct ExperimentResult {
    vector<long double> observed;
    vector<long double> expected_binom;
    vector<long double> expected_normal;
    long double mse_ob_bin;
    long double mse_ob_norm;
    long double mse_bin_norm;
};

// Galton board experiment
ExperimentResult galton_board(int n, int N) {
    vector<int> counts(n + 1, 0);

    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution coin(0.5);

    // Simulate N balls
    for (int b = 0; b < N; ++b) {
        int right_moves = 0;
        for (int step = 0; step < n; ++step) {
            if (coin(gen)) ++right_moves;
        }
        ++counts[right_moves];
    }

    // Convert observed counts to probabilities
    vector<long double> p_observed(n + 1);
    for (int k = 0; k <= n; ++k) p_observed[k] = (long double)counts[k] / N;

    // Theoretical distributions
    vector<long double> p_binom = binomial_distr(n);
    vector<long double> p_normal = normal_distr(n);

    // Compute MSEs
    long double mse_ob_bin = mean_squared_error(p_observed, p_binom);
    long double mse_ob_norm = mean_squared_error(p_observed, p_normal);
    long double mse_bin_norm = mean_squared_error(p_binom, p_normal);

    return {p_observed, p_binom, p_normal, mse_ob_bin, mse_ob_norm, mse_bin_norm};
}


int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <levels n> <balls N> <repetitions x>\n";
        return 1;
    }

    int n = atoi(argv[1]);
    int N = atoi(argv[2]);
    int x = atoi(argv[3]);

    if (n <= 0 || N <= 0 || x <= 0) {
        cerr << "Both n and N must be positive integers.\n";
        return 1;
    }

    vector<long double> avg_observed(n + 1, 0.0L);
    long double sum_mse_ob_bin = 0.0L;
    long double sum_mse_ob_norm = 0.0L;
    ExperimentResult r;
    for (int i = 0; i < x; ++i) {
        r = galton_board(n, N);
        for (int j = 0; j <= n; ++j) avg_observed[j] += r.observed[j];
        sum_mse_ob_bin += r.mse_ob_bin;
        sum_mse_ob_norm += r.mse_ob_norm;
    }

    for (int k = 0; k <= n; ++k) avg_observed[k] /= x;
    vector<long double> binom = r.expected_binom;
    vector<long double> norm = r.expected_normal;
    long double avg_mse_ob_bin = sum_mse_ob_bin / x;
    long double avg_mse_ob_norm = sum_mse_ob_norm / x;
    long double mse_bin_norm = r.mse_bin_norm;

    // Output main results table
    cout << "\nResults after " << N << " balls with " << n << " levels:\n\n";
    cout << setw(10) << "k"
         << setw(12) << "Observed"
         << setw(20) << "Expected(binomial)"
         << setw(20) << "Expected(normal)"
         << endl;
    cout << string(62, '-') << '\n';

    for (int k = 0; k <= n; ++k) {
        long double expected_obser = (long double)N * avg_observed[k];
        long double expected_binom = (long double)N * binom[k];
        long double expected_normal = (long double)N * norm[k];
        cout << setw(10) << k
             << setw(12) << fixed << setprecision(5) << expected_obser
             << setw(20) << fixed << setprecision(5) << expected_binom
             << setw(20) << fixed << setprecision(5) << expected_normal
             << '\n';
    }

    // Second table: MSE summary
    cout << "\nMean Squared Errors\n";
    cout << string(45, '-') << '\n';
    cout << setw(30) << left << "Observed vs Binomial:" << setw(15) << fixed << setprecision(10) << avg_mse_ob_bin << '\n';
    cout << setw(30) << left << "Observed vs Normal:"   << setw(15) << fixed << setprecision(10) << avg_mse_ob_norm << '\n';
    cout << setw(30) << left << "Binomial vs Normal:"   << setw(15) << fixed << setprecision(10) << mse_bin_norm << '\n';
    cout << string(45, '-') << '\n';
    return 0;
}