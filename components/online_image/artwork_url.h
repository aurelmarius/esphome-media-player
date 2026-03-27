#pragma once

#include <string>

namespace esphome {
namespace online_image {

/// Rewrite known CDN artwork URLs to cap the requested resolution and
/// force a decodable format (JPEG).
///
/// Apple Music embeds resolution in the filename in two forms:
///   1. Concrete: /3000x3000bb.jpg  (high-res, can OOM the ESP32)
///   2. Template: /{w}x{h}{c}.{f}   (HA proxy resolves to HEIC, unsupported)
///
/// This rewrites both patterns to at most max_dim and forces JPEG output.
/// URLs that don't match a known pattern are returned unchanged.
inline std::string cap_artwork_url(const std::string &url, int max_dim = 600) {
  std::string dim = std::to_string(max_dim) + "x" + std::to_string(max_dim);

  // Pattern 2: Apple Music CDN template — /{w}x{h}{c}.{f}
  // The HA media proxy often resolves {f} to HEIC which we can't decode.
  // Replace the entire template segment with a concrete JPEG request.
  auto tpl_pos = url.find("{w}x{h}");
  if (tpl_pos != std::string::npos) {
    auto slash = url.rfind('/', tpl_pos);
    if (slash != std::string::npos) {
      return url.substr(0, slash + 1) + dim + "bb.jpg";
    }
  }

  // Pattern 1: Apple Music CDN concrete — /{W}x{H}bb.{ext}
  auto bb_pos = url.rfind("bb.");
  if (bb_pos == std::string::npos)
    return url;

  auto x_pos = url.rfind('x', bb_pos);
  if (x_pos == std::string::npos || x_pos == 0)
    return url;

  auto slash_pos = url.rfind('/', x_pos);
  if (slash_pos == std::string::npos)
    return url;

  std::string w_str = url.substr(slash_pos + 1, x_pos - slash_pos - 1);
  std::string h_str = url.substr(x_pos + 1, bb_pos - x_pos - 1);

  if (w_str.empty() || h_str.empty())
    return url;
  for (char c : w_str) {
    if (c < '0' || c > '9') return url;
  }
  for (char c : h_str) {
    if (c < '0' || c > '9') return url;
  }

  int w = std::stoi(w_str);
  int h = std::stoi(h_str);

  if (w <= max_dim && h <= max_dim)
    return url;

  return url.substr(0, slash_pos + 1) + dim + url.substr(bb_pos);
}

}  // namespace online_image
}  // namespace esphome
