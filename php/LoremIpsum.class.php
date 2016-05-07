<?php
class LoremIpsumGenerator {
	/**
	*	Copyright (c) 2009, Mathew Tinsley (tinsley@tinsology.net)
	*	All rights reserved.
	*
	*	Redistribution and use in source and binary forms, with or without
	*	modification, are permitted provided that the following conditions are met:
	*		* Redistributions of source code must retain the above copyright
	*		  notice, this list of conditions and the following disclaimer.
	*		* Redistributions in binary form must reproduce the above copyright
	*		  notice, this list of conditions and the following disclaimer in the
	*		  documentation and/or other materials provided with the distribution.
	*		* Neither the name of the organization nor the
	*		  names of its contributors may be used to endorse or promote products
	*		  derived from this software without specific prior written permission.
	*
	*	THIS SOFTWARE IS PROVIDED BY MATHEW TINSLEY ''AS IS'' AND ANY
	*	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	*	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	*	DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
	*	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	*	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	*	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	*	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	*	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	*	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	*/

	private $words, $wordsPerParagraph, $wordsPerSentence;

	function __construct($wordsPer = 100)
	{
		$this->wordsPerParagraph = $wordsPer;
		$this->wordsPerSentence = 24.460;
		$this->words = array(
		'a',
		'ac',
		'accumsan',
		'ad',
		'adipiscing',
		'aenean',
		'aenean',
		'aliquam',
		'aliquet',
		'amet',
		'ante',
		'aptent',
		'arcu',
		'at',
		'auctor',
		'augue',
		'bibendum',
		'blandit',
		'class',
		'commodo',
		'condimentum',
		'congue',
		'consectetur',
		'consequat',
		'conubia',
		'convallis',
		'cras',
		'cubilia',
		'curabitur',
		'curae',
		'cursus',
		'dapibus',
		'diam',
		'dictum',
		'dictumst',
		'dolor',
		'donec',
		'dui',
		'duis',
		'egestas',
		'eget',
		'eleifend',
		'elementum',
		'elit',
		'enim',
		'erat',
		'eros',
		'est',
		'et',
		'etiam',
		'eu',
		'euismod',
		'facilisis',
		'fames',
		'faucibus',
		'felis',
		'fermentum',
		'feugiat',
		'fringilla',
		'fusce',
		'gravida',
		'habitant',
		'habitasse',
		'hac',
		'hendrerit',
		'himenaeos',
		'iaculis',
		'id',
		'imperdiet',
		'in',
		'inceptos',
		'integer',
		'interdum',
		'ipsum',
		'justo',
		'lacinia',
		'lacus',
		'laoreet',
		'lectus',
		'leo',
		'libero',
		'ligula',
		'litora',
		'lobortis',
		'lorem',
		'luctus',
		'maecenas',
		'magna',
		'malesuada',
		'massa',
		'mattis',
		'mauris',
		'metus',
		'mi',
		'molestie',
		'mollis',
		'morbi',
		'nam',
		'nec',
		'neque',
		'netus',
		'nibh',
		'nisi',
		'non',
		'nostra',
		'nulla',
		'nullam',
		'nunc',
		'odio',
		'orci',
		'ornare',
		'pellentesque',
		'per',
		'pharetra',
		'phasellus',
		'placerat',
		'platea',
		'porta',
		'porttitor',
		'posuere',
		'potenti',
		'praesent',
		'pretium',
		'primis',
		'proin',
		'pulvinar',
		'purus',
		'quam',
		'quis',
		'quisque',
		'rhoncus',
		'risus',
		'rutrum',
		'sagittis',
		'sapien',
		'scelerisque',
		'sed',
		'sem',
		'semper',
		'senectus',
		'sit',
		'sociosqu',
		'sodales',
		'sollicitudin',
		'suscipit',
		'suspendisse',
		'taciti',
		'tellus',
		'tempor',
		'tempus',
		'tincidunt',
		'torquent',
		'tortor',
		'tristique',
		'turpis',
		'ullamcorper',
		'ultrices',
		'ultricies',
		'urna',
		'ut',
		'ut',
		'varius',
		'vehicula',
		'vel',
		'velit',
		'venenatis',
		'vestibulum',
		'vitae',
		'vivamus',
		'viverra',
		'volutpat',
		'vulputate',
		);
	}

	function getContent($count, $format = 'html', $loremipsum = true)
	{
		$format = strtolower($format);

		if($count <= 0)
			return '';

		switch($format)
		{
			case 'txt':
				return $this->getText($count, $loremipsum);
			case 'plain':
				return $this->getPlain($count, $loremipsum);
			default:
				return $this->getHTML($count, $loremipsum);
		}
	}

	private function getWords(&$arr, $count, $loremipsum)
	{
		$i = 0;
		if($loremipsum)
		{
			$i = 2;
			$arr[0] = 'lorem';
			$arr[1] = 'ipsum';
		}

		for($i; $i < $count; $i++)
		{
			$index = array_rand($this->words);
			$word = $this->words[$index];
			//echo $index . '=>' . $word . '<br />';

			if($i > 0 && $arr[$i - 1] == $word)
				$i--;
			else
				$arr[$i] = $word;
		}
	}

