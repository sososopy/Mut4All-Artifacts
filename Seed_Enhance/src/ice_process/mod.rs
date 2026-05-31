use std::{error::Error, fmt::Debug};

use dyn_clone::DynClone;

use crate::{conf::Args, fuzz::fuzzbase::FResult};

pub mod querystack;
pub mod panicfunc;
pub mod flagbisect;

#[allow(unused)]
pub trait ICEFilter: Send + Sync + DynClone + Debug {
    fn filter(&self, info: &FResult) -> bool;
    fn add(&mut self, info: &FResult) -> bool;
    fn reset(&mut self);
    fn import(&mut self, args: &Args) -> Result<(), Box<dyn Error>>;
    fn export(&self, args: &Args) -> Result<(), Box<dyn Error>>;
}

#[derive(Default, Clone, Debug)]
pub struct DummyFilter {}
impl DummyFilter {
    #[allow(clippy::new_ret_no_self)]
    pub fn new() -> Box<dyn ICEFilter> {
        Box::new(Self {})
    }
}
impl ICEFilter for DummyFilter {
    fn filter(&self, _: &FResult) -> bool {
        true
    }
    fn add(&mut self, _: &FResult) -> bool {
        true
    }
    fn reset(&mut self) {}
    fn import(&mut self, _: &Args) -> Result<(), Box<dyn Error>> {
        Ok(())
    }
    fn export(&self, _: &Args) -> Result<(), Box<dyn Error>> {
        Ok(())
    }
}
