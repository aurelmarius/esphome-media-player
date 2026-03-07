# Speaker Grouping

Group and ungroup multi-room speakers directly from the touchscreen. A speaker icon appears on the main screen — tap it to open a panel listing all your speakers with toggle switches to group or ungroup them.

This works with any speaker platform that supports grouping in Home Assistant (Sonos, Google Cast, etc.).

## Setup

Speaker grouping requires a **template sensor** in Home Assistant that provides the list of available speakers. The controller reads this sensor to populate the grouping panel.

### 1. Create a template sensor

Add the following to your Home Assistant `configuration.yaml` (or a template sensor via the UI):

```yaml
template:
  - sensor:
      - name: "Media Player Group Members"
        state: "ok"
        attributes:
          entities: >
            {{ states.media_player
               | selectattr('attributes.supported_features', 'defined')
               | map(attribute='entity_id')
               | list }}
```

Restart Home Assistant after adding the sensor.

### 2. Configure the device

On the device page in Home Assistant (**Settings → Devices & Services → ESPHome** → your device), set the **Speaker Group Sensor** field to the entity ID of the template sensor you created (e.g. `sensor.media_player_group_members`).

### 3. Use it

Tap the speaker icon on the main screen to open the grouping panel. Toggle speakers on or off to group or ungroup them with the primary player.
