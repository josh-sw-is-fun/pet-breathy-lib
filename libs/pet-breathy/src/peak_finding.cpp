#include "pet_breathy/peak_finding.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

namespace {

void _local_maxima_1d(const std::vector<float> &x, pb::peaks_data &data);

void _select_by_peak_distance(const std::vector<float> &x, const std::vector<size_t> &peaks_lookup,
                              const float distance, pb::peaks_data &data);

void _select_by_property(const std::vector<float> &x, const std::vector<size_t> &peaks_lookup,
                         const std::optional<float> pmin, const std::optional<float> pmax,
                         pb::peaks_data &data);

void _sort_indexes(const std::vector<float> &vals, const std::vector<size_t> &lookup, pb::peaks_data &data);

} // namespace

namespace pb {

/*
Options used:
- height
- distance
properties = {}

Finding peaks with distance only is much slower than not using distance and distance + height
*/
void FindPeaks(const std::vector<float> &x, peaks_data &data, const std::optional<float> height,
               const std::optional<float> distance) {

    // Ensure argument :None:None:`x` is a 1-D C-contiguous array of
    // dtype('float64'). > x = _arg_x_as_expected(x)

    // > if distance is not None and distance < 1:
    // >    raise ValueError('`distance` must be greater or equal to 1')

    // https://github.com/scipy/scipy/blob/main/scipy/signal/_peak_finding.py#L943
    // https://github.com/scipy/scipy/blob/main/scipy/signal/_peak_finding_utils.pyx#L20
    // > peaks, left_edges, right_edges = _local_maxima_1d(x)
    _local_maxima_1d(x, data);

    /*
    if plateau_size is not None:
        # Evaluate plateau size
        plateau_sizes = right_edges - left_edges + 1
        pmin, pmax = _unpack_condition_args(plateau_size, x, peaks)
        keep = _select_by_property(plateau_sizes, pmin, pmax)
        peaks = peaks[keep]
        properties["plateau_sizes"] = plateau_sizes
        properties["left_edges"] = left_edges
        properties["right_edges"] = right_edges
        properties = {key: array[keep] for key, array in properties.items()}
    */

    // > if height is not None:
    if (height.has_value()) {
        // > #Evaluate height condition

        // > peak_heights = x[peaks]
        // x[peaks]:
        // - x is a buffer of floats
        // - peaks is buffer of size_t or position of peak locations in x
        //
        // x[peaks] returns a new ArrayLike object in numpy that contains x
        // values corresponding to the peaks
        //
        // Instead of creating a new vector that contains the values of x at
        // indexes data.midpoints, pass x and data.midpoints along, where
        // data.midpoints are the lookups for the values in x.

        // > hmin, hmax = _unpack_condition_args(height, x, peaks)
        // void _unpack_condition_args(
        //      const float interval,
        //      const float* x, const size_t x_len,
        //      const std::vector<size_t> &peaks_lookup);
        // const float hmin = _unpack_condition_args(height.value(), x, x_len,
        // data.midpoints);
        //
        // I'm calling _unpack_condition_args with a single value, not a
        // sequence or tuple. It should return height, None hmin = height hmax =
        // None
        const std::optional<float> hmin = height;
        const std::optional<float> hmax = std::nullopt;

        _select_by_property(x, data.peaks, hmin, hmax, data);

        // > peaks = peaks[keep]
        size_t keep_count = 0;
        for (size_t i = 0; i < data.peaks.size(); i++) {
            if (data.keep[i]) {
                data.peaks[keep_count] = data.peaks[i];
                keep_count++;
            }
        }
        data.peaks.resize(keep_count);

        // > properties["peak_heights"] = peak_heights
        // > properties = {key: array[keep] for key, array in properties.items()}
    }

    /*
    if threshold is not None:
        # Evaluate threshold condition
        tmin, tmax = _unpack_condition_args(threshold, x, peaks)
        keep, left_thresholds, right_thresholds = _select_by_peak_threshold(
            x, peaks, tmin, tmax)
        peaks = peaks[keep]
        properties["left_thresholds"] = left_thresholds
        properties["right_thresholds"] = right_thresholds
        properties = {key: array[keep] for key, array in properties.items()}
    */

    // > if distance is not None:
    if (distance.has_value()) {
        // > #Evaluate distance condition
        // > keep = _select_by_peak_distance(peaks, x[peaks], distance)
        //
        _select_by_peak_distance(x, data.peaks, distance.value(), data);

        // > peaks = peaks[keep]
        size_t keep_count = 0;
        for (size_t i = 0; i < data.peaks.size(); i++) {
            if (data.keep[i]) {
                data.peaks[keep_count] = data.peaks[i];
                keep_count++;
            }
        }
        data.peaks.resize(keep_count);

        // > properties = {key: array[keep] for key, array in properties.items()}
    }

    /*
    if prominence is not None or width is not None:
        # Calculate prominence (required for both conditions)
        wlen = _arg_wlen_as_expected(wlen)
        properties.update(zip(
            ['prominences', 'left_bases', 'right_bases'],
            _peak_prominences(x, peaks, wlen=wlen)
        ))

    if prominence is not None:
        # Evaluate prominence condition
        pmin, pmax = _unpack_condition_args(prominence, x, peaks)
        keep = _select_by_property(properties['prominences'], pmin, pmax)
        peaks = peaks[keep]
        properties = {key: array[keep] for key, array in properties.items()}

    if width is not None:
        # Calculate widths
        properties.update(zip(
            ['widths', 'width_heights', 'left_ips', 'right_ips'],
            _peak_widths(x, peaks, rel_height, properties['prominences'],
                         properties['left_bases'],
    properties['right_bases'])
        ))
        # Evaluate width condition
        wmin, wmax = _unpack_condition_args(width, x, peaks)
        keep = _select_by_property(properties['widths'], wmin, wmax)
        peaks = peaks[keep]
        properties = {key: array[keep] for key, array in properties.items()}

     */

    // > return peaks, properties
}

} // namespace pb

