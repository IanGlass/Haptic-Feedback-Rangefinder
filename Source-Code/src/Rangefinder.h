/**************************************************************************//**
* @file Rangefinder.c
* @author Ian Glass
* @version 3.20.2
* @date    28-July-2018
* @brief   Include module for Rangefinder.c
*******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/

/* Defines -------------------------------------------------------------------*/
#ifndef TRIG_PIN
#define TRIG_PIN     3
#endif
#ifndef TRIG_PORT
#define TRIG_PORT    gpioPortB
#endif
#ifndef ECHO_PIN
#define ECHO_PIN     4
#endif
#ifndef ECHO_PORT
#define ECHO_PORT    gpioPortB
#endif
#ifndef PWM_PIN
#define PWM_PIN     4
#endif
#ifndef PWM_PORT
#define PWM_PORT    gpioPortF
#endif
#ifndef BUTTON_PIN
#define BUTTON_PIN     13
#endif
#ifndef BUTTON_PORT
#define BUTTON_PORT    gpioPortC
#endif
#ifndef CHARGE_PIN
#define CHARGE_PIN     0
#endif
#ifndef CHARGE_PORT
#define CHARGE_PORT    gpioPortA
#endif

//Optimal without signal cross-over
#define TRIGGER_CALL_FREQ 25
//Set to 40us high time
#define TRIGGER_HIGH_FREQ 25000
//PWM CALL frequency (100 times PWM frequency). Should be no greater than time to acquire 5 samples
#define PWM_FREQ 100000 //25000
//Systick call frequency
#define SYS_FREQ 10000
//Frequency for range indicator (set to 2 Hz atm)
#define RANGE_FREQ 200


















