

namespace x
{
    function* algorithm() {

        // the pairs of words we are going to compare
        var pairs = [
                     ["CONSERVATIONALISTS", "CONVERSATIONALISTS"],
                     ["WHIRLED", "WORLD"],
                     ["COMPLEMENT", "COMPLIMENT"],
                     ["BAZAAR", "BIZARRE"],
                     ["ACCESSARY", "ACCESSORY"],
                     ["ALGORITHMS ARE FUN", "LOGARITHMS ARE NOT"],
                     ["ASSISTANCE", "ASSISTANTS"],
                     ["ALL TOGETHER", "ALTOGETHER"],
                     ["IDENTICAL STRINGS", "IDENTICAL STRINGS"]
                     ];

        // compare each pair of words

        for (var i = 0; i < pairs.length; i += 1) {
            yield * compareWords(pairs[i][0], pairs[i][1]);
        }

        yield ({
        step: "All word pairs have been compared."
        });

        // compare two words, displaying our progress as we go
        function* compareWords(w1, w2) {

            // get bounds of surface we are displayed on
            var bounds = algo.BOUNDS.inflate(-10, -100);

            // layout is based on five rows and columns equal to the longest word
            var layout = new algo.layout.GridLayout(bounds, 5, Math.max(w1.length, w2.length));

            // display both words
            function displayWord(w, row) {
                for (var i = 0; i < w.length; i += 1) {
                    var box = layout.getBox(row, i).inflate(-4, -4);
                    new algo.render.Rectangle({
                    state: algo.render.kS_NORMAL,
                    shape: box,
                    text: w[i]
                    });
                }
            }

            displayWord(w1, 0);
            displayWord(w2, 3);

            //=start
            yield ({
            step: _.sprintf("Comparing %s against %s.", w1, w2),
            line: "start",
            variables: {
                "First Word": w1,
                "Second Word": w2
            }
            });

            // display letter pairs (bigrams) from each word and return the bigrams
            // and the element used to display it. Also set a matched flag to false,
            // this flag becomes true when matched to another bigram so that it is not
            // counted twice
            function displayPairs(w, row) {
                var bigrams = [],
                i;
                for (i = 0; i < w.length - 1; i += 1) {

                    var bigram = w.substr(i, 2);
                    var box = layout.getBox(row, i).inflate(-4, -4);
                    box.x += box.w / 2;
                    bigrams.push({
                    bigram: bigram,
                    matched: false,
                    element: new algo.render.Rectangle({
                    state: algo.render.kS_FADED,
                    shape: box,
                    fontSize: 24,
                    text: bigram
                    })
                    });
                }

                return bigrams;

            }

            var w1Bigrams = displayPairs(w1, 1);
            var w2Bigrams = displayPairs(w2, 2);

            //=bigrams
            yield ({
            step: "Construct a list of letter pairs from each word",
            line: "bigrams"
            });

            // setup the progress bar/score display on the bottom row
            var scoreBox = layout.getRowBox(4).inflate(-20, -10);

            var scoreRectangle = new algo.render.Rectangle({
            strokeWidth: 1,
            stroke: 'black',
            fill: 'white',
            pen: 'black',
            shape: scoreBox,
            fontSize: 24
            });

            var progressBar = new algo.render.Rectangle({
            parent: scoreRectangle,
            strokeWidth: 0,
            x: 0,
            y: 0,
            w: 0,
            h: scoreRectangle.h
            });

            // find how many bigrams from word 1 are present in the bigrams from word 2
            var matchingPairs = 0;

            // update the similarity score incrementally so we can display to the user as we calculate
            var score = 0;

            // total number of bigrams in both words
            var max = (w1.length + w2.length - 2);

            // get a color palette containing colors equal to the largest possible set of matches
            var colors = new algo.Color('royalblue').circularPalette(Math.max(w1Bigrams.length, w2Bigrams.length));

            for (var i = 0; i < w1Bigrams.length; i += 1) {

                // find out if the current bigram has a match in the second word
                var w1b = w1Bigrams[i];
                var match = _.find(w2Bigrams, function(w2b) {
                    return w1b.bigram === w2b.bigram;
                }, this);

                if (match) {
                    //=remove matching bigram from list so it is not considered again
                    w2Bigrams = _.without(w2Bigrams, match);

                    // color the matching pair
                    algo.render.Element.set({
                    fill: colors[matchingPairs >> 1],
                    pen: 'white',
                    stroke: colors[matchingPairs >> 1]
                    }, w1b.element, match.element);

                    // bump count of matches, by two since the bigrams occurs once
                    // in each word
                    matchingPairs += 2;

                    // update score
                    score = matchingPairs / max;

                    scoreRectangle.set({
                    text: _.sprintf("Sørensen–Dice Index: %1.4f", score)
                    });

                    // progress bar width and color are related to the score
                    progressBar.set({
                    w: scoreRectangle.w * score,
                    fill: new algo.Color({
                    red: 1 - score,
                    green: score,
                    blue: 0,
                    alpha: 0.3
                    })
                    });

                    yield ({
                    step: _.sprintf("The bigram '%s' occurs in both strings.", w1b.bigram),
                    line: "remove",
                    variables: {
                        "Matching Bigrams": matchingPairs,
                        "Total Bigrams": max,
                        "Score": _.sprintf("%1.4f", score)
                    }
                    });
                }
            }
            
            //=results
            yield ({
            step: _.sprintf("There were %d matching bigrams from a possible total of %d. The Sørensen–Dice similarity index is %1.4f",
                            matchingPairs, max, score),
            line: "results",
            variables: {
                "Matching Bigrams": matchingPairs,
                "Total Bigrams": max,
                "Score": _.sprintf("%1.4f", score)
            }
            });
            
            // destroy any previous display elements
            algo.SURFACE.root.children.destroy();
        }
    }
}
