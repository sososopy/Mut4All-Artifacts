use std::{error::Error, ffi::CStr};

use pyo3::{ffi::c_str, prelude::*};

use crate::{conf::Args, fuzz::fuzzbase::{Fuzzer, InfillFuzzer}};

/*
So the python side class looks like this:

```python
class ModelFuzzer(ABC):
    def __init__(self, args: Args):
        pass // Subclass should implement this

    @abstractmethod
    def infill(self, code_prefix: str, code_suffix: str) -> str:
        pass // Subclass should implement this
```
*/

static MODEL_FUZZER: &CStr = c_str!(include_str!("model_fuzzer.py"));

// #[derive(Clone)]
pub struct LLMModule {
    instance: Py<PyAny>,
    conf: Args,
    // instance: Arc<Mutex<Py<PyAny>>>,
}
impl LLMModule {
    fn do_infill(
        &mut self,
        code_prefix: &[u8],
        code_suffix: &[u8],
    ) -> Result<Vec<u8>, Box<dyn Error>> {
        // let instance_lock = self.instance.lock()
        //     .map_err(|e| e.to_string())?;
        let code_prefix = str::from_utf8(code_prefix)?;
        let code_suffix = str::from_utf8(code_suffix)?;
        let res = Python::with_gil(|py| -> Result<String, Box<dyn Error>> {
            // let instance = instance_lock.bind(py);
            let instance = self.instance.bind(py);
            let res = instance
                .call_method1("infill", (code_prefix, code_suffix))?;
            Ok(
                res.extract()?
            )
        })?;
        let res = res.into_bytes();
        Ok(res)
    }
}
// impl Clone for LLMModule {
//     fn clone(&self) -> Self {
//         let new_instance = Python::with_gil(|py| -> Result<Py<PyAny>, Box<dyn Error>> {
//             let instance = self.instance.bind(py);
//             // copy.deepcopy(instance)
//             let copy = PyModule::import(py, "copy")?;
//             let instance_copy = copy.call_method1("deepcopy", (instance,))?;
//             Ok(instance_copy.into())
//         }).unwrap();
//         LLMModule {
//             instance: new_instance,
//         }
//     }
// }
impl Clone for LLMModule {
    fn clone(&self) -> Self {
        let conf_owned = self.conf.clone();
        let instance = Python::with_gil(|py| -> Result<Py<PyAny>, Box<dyn Error>> {
            let module = PyModule::from_code(
                py,
                MODEL_FUZZER,
                c_str!("model_fuzzer.py"),
                c_str!("model_fuzzer"),
            )?;
            module.gil_used(false)?;

            let class = module.getattr("ModelFuzzer")?;
            let instance = class.call1((conf_owned,))?;

            Ok(
                instance.into()
            )
        }).unwrap();

        LLMModule {
            instance,
            conf: self.conf.clone(),
        }
    }
}
impl Fuzzer for LLMModule {
    fn new(conf: &Args) -> Result<Box<dyn Fuzzer>, Box<dyn Error>> {

        let conf_owned = conf.to_owned();
        let instance = Python::with_gil(|py| -> Result<Py<PyAny>, Box<dyn Error>> {
            let module = PyModule::from_code(
                py,
                MODEL_FUZZER,
                c_str!("model_fuzzer.py"),
                c_str!("model_fuzzer"),
            )?;
            module.gil_used(false)?;

            let class = module.getattr("ModelFuzzer")?;
            let instance = class.call1((conf_owned,))?;

            Ok(
                instance.into()
            )
        })?;
        
        let res = LLMModule {
            // instance: Arc::new(Mutex::new(instance)),
            instance, conf: conf.clone(),
        };
        Ok(Box::new(res))
    }
    fn generate(&mut self) -> Result<Vec<u8>, Box<dyn Error>> {
        let code_prefix = b"";
        let code_suffix = b"";
        let res = self.do_infill(code_prefix, code_suffix)?;
        Ok(res)
    }

    fn as_infill_fuzzer(&self) -> Result<&dyn InfillFuzzer, Box<dyn Error>> {
        Ok(self)
    }
    fn as_infill_fuzzer_mut(&mut self) -> Result<&mut dyn InfillFuzzer, Box<dyn Error>> {
        Ok(self)
    }
}
impl InfillFuzzer for LLMModule {
    fn infill(
        &mut self,
        code_prefix: &[u8],
        code_suffix: &[u8],
    ) -> Result<Vec<u8>, Box<dyn Error>> {
        let res = self.do_infill(code_prefix, code_suffix)?;
        Ok(res)
    }
}