namespace {

/**
 * x.shape[0] == x_len
 */
// > def _local_maxima_1d(const np.float64_t[::1] x not None):
void _local_maxima_1d(const std::vector<float> &x, pb::peaks_data &data) {
    // > cdef:
    // >     np.intp_t[::1] midpoints, left_edges, right_edges
    // >     np.intp_t m, i, i_ahead, i_max
    const size_t x_len = x.size();

    // > # Preallocate, there can't be more maxima than half the size of `x`
    // > midpoints = np.empty(x.shape[0] // 2, dtype=np.intp)
    data.peaks.resize(x_len / 2);
    // > left_edges = np.empty(x.shape[0] // 2, dtype=np.intp)
    data.left_edges.resize(x_len / 2);
    // > right_edges = np.empty(x.shape[0] // 2, dtype=np.intp)
    data.right_edges.resize(x_len / 2);
    // > m = 0  # Pointer to the end of valid area in allocated arrays
    size_t m = 0;

    // > with nogil:
    // >     i = 1  # Pointer to current sample, first one can't be maxima
    size_t i = 1;
    // >     i_max = x.shape[0] - 1  # Last sample can't be maxima
    size_t i_max = x_len - 1;
    // >     while i < i_max:
    while (i < i_max) {
        // > # Test if previous sample is smaller
        // > if x[i - 1] < x[i]:
        if (x[i - 1] < x[i]) {
            // > i_ahead = i + 1  # Index to look ahead of current sample
            size_t i_ahead = i + 1;

            // > # Find next sample that is unequal to x[i]
            // > while i_ahead < i_max and x[i_ahead] == x[i]:
            while (i_ahead < i_max && x[i_ahead] == x[i]) {
                // > i_ahead += 1
                i_ahead++;
            }

            // > # Maxima is found if next unequal sample is smaller than x[i]
            // > if x[i_ahead] < x[i]:
            if (x[i_ahead] < x[i]) {
                // > left_edges[m] = i
                data.left_edges[m] = i;
                // > right_edges[m] = i_ahead - 1
                data.right_edges[m] = i_ahead - 1;
                // > midpoints[m] = (left_edges[m] + right_edges[m]) // 2
                data.peaks[m] = (data.left_edges[m] + data.right_edges[m]) / 2;
                // > m += 1
                m++;
                // > # Skip samples that can't be maximum
                // > i = i_ahead
                i = i_ahead;
            }
        }
        // > i += 1
        i++;
    }

    // > # Keep only valid part of array memory.
    // > midpoints.base.resize(m, refcheck=False)
    data.peaks.resize(m);
    // > left_edges.base.resize(m, refcheck=False)
    data.left_edges.resize(m);
    // > right_edges.base.resize(m, refcheck=False)
    data.right_edges.resize(m);

    // > return midpoints.base, left_edges.base, right_edges.base
}

// > def _select_by_peak_distance(const np.intp_t[::1] peaks not None,
// >                              const np.float64_t[::1] priority not None,
// >                              np.float64_t distance):
void _select_by_peak_distance(const std::vector<float> &x, const std::vector<size_t> &peaks_lookup,
                              const float distance, pb::peaks_data &data) {
    // > cdef:
    // >     np.uint8_t[::1] keep
    // >     np.intp_t[::1] priority_to_position
    // >     np.intp_t i, j, k, peaks_size, distance_

    // > peaks_size = peaks.shape[0]
    const int peaks_size = static_cast<int>(peaks_lookup.size());

    // > # Round up because actual peak distance can only be natural number
    // > distance_ = <np.intp_t>ceil(distance)
    const float distance_ = std::ceil(distance);

    // > keep = np.ones(peaks_size, dtype=np.uint8)  # Prepare array of flags
    data.keep.resize(peaks_size);
    std::fill(data.keep.begin(), data.keep.end(), true);

    // > # Create map from `i` (index for `peaks` sorted by `priority`) to `j` (index
    // > # for `peaks` sorted by position). This allows to iterate `peaks` and `keep`
    // > # with `j` by order of `priority` while still maintaining the ability to
    // > # step to neighbouring peaks with (`j` + 1) or (`j` - 1).
    // > priority_to_position = np.argsort(priority)
    _sort_indexes(x, peaks_lookup, data);

    // > with nogil:
    {
        // > # Highest priority first -> iterate in reverse order (decreasing)
        // > for i in range(peaks_size - 1, -1, -1):
        for (int i = static_cast<int>(peaks_size) - 1; i >= 0; i--) {
            // > # "Translate" `i` to `j` which points to current peak whose
            // > # neighbours are to be evaluated
            // > j = priority_to_position[i]
            const int j = static_cast<int>(data.dist_idxs[i]);

            // > if keep[j] == 0:
            if (!data.keep[j]) {
                // > # Skip evaluation for peak already marked as "don't keep"
                // > continue
                continue;
            }

            // > k = j - 1
            int k = j - 1;

            // > # Flag "earlier" peaks for removal until minimal distance is exceeded
            // > while 0 <= k and peaks[j] - peaks[k] < distance_:
            while (0 <= k && peaks_lookup[j] - peaks_lookup[k] < distance_) {
                // > keep[k] = 0
                data.keep[k] = false;

                // > k -= 1
                k--;
            }

            // > k = j + 1
            k = j + 1;

            // > # Flag "later" peaks for removal until minimal distance is exceeded
            // > while k < peaks_size and peaks[k] - peaks[j] < distance_:
            while (k < peaks_size && peaks_lookup[k] - peaks_lookup[j] < distance_) {
                // > keep[k] = 0
                data.keep[k] = false;

                // > k += 1
                k++;
            }
        }
    }

    // > return keep.base.view(dtype=np.bool_)  # Return as boolean array
}

// > def _select_by_property(peak_properties, pmin, pmax):
void _select_by_property(const std::vector<float> &x, const std::vector<size_t> &peaks_lookup,
                         const std::optional<float> pmin, const std::optional<float> pmax,
                         pb::peaks_data &data) {
    const size_t peaks_lookup_size = peaks_lookup.size();

    // > keep = np.ones(peak_properties.size, dtype=bool)
    data.keep.resize(peaks_lookup_size);
    std::fill(data.keep.begin(), data.keep.end(), true);

    // > if pmin is not None:
    if (pmin.has_value()) {
        // > keep &= (pmin <= peak_properties)
        for (size_t i = 0; i < peaks_lookup_size; i++) {
            data.keep[i] = pmin <= x[peaks_lookup[i]];
        }
    }

    // > if pmax is not None:
    if (pmax.has_value()) {
        // > keep &= (peak_properties <= pmax)
        for (size_t i = 0; i < peaks_lookup_size; i++) {
            data.keep[i] = x[peaks_lookup[i]] <= pmax;
        }
    }

    // > return keep
}

void _sort_indexes(const std::vector<float> &vals, const std::vector<size_t> &lookup, pb::peaks_data &data) {
    data.dist_idxs.resize(lookup.size());
    std::iota(data.dist_idxs.begin(), data.dist_idxs.end(), 0);

    std::stable_sort(data.dist_idxs.begin(), data.dist_idxs.end(),
                     [&vals, &lookup](size_t i1, size_t i2) { return vals[lookup[i1]] < vals[lookup[i2]]; });
}

} // namespace
