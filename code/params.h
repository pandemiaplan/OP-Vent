// params.h
// Limits and increments for input parameters
// (c) Bill Dally, April 2020
//
// Copyright 2020 Bill Dally
// 
// Licensed under the Ventilator Open Source License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// ./LICENSE
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// limits on input parameters
#define MIN_BPM 5
#define MAX_BPM 40
#define INC_BPM 2  

#define MIN_VOL 100
#define MAX_VOL 800
#define INC_VOL 10  //
#define INC_VOL2 20  //

#define MIN_ETOI 5
#define MAX_ETOI 40
#define INC_ETOI 5  //

#define MIN_MAXP 100 // mm of H2O 
#define MAX_MAXP 600 // 
#define INC_MAXP 50

#define MIN_ALARMP 100 // mm of H2O 
#define MAX_ALARMP 700 // 
#define INC_ALARMP 50

#define MIN_LMV 0      // ml  
#define MAX_LMV 30000  
#define INC_LMV 500   

#define MIN_HMV 0      // ml 
#define MAX_HMV 30000 
#define INC_HMV 500    

#define MIN_PIPD 0      // mm of H2O 
#define MAX_PIPD 200    
#define INC_PIPD 10     

#define MIN_TRIGP 10      // mm of H2O 
#define MAX_TRIGP 50    
#define INC_TRIGP 5     
