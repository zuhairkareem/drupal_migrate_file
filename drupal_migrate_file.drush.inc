<?php
/**
 * @file
 * Contains drupal_migrate_file_migrate.drush.inc
 */

/**
 * Implements hook_drush_command().
 *
 * Defines drush commands.
 */
function drupal_migrate_file_drush_command() {
  // Company update Dynamically
  $items['migrate-prune-configs'] = array(
    'description' => dt('Prune migrate configs.'),
    'aliases' => array('mpc'),
    'examples' => array(
      'drush migrate-prune-configs' => 'Removes migrate configuations.',
    ),
    'callback' => 'drupal_migrate_file_migrate_migrate_prune_configs',
  );

  $items['module-config-reset'] = array(
    'description' => dt('Resets default config for a given module.'),
    'arguments' => [
      'module_name' => dt('Module name'),
    ],
    'aliases' => ['mcr'],
    'required-arguments' => 1,
    'callback' => 'drupal_migrate_file_migrate_config_reset',
  );

  return $items;
}

/**
 * Callback for drush command migrate-prune-configs to prune migratin configs.
 */
function drupal_migrate_file_migrate_migrate_prune_configs() {
  //Confirmation will be Y/N when use type “y” the condition will be executed if not it will not
  if (drush_confirm(dt("Are you sure you want to remove migrate configs?"))) {
    drush_print("Removing.....");
    // Get all of the migration config names.
    $query = db_select('config', 'c')
      ->fields('c', array('name'))
      ->condition('name', db_like('migrate_plus.') . '%', 'LIKE')
      ->execute();

    $config_names = $query->fetchAll();

    // Delete each config using configFactory.
    foreach ($config_names as $config_name) {
      drush_print("..." . $config_name->name);
      \Drupal::configFactory()->getEditable($config_name->name)->delete();
    }

    drush_print("Removed succesfully");

    return TRUE;
  }
}

/**
 * Reinstall default module's config files
 */
function drupal_migrate_file_migrate_config_reset($module_name) {
  if (!in_array($module_name, array_keys(\Drupal::moduleHandler()->getModuleList()))) {
    return drush_set_error(dt('Module @module_name not found.', ['@module_name' => $module_name]));
  }
  // Initiate config installer for given module.
  \Drupal::service('config.installer')->installDefaultConfig('module', $module_name);

  drush_print('Configuration was successfully reset.');

  return TRUE;
}