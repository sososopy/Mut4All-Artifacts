use std::{
    iter, mem::ManuallyDrop, ops::{Deref, DerefMut}, sync::{LazyLock, Mutex, RwLock}
};

use rand::{
    Rng, RngCore, SeedableRng,
    distr::{
        Distribution, StandardUniform,
        uniform::{SampleRange, SampleUniform},
    },
};
use rand_chacha::ChaCha20Rng;

#[doc(hidden)]
pub struct ForceSend<T> {
    inner: ManuallyDrop<T>,
}

impl<T> ForceSend<T> {
    pub fn new(inner: T) -> Self {
        Self {
            inner: ManuallyDrop::new(inner),
        }
    }
}

unsafe impl<T> Send for ForceSend<T> {}
unsafe impl<T> Sync for ForceSend<T> {}

impl<T> Deref for ForceSend<T> {
    type Target = T;
    fn deref(&self) -> &T {
        &self.inner
    }
}
impl<T> DerefMut for ForceSend<T> {
    fn deref_mut(&mut self) -> &mut T {
        &mut self.inner
    }
}
impl<T> Drop for ForceSend<T> {
    fn drop(&mut self) {
        unsafe {
            ManuallyDrop::drop(&mut self.inner);
        }
    }
}
impl<T> Clone for ForceSend<T>
where
    T: Clone,
{
    fn clone(&self) -> Self {
        Self::new(self.deref().clone())
    }
}

#[allow(clippy::upper_case_acronyms)]
pub struct RNG {
    seed: [u8; 32],
    rng: Box<dyn RngCore>,
}
impl Default for RNG {
    fn default() -> Self {
        let rng = ChaCha20Rng::from_os_rng();
        let seed = rng.get_seed();
        let rng = Box::new(rng);
        let rng = rng as Box<dyn RngCore>;
        Self { seed, rng }
    }
}
impl RNG {
    #[allow(unused)]
    pub fn new(seed: [u8; 32]) -> Self {
        let rng = ChaCha20Rng::from_seed(seed);
        let rng = Box::new(rng);
        let rng = rng as Box<dyn RngCore>;
        Self { seed, rng }
    }
    pub fn renew(&mut self, seed: Option<[u8; 32]>) {
        let seed = seed.unwrap_or_else(|| ChaCha20Rng::from_os_rng().get_seed());
        let rng = ChaCha20Rng::from_seed(seed);
        let rng = Box::new(rng);
        let rng = rng as Box<dyn RngCore>;
        self.rng = rng;
        self.seed = seed;
    }
    pub fn get_seed(&self) -> [u8; 32] {
        self.seed
    }
    pub fn next<T>(&mut self) -> T
    where
        StandardUniform: Distribution<T>,
    {
        self.rng.random()
    }
    pub fn range<T, R>(&mut self, range: R) -> T
    where
        T: SampleUniform,
        R: SampleRange<T>,
    {
        self.rng.random_range(range)
    }
}
unsafe impl Send for RNG {}
unsafe impl Sync for RNG {}

static GLOB_RNG: LazyLock<Mutex<RNG>> = LazyLock::new(|| Mutex::new(RNG::default()));
#[allow(unused)]
pub fn reinit_glob_rng(seed: Option<[u8; 32]>) {
    unsafe {
        if let Ok(mut rng) = GLOB_RNG.lock() {
            rng.renew(seed);
        }
    }
}
#[allow(unused)]
pub fn glob_next<T>() -> T
where
    StandardUniform: Distribution<T>,
{
    unsafe {
        if let Ok(mut rng) = GLOB_RNG.lock() {
            rng.next()
        } else {
            unreachable!()
        }
    }
}
#[allow(unused)]
pub fn glob_range<T, R>(range: R) -> T
where
    T: SampleUniform,
    R: SampleRange<T>,
{
    unsafe {
        if let Ok(mut rng) = GLOB_RNG.lock() {
            rng.range(range)
        } else {
            unreachable!()
        }
    }
}
#[allow(unused)]
pub fn glob_seed() -> [u8; 32] {
    unsafe {
        if let Ok(rng) = GLOB_RNG.lock() {
            rng.get_seed()
        } else {
            unreachable!()
        }
    }
}
#[allow(unused)]
pub fn gen_alpha(len: usize) -> String {
    const CHARSET: &[u8] = b"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
    let f = || CHARSET[glob_range(0..CHARSET.len())] as char;
    iter::repeat_with(f).take(len).collect()
}
#[allow(unused)]
pub fn gen_alnum(len: usize) -> String {
    const CHARSET: &[u8] = b"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890_";
    let f = || CHARSET[glob_range(0..CHARSET.len())] as char;
    iter::repeat_with(f).take(len).collect()
}

#[doc(hidden)]
#[allow(non_upper_case_globals)]
pub static __seed_weight_holder: LazyLock<RwLock<Vec<usize>>> =
    LazyLock::new(|| RwLock::new(Vec::new()));
#[doc(hidden)]
#[allow(non_upper_case_globals)]
pub static __seed_weight_seed_holder: LazyLock<Mutex<Option<Vec<[u8; 32]>>>> =
    LazyLock::new(|| Mutex::new(None));

#[macro_export]
macro_rules! register_ordered_rng {
    ($weight: literal) => {
        register_ordered_rng!(@ $weight, $weight);
    };
    (@ $weight: literal, $func: literal) => {
        #[allow(non_snake_case)]
        #[ctor::ctor]
        fn ${concat("__seed_weight_reg_func_ctor_", $func)}() {
            if let Ok(mut guard) = $crate::util::__seed_weight_holder.write() {
                guard.push($weight.parse().unwrap());
            }
        }

        #[allow(non_snake_case)]
        pub fn ${concat("__seed_weight_reg_func_get_", $func)}() -> $crate::util::RNG {
            let w: usize = $weight.parse().unwrap();

            let pos = {
                let guard = $crate::util::__seed_weight_holder.read().unwrap();
                let mut v = guard.clone();
                v.sort();
                v.iter().position(|n| n == &w).unwrap()
            };
            let rng = {
                let mut guard = $crate::util::__seed_weight_seed_holder.lock().unwrap();
                if guard.is_none() {
                    let mut v: Vec<[u8; 32]> = Vec::new();
                    let len = $crate::util::__seed_weight_holder.read().unwrap().len();
                    for _ in 0..len {
                        let mut t: [u8; 32] = [0; 32];
                        for i in 0..32 {
                            t[i] = $crate::util::glob_next();
                        }
                        v.push(t);
                    }
                    guard.replace(v);
                }
                let seed = guard.as_ref().unwrap();
                $crate::util::RNG::new(seed[pos])
            };
            rng

        }
    };
}

#[macro_export]
macro_rules! get_ordered_rng {
    ($weight: literal) => {
        ${concat("__seed_weight_reg_func_get_", $weight)}()
    }
}
