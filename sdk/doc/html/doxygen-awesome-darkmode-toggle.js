/**

Doxygen Awesome
https://github.com/jothepro/doxygen-awesome-css

MIT License

Copyright (c) 2021 - 2023 jothepro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

class DoxygenAwesomeDarkModeToggle extends HTMLElement {
    // SVG icons from https://fonts.google.com/icons
    // Licensed under the Apache 2.0 license:
    // https://www.apache.org/licenses/LICENSE-2.0.html
    static lightModeIcon = `<svg xmlns="http://www.w3.org/2000/svg" xml:space="preserve" width="800px" height="800px" shape-rendering="geometricPrecision" text-rendering="geometricPrecision" image-rendering="optimizeQuality" clip-rule="evenodd" viewBox="0 0 10240 10240"><path id="curve2" fill="#005075" d="M5120 2560c3,0 7,0 10,0l0 635 -10 5c-1060,0 -1920,860 -1920,1920 0,1060 860,1920 1920,1920l10 6 0 634c-3,0 -7,0 -10,0 -1414,0 -2560,-1146 -2560,-2560 0,-1414 1146,-2560 2560,-2560z"/><path id="curve1" fill="#005075" d="M5120 2560c1414,0 2560,1146 2560,2560 0,1414 -1146,2560 -2560,2560l0 -640 0 -3840 0 -640z"/><path id="curve0" fill="#005075" d="M5120 9600l0 0c-176,0 -320,-144 -320,-320l0 -640c0,-176 144,-320 320,-320l0 0c176,0 320,144 320,320l0 640c0,176 -144,320 -320,320zm0 -8960l0 0c176,0 320,144 320,320l0 640c0,176 -144,320 -320,320l0 0c-176,0 -320,-144 -320,-320l0 -640c0,-176 144,-320 320,-320zm-2240 600l0 0c153,-88 349,-35 437,117l320 555c88,152 36,349 -117,437l0 0c-153,88 -349,35 -437,-117l-320 -555c-88,-152 -36,-349 117,-437zm-1640 1640l0 0c88,-153 285,-205 437,-117l555 320c152,88 205,284 117,437l0 0c-88,153 -285,205 -437,117l-555 -320c-152,-88 -205,-284 -117,-437zm-600 2240l0 0c0,-176 144,-320 320,-320l640 0c176,0 320,144 320,320l0 0c0,176 -144,320 -320,320l-640 0c-176,0 -320,-144 -320,-320zm600 2240l0 0c-88,-153 -35,-349 117,-437l555 -320c152,-88 349,-36 437,117l0 0c88,153 35,349 -117,437l-555 320c-152,88 -349,36 -437,-117zm1640 1640l0 0c-153,-88 -205,-285 -117,-437l320 -555c88,-152 284,-205 437,-117l0 0c153,88 205,285 117,437l-320 555c-88,152 -284,205 -437,117z"/></svg>`
    static darkModeIcon = `<svg xmlns="http://www.w3.org/2000/svg" xml:space="preserve" width="800px" height="800px" shape-rendering="geometricPrecision" text-rendering="geometricPrecision" image-rendering="optimizeQuality" clip-rule="evenodd" viewBox="0 0 10240 10240"><path id="curve2" fill="#005075" d="M5120 2560c3,0 7,0 10,0l0 635 -10 5c-1060,0 -1920,860 -1920,1920 0,1060 860,1920 1920,1920l10 6 0 634c-3,0 -7,0 -10,0 -1414,0 -2560,-1146 -2560,-2560 0,-1414 1146,-2560 2560,-2560z"/><path id="curve1" fill="#005075" d="M5120 2560c1414,0 2560,1146 2560,2560 0,1414 -1146,2560 -2560,2560l0 -640 0 -3840 0 -640z"/><path id="curve0" fill="#005075" d="M5120 9600l0 0c-176,0 -320,-144 -320,-320l0 -640c0,-176 144,-320 320,-320l0 0c176,0 320,144 320,320l0 640c0,176 -144,320 -320,320zm0 -8960l0 0c176,0 320,144 320,320l0 640c0,176 -144,320 -320,320l0 0c-176,0 -320,-144 -320,-320l0 -640c0,-176 144,-320 320,-320zm-2240 600l0 0c153,-88 349,-35 437,117l320 555c88,152 36,349 -117,437l0 0c-153,88 -349,35 -437,-117l-320 -555c-88,-152 -36,-349 117,-437zm-1640 1640l0 0c88,-153 285,-205 437,-117l555 320c152,88 205,284 117,437l0 0c-88,153 -285,205 -437,117l-555 -320c-152,-88 -205,-284 -117,-437zm-600 2240l0 0c0,-176 144,-320 320,-320l640 0c176,0 320,144 320,320l0 0c0,176 -144,320 -320,320l-640 0c-176,0 -320,-144 -320,-320zm600 2240l0 0c-88,-153 -35,-349 117,-437l555 -320c152,-88 349,-36 437,117l0 0c88,153 35,349 -117,437l-555 320c-152,88 -349,36 -437,-117zm1640 1640l0 0c-153,-88 -205,-285 -117,-437l320 -555c88,-152 284,-205 437,-117l0 0c153,88 205,285 117,437l-320 555c-88,152 -284,205 -437,117z"/></svg>`
    static title = "Toggle Light/Dark Mode"

    static prefersLightModeInDarkModeKey = "prefers-light-mode-in-dark-mode"
    static prefersDarkModeInLightModeKey = "prefers-dark-mode-in-light-mode"

    static _staticConstructor = function() {
        DoxygenAwesomeDarkModeToggle.enableDarkMode(DoxygenAwesomeDarkModeToggle.userPreference)
        // Update the color scheme when the browsers preference changes
        // without user interaction on the website.
        window.matchMedia('(prefers-color-scheme: dark)').addEventListener('change', event => {
            DoxygenAwesomeDarkModeToggle.onSystemPreferenceChanged()
        })
        // Update the color scheme when the tab is made visible again.
        // It is possible that the appearance was changed in another tab
        // while this tab was in the background.
        document.addEventListener("visibilitychange", visibilityState => {
            if (document.visibilityState === 'visible') {
                DoxygenAwesomeDarkModeToggle.onSystemPreferenceChanged()
            }
        });
    }()

    static init() {
        $(function() {
            $(document).ready(function() {
                const toggleButton = document.createElement('doxygen-awesome-dark-mode-toggle')
                toggleButton.title = DoxygenAwesomeDarkModeToggle.title
                toggleButton.updateIcon()

                window.matchMedia('(prefers-color-scheme: dark)').addEventListener('change', event => {
                    toggleButton.updateIcon()
                })
                document.addEventListener("visibilitychange", visibilityState => {
                    if (document.visibilityState === 'visible') {
                        toggleButton.updateIcon()
                    }
                });

                $(document).ready(function(){
                    document.getElementById("MSearchBox").parentNode.appendChild(toggleButton)
                })
                $(window).resize(function(){
                    document.getElementById("MSearchBox").parentNode.appendChild(toggleButton)
                })
            })
        })
    }

    constructor() {
        super();
        this.onclick=this.toggleDarkMode
    }

    /**
     * @returns `true` for dark-mode, `false` for light-mode system preference
     */
    static get systemPreference() {
        return window.matchMedia('(prefers-color-scheme: dark)').matches
    }

    /**
     * @returns `true` for dark-mode, `false` for light-mode user preference
     */
    static get userPreference() {
        return (!DoxygenAwesomeDarkModeToggle.systemPreference && localStorage.getItem(DoxygenAwesomeDarkModeToggle.prefersDarkModeInLightModeKey)) ||
        (DoxygenAwesomeDarkModeToggle.systemPreference && !localStorage.getItem(DoxygenAwesomeDarkModeToggle.prefersLightModeInDarkModeKey))
    }

    static set userPreference(userPreference) {
        DoxygenAwesomeDarkModeToggle.darkModeEnabled = userPreference
        if(!userPreference) {
            if(DoxygenAwesomeDarkModeToggle.systemPreference) {
                localStorage.setItem(DoxygenAwesomeDarkModeToggle.prefersLightModeInDarkModeKey, true)
            } else {
                localStorage.removeItem(DoxygenAwesomeDarkModeToggle.prefersDarkModeInLightModeKey)
            }
        } else {
            if(!DoxygenAwesomeDarkModeToggle.systemPreference) {
                localStorage.setItem(DoxygenAwesomeDarkModeToggle.prefersDarkModeInLightModeKey, true)
            } else {
                localStorage.removeItem(DoxygenAwesomeDarkModeToggle.prefersLightModeInDarkModeKey)
            }
        }
        DoxygenAwesomeDarkModeToggle.onUserPreferenceChanged()
    }

    static enableDarkMode(enable) {
        if(enable) {
            DoxygenAwesomeDarkModeToggle.darkModeEnabled = true
            document.documentElement.classList.add("dark-mode")
            document.documentElement.classList.remove("light-mode")
        } else {
            DoxygenAwesomeDarkModeToggle.darkModeEnabled = false
            document.documentElement.classList.remove("dark-mode")
            document.documentElement.classList.add("light-mode")
        }
    }

    static onSystemPreferenceChanged() {
        DoxygenAwesomeDarkModeToggle.darkModeEnabled = DoxygenAwesomeDarkModeToggle.userPreference
        DoxygenAwesomeDarkModeToggle.enableDarkMode(DoxygenAwesomeDarkModeToggle.darkModeEnabled)
    }

    static onUserPreferenceChanged() {
        DoxygenAwesomeDarkModeToggle.enableDarkMode(DoxygenAwesomeDarkModeToggle.darkModeEnabled)
    }

    toggleDarkMode() {
        DoxygenAwesomeDarkModeToggle.userPreference = !DoxygenAwesomeDarkModeToggle.userPreference
        this.updateIcon()
    }

    updateIcon() {
        if(DoxygenAwesomeDarkModeToggle.darkModeEnabled) {
            this.innerHTML = DoxygenAwesomeDarkModeToggle.darkModeIcon
        } else {
            this.innerHTML = DoxygenAwesomeDarkModeToggle.lightModeIcon
        }
    }
}

customElements.define("doxygen-awesome-dark-mode-toggle", DoxygenAwesomeDarkModeToggle);
