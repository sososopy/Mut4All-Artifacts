use std::{error::Error, fmt::Debug, path::Path};

use crate::fuzz::fuzzbase::{FResult, Fuzzer};

fn bruce_filter<T, F>(mut set: Vec<T>, test_fn: &F) -> Vec<T>
where
    T: Clone + Eq,
    F: Fn(&[T]) -> bool,
{
    for i in (set.len() - 1)..=0 {
        let tmp = set.swap_remove(i);
        if !test_fn(&set) {
            set.push(tmp);
        }
    }

    set
}

fn quick_xplain<T, F>(set: Vec<T>, test_fn: &F, start: Vec<T>) -> Vec<T>
where
    T: Clone + Eq + Debug,
    F: Fn(&[T]) -> bool,
{
    // println!("{} / {:#?}", set.len(), set);
    if set.len() <= 1 {
        return set;
    }

    let mid = set.len() / 2;
    let mut left = set;
    let right = left.split_off(mid);

    let mut test_left = left.clone();
    test_left.extend(start.clone());
    if test_fn(&test_left) {
        return quick_xplain(left.clone(), test_fn, start.clone());
    }
    let mut test_right = right.clone();
    test_right.extend(start.clone());
    if test_fn(&test_right) {
        return quick_xplain(right.clone(), test_fn, start.clone());
    }

    let mut new_start = start.clone();
    new_start.extend(left.clone());
    let from_right = quick_xplain(right.clone(), test_fn, new_start);

    let mut new_start = start.clone();
    new_start.extend(right.clone());
    let from_left = quick_xplain(left.clone(), test_fn, new_start);

    let mut res = from_left;
    res.extend(from_right);
    res.dedup();

    res
}

pub fn filter_flags<T: Fuzzer>(
    flags: Vec<String>,
    code: &[u8],
    output_source: &Path,
    output_bin: &Path,
    extra_args: &[&str],
) -> Result<Vec<String>, Box<dyn Error>> {
    let f = |flags: &[String]| -> bool {
        let flags = flags.iter().map(|s| s.as_str()).collect::<Vec<_>>();
        let (_, res) =
            T::compile_with_features(code, output_source, output_bin, extra_args, &flags).unwrap();
        matches!(res, FResult::InternalCompileError(..))
    };
    let set = quick_xplain(flags, &f, vec![]);
    let set = bruce_filter(set, &f); // Optional, just in-case
    Ok(set)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_quick_xplain() {
        let test_fn = |set: &[i32]| -> bool {
            set.contains(&10) && set.contains(&20) && set.contains(&1) && set.contains(&70)
        };
        let set = vec![1, 2, 3, 4, 5, 10, 20, 30, 40, 50, 60, 70];
        println!("{:?}", set);
        let res = quick_xplain(set, &test_fn, vec![]);
        assert!(test_fn(&res));
    }

    #[test]
    fn test_bruce_filter() {
        let test_fn = |set: &[i32]| -> bool {
            set.contains(&10) && set.contains(&20) && set.contains(&1) && set.contains(&70)
        };
        let set = vec![1, 2, 3, 4, 5, 10, 20, 30, 40, 50, 60, 70];
        let res = bruce_filter(set, &test_fn);
        assert!(test_fn(&res));
    }

    #[test]
    fn test_combinend_filter() {
        let test_fn = |set: &[i32]| -> bool {
            set.contains(&10) && set.contains(&20) && set.contains(&1) && set.contains(&70)
        };
        let set = vec![1, 2, 3, 4, 5, 10, 20, 30, 40, 50, 60, 70];
        let res = quick_xplain(set, &test_fn, vec![]);
        let res = bruce_filter(res, &test_fn);
        assert!(test_fn(&res));
    }
}
