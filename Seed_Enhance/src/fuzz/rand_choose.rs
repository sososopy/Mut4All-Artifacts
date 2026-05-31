use std::{
    error::Error,
    fmt::Debug,
    ops::{Add, AddAssign, Sub, SubAssign},
};

use rand::distr::uniform::SampleUniform;

use crate::util::{RNG, glob_next};

#[doc(hidden)]
pub struct If<const B: bool>;
#[doc(hidden)]
pub trait True {}
impl True for If<true> {}

struct FenWickTree<T>
where
    T: Add<Output = T> + Sub<Output = T> + AddAssign + Copy + Default,
{
    n: usize,
    arr: Vec<T>,
}

impl<T> FenWickTree<T>
where
    T: Add<Output = T> + Sub<Output = T> + AddAssign + Copy + Default,
{
    pub fn new(sz: usize) -> Self {
        if sz + 1 >= i64::MAX as usize {
            panic!("Too big for a FenWickTree")
        }
        Self {
            n: sz,
            arr: vec![T::default(); sz + 1],
        }
    }

    fn lowbit(x: i64) -> i64 {
        x & (-x)
    }

    pub fn modify(&mut self, mut pos: usize, val: T) {
        pos += 1; // FenWickTree's range is [1, N], convert to [0,N)
        while pos <= self.n {
            self.arr[pos] += val;
            pos = pos + Self::lowbit(pos as i64) as usize;
        }
    }

    fn __query(&self, mut pos: usize) -> T {
        let mut res: T = T::default();
        while pos > 0 {
            res += self.arr[pos];
            pos = pos - Self::lowbit(pos as i64) as usize;
        }
        res
    }

    pub fn query(&self, l: usize, r: usize) -> T {
        let _l = l + 1; // FenWickTree's range is [1, N], convert to [0,N)
        let _r = r + 1;
        self.__query(_r) - self.__query(_l)
    }
}

pub struct WeightedRand<T, WeightType = usize>
where
    WeightType: Copy + Default,
{
    sum: WeightType,
    items: Vec<(T, WeightType)>,
    rng: RNG,
}

impl<T, WeightType> WeightedRand<T, WeightType>
where
    WeightType: SampleUniform
        + Add<Output = WeightType>
        + Sub<Output = WeightType>
        + AddAssign
        + SubAssign
        + PartialEq
        + PartialOrd
        + Copy
        + Default
        + Debug,
{
    pub fn new(items: Option<Vec<(T, WeightType)>>, rng: Option<RNG>) -> Self {
        let rng = match rng {
            Some(r) => r,
            None => {
                let mut seed: [u8; 32] = [0; 32];
                for item in &mut seed {
                    *item = glob_next();
                }
                RNG::new(seed)
            }
        };
        let items = items.unwrap_or_default();
        let sum = items.iter().fold(WeightType::default(), |i, s| i + s.1);
        Self { sum, items, rng }
    }

    pub fn get_weight(&self, idx: usize) -> WeightType {
        self.items[idx].1
    }

    pub fn set_weight(&mut self, idx: usize, val: WeightType) {
        self.sum -= self.items[idx].1;
        self.items[idx].1 = val;
        self.sum += self.items[idx].1;
    }

    pub fn rand(&mut self) -> Result<(&T, usize), Box<dyn Error>> {
        let bound = self.sum;
        let r = WeightType::default()..bound;
        let mut rnd = self.rng.range(r);
        for idx in 0..self.items.len() {
            let (item, weight) = &self.items[idx];
            let w = *weight;
            if rnd < w {
                return Ok((item, idx));
            }
            rnd -= w;
        }
        let (it, _) = self.items.last().ok_or("Have no rand items")?;
        Ok((it, self.items.len() - 1))
    }
}

type WeightGen<P, R> = fn(&P) -> R;

pub struct WeightedRandDynamic<T, WeightType = usize, WeightParam = ()>
where
    WeightType: Copy + Default,
{
    items: Vec<(T, WeightGen<WeightParam, WeightType>)>,
    rng: RNG,
}

impl<T, WeightType, WeightParam> WeightedRandDynamic<T, WeightType, WeightParam>
where
    WeightType: SampleUniform
        + Add<Output = WeightType>
        + Sub<Output = WeightType>
        + AddAssign
        + SubAssign
        + PartialEq
        + PartialOrd
        + Copy
        + Default
        + Debug,
{
    pub fn new(items: Option<Vec<(T, WeightGen<WeightParam, WeightType>)>>, rng: Option<RNG>) -> Self {
        let rng = match rng {
            Some(r) => r,
            None => {
                let mut seed: [u8; 32] = [0; 32];
                for item in &mut seed {
                    *item = glob_next();
                }
                RNG::new(seed)
            }
        };
        let items = items.unwrap_or_default();
        Self { items, rng }
    }

    pub fn rand(&mut self, param: &WeightParam) -> Result<(&T, usize), Box<dyn Error>> {
        let weights: Vec<_> = self.items.iter().map(|x| x.1(param)).collect();
        let sum = weights.iter().fold(WeightType::default(), |a, b| a + *b);
        let r = WeightType::default()..sum;
        let mut rnd = self.rng.range(r);
        #[allow(clippy::needless_range_loop)] // Clippy is wrong
        for idx in 0..self.items.len() {
            let (item, _) = &self.items[idx];
            if rnd < weights[idx] {
                return Ok((item, idx));
            }
            rnd -= weights[idx];
        }
        let (it, _) = self.items.last().ok_or("Have no rand items")?;
        Ok((it, self.items.len() - 1))
    }
}
