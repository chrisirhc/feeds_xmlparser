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
    $file = realpath($batch->getFilePath());
    if (!is_file($file)) {
      throw new Exception(t('File %name not found.', array('%name' => $batch->getFilePath())));
    }
    $xml = @simplexml_load_file($file);
    if ($xml === FALSE) {
      throw new Exception(t('Could not parse XML file: %file', array('%file' => $batch->getFilePath())));
    }
    if (!empty($this->config['xpath'])) {
        $xml = $xml->xpath($this->config['xpath']);
    }
    $xml = (array)$xml;
    // Convert each item to XML as SimpleXMLElement cannot be serialized
    foreach ($xml as &$item) {
      $item = $item->asXML();
    }
    $batch->setItems($xml);
  }

  public function getSourceElement($item, $element_key) {
    $xml = new SimpleXMLElement($item);
    $xml = $xml->xpath($element_key);

    if (is_array($xml)) return (string) $xml[0];
    // Return the first scalar value found after applying xpath
    /* I may not know why this is being used
    if (is_array($xml)) {
      foreach ((array)$xml[0] as $value) {
        if (is_scalar($value)) return $value;
      }
    }
    */
    return '';
  }

  public function configDefaults() {
    return array(
      'xpath' => '',
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
