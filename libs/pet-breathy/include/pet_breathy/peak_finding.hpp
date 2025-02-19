#pragma once

#include <optional>
#include <vector>

namespace pb {

struct peaks_data {
    // In scipy, peaks and midpoints mean the same thing
    std::vector<size_t> peaks;
    std::vector<size_t> left_edges;
    std::vector<size_t> right_edges;

    std::vector<bool> keep;
    std::vector<size_t> dist_idxs;
};

/* Reference:
 *
 * https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.find_peaks.html
 * https://github.com/scipy/scipy/blob/main/scipy/signal/_peak_finding.py#L729
 *
def find_peaks(x, height=None, threshold=None, distance=None,
               prominence=None, width=None, wlen=None, rel_height=0.5,
               plateau_size=None):
*/
void FindPeaks(const std::vector<float> &x, peaks_data &data, const std::optional<float> height = std::nullopt,
               const std::optional<float> distance = std::nullopt);

} // namespace pb