	private function getPlain($count, $loremipsum, $returnStr = true)
	{
		$words = array();
		$this->getWords($words, $count, $loremipsum);
		//print_r($words);

		$delta = $count;
		$curr = 0;
		$sentences = array();
		while($delta > 0)
		{
			$senSize = $this->gaussianSentence();
			//echo $curr . '<br />';
			if(($delta - $senSize) < 4)
				$senSize = $delta;

			$delta -= $senSize;

			$sentence = array();
			for($i = $curr; $i < ($curr + $senSize); $i++)
				$sentence[] = $words[$i];

			$this->punctuate($sentence);
			$curr = $curr + $senSize;
			$sentences[] = $sentence;
		}

		if($returnStr)
		{
			$output = '';
			foreach($sentences as $s)
				foreach($s as $w)
					$output .= $w . ' ';

			return $output;
		}
		else
			return $sentences;
	}

	private function getText($count, $loremipsum)
	{
		$sentences = $this->getPlain($count, $loremipsum, false);
		$paragraphs = $this->getParagraphArr($sentences);

		$paragraphStr = array();
		foreach($paragraphs as $p)
		{
			$paragraphStr[] = $this->paragraphToString($p);
		}

		$paragraphStr[0] = "\t" . $paragraphStr[0];
		return implode("\n\n\t", $paragraphStr);
	}

	private function getParagraphArr($sentences)
	{
		$wordsPer = $this->wordsPerParagraph;
		$sentenceAvg = $this->wordsPerSentence;
		$total = count($sentences);

		$paragraphs = array();
		$pCount = 0;
		$currCount = 0;
		$curr = array();

		for($i = 0; $i < $total; $i++)
		{
			$s = $sentences[$i];
			$currCount += count($s);
			$curr[] = $s;
			if($currCount >= ($wordsPer - round($sentenceAvg / 2.00)) || $i == $total - 1)
			{
				$currCount = 0;
				$paragraphs[] = $curr;
				$curr = array();
				//print_r($paragraphs);
			}
			//print_r($paragraphs);
		}

		return $paragraphs;
	}

	private function getHTML($count, $loremipsum)
	{
		$sentences = $this->getPlain($count, $loremipsum, false);
		$paragraphs = $this->getParagraphArr($sentences);
		//print_r($paragraphs);

		$paragraphStr = array();
		foreach($paragraphs as $p)
		{
			$paragraphStr[] = "<p>\n" . $this->paragraphToString($p, true) . '</p>';
		}

		//add new lines for the sake of clean code
		return implode("\n", $paragraphStr);
	}

	private function paragraphToString($paragraph, $htmlCleanCode = false)
	{
		$paragraphStr = '';
		foreach($paragraph as $sentence)
		{
			foreach($sentence as $word)
				$paragraphStr .= $word . ' ';

			if($htmlCleanCode)
				$paragraphStr .= "\n";
		}
		return $paragraphStr;
	}

	/*
	* Inserts commas and periods in the given
	* word array.
	*/
	private function punctuate(& $sentence)
	{
		$count = count($sentence);
		$sentence[$count - 1] = $sentence[$count - 1] . '.';

		if($count < 4)
			return $sentence;

		$commas = $this->numberOfCommas($count);

		for($i = 1; $i <= $commas; $i++)
		{
			$index = (int) round($i * $count / ($commas + 1));

			if($index < ($count - 1) && $index > 0)
			{
				$sentence[$index] = $sentence[$index] . ',';
			}
		}
	}

	/*
	* Determines the number of commas for a
	* sentence of the given length. Average and
	* standard deviation are determined superficially
	*/
	private function numberOfCommas($len)
	{
		$avg = (float) log($len, 6);
		$stdDev = (float) $avg / 6.000;

		return (int) round($this->gauss_ms($avg, $stdDev));
	}

	/*
	* Returns a number on a gaussian distribution
	* based on the average word length of an english
	* sentence.
	* Statistics Source:
	*	http://hearle.nahoo.net/Academic/Maths/Sentence.html
	*	Average: 24.46
	*	Standard Deviation: 5.08
	*/
	private function gaussianSentence()
	{
		$avg = (float) 24.460;
		$stdDev = (float) 5.080;

		return (int) round($this->gauss_ms($avg, $stdDev));
	}

	/*
	* The following three functions are used to
	* compute numbers with a guassian distrobution
	* Source:
	* 	http://us.php.net/manual/en/function.rand.php#53784
	*/
	private function gauss()
	{   // N(0,1)
		// returns random number with normal distribution:
		//   mean=0
		//   std dev=1

		// auxilary vars
		$x=$this->random_0_1();
		$y=$this->random_0_1();

		// two independent variables with normal distribution N(0,1)
		$u=sqrt(-2*log($x))*cos(2*pi()*$y);
		$v=sqrt(-2*log($x))*sin(2*pi()*$y);

		// i will return only one, couse only one needed
		return $u;
	}

	private function gauss_ms($m=0.0,$s=1.0)
	{
		return $this->gauss()*$s+$m;
	}

	private function random_0_1()
	{
		return (float)rand()/(float)getrandmax();
	}

}