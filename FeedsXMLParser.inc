<?php
// $Id$

/**
 * Parses a given file as an XML document.
 */
class FeedsXMLParser extends FeedsParser {

  /**
   * Implementation of FeedsParser::parse().
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {
    if (!function_exists('simplexml_load_file')) {
      throw new Exception(t('Feeds XML Parser requires SimpleXML PHP extension.'));
    }
    if (empty($this->config['xpath'])) {
      throw new Exception(t('Please set xpath for items in XML parser settings.'));
    }
    $file = realpath($batch->getFilePath());
    if (!is_file($file)) {
      throw new Exception(t('File %name not found.', array('%name' => $batch->getFilePath())));
    }
    $xml = @simplexml_load_file($file);
    if ($xml === FALSE) {
      throw new Exception(t('Could not parse XML file: %file', array('%file' => $batch->getFilePath())));
    }

    if (version_compare(PHP_VERSION, '5.2.0', '>=')) {
      //Fetch all namespaces
      $namespaces = $xml->getNamespaces(true);

      //Register them with their prefixes
      foreach ($namespaces as $prefix => $ns) {
        $xml->registerXPathNamespace($prefix, $ns);
      }
    }

    $result = $xml->xpath($this->config['xpath']);
    unset($xml);
    if (!is_array($result)) {
      throw new Exception(t('Xpath %xpath in XML file %file failed.', array('%xpath' => $this->config['xpath'], '%file' => $batch->getFilePath())));
    }
    // Convert each item to XML as SimpleXMLElement cannot be serialized
    foreach ($result as &$item) {
      $item = $item->asXML();
    }
    $batch->setItems($result);
  }

  function getSourceElement($item, $element_key) {
    $xml = new SimpleXMLElement($item);
    $result = $xml->xpath($element_key);
    if (is_array($result)) {
      $values = array();
      foreach ($result as $obj) {
        $values[] = (string)$obj;
      }
      if (count($values) > 1) {
        return $values;
      }
      elseif (count($values) == 1) {
        return $values[0];
      }
    }
    return '';
  }

  public function configDefaults() {
    return array(
      'xpath' => 'item',
    );
  }

  public function configForm(&$form_state) {
    $form = array();
    $form['xpath'] = array(
      '#type' => 'textfield',
      '#title' => t('XPath'),
      '#default_value' => $this->config['xpath'],
      '#description' => t('XPath to locate items.'),
    );
    return $form;
  }
}
