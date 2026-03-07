# Manual Setup

Install the media player firmware via the ESPHome dashboard instead of the web installer. This gives you full control over substitutions and lets you customise behaviour that the web installer leaves at defaults.

## Prerequisites

- [ESPHome](https://esphome.io/) running (as a Home Assistant add-on or standalone)
- Your device connected via USB for the first flash (OTA updates work after that)

## Create a configuration

In the ESPHome dashboard, create a new YAML configuration for your device. Use one of the examples below as a starting point.

### ESP32-S3 4848S040 (4")

```yaml
substitutions:
  name: "music-dashboard"
  friendly_name: "Music Dashboard"
  # ha_port: "8123"  # Home Assistant port (change if HA runs on a non-standard port)

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

packages:
  music_dashboard:
    url: https://github.com/jtenniswood/esphome-media-player
    files: [guition-esp32-s3-4848s040/packages.yaml]
    ref: main
    refresh: 1s
```

### ESP32-P4 JC8012P4A1 (10.1")

```yaml
substitutions:
  name: "music-dashboard-10inch"
  friendly_name: "Music Dashboard 10inch"
  # ha_port: "8123"  # Home Assistant port (change if HA runs on a non-standard port)

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

packages:
  music_dashboard:
    url: https://github.com/jtenniswood/esphome-media-player
    files: [guition-esp32-p4-jc8012p4a1/packages.yaml]
    ref: main
    refresh: 1s
```

## Available substitutions

These substitutions can be added to the `substitutions:` block in your configuration to override the defaults.

| Substitution | Default | Description |
|---|---|---|
| `name` | — | Device name used on your network (required) |
| `friendly_name` | — | Display name shown in Home Assistant (required) |
| `media_player` | `""` | Entity ID of your primary media player (configured in HA after first boot) |
| `tv_media_player` | `""` | Entity ID of a secondary TV media player (optional) |
| `ha_port` | `"8123"` | Port that Home Assistant is running on |

The ESP32-S3 4848S040 also supports rotation substitutions (`display_rotation`, `touch_mirror_x`, `touch_mirror_y`) — see the comments in `packages.yaml` for values.

## Non-standard Home Assistant port

By default the device constructs artwork URLs using port `8123`. If your Home Assistant instance runs on a different port (for example port `80` behind a reverse proxy), album art will fail to load.

To fix this, uncomment and change the `ha_port` substitution in your configuration:

```yaml
substitutions:
  name: "music-dashboard"
  friendly_name: "Music Dashboard"
  ha_port: "80"
```

The device builds artwork URLs as `http://<home-assistant-ip>:<ha_port>/api/...`, so the port must match whatever port Home Assistant is reachable on from the device's network.

::: tip
Users who installed via the web installer do not need this setting — it defaults to `8123`. If you need to change the port after a web install, adopt the device into your ESPHome dashboard and add the `ha_port` substitution to the generated configuration.
:::
