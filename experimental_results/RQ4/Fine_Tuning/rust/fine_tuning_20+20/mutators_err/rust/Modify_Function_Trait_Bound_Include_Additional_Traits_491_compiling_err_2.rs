use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Function_Trait_Bound_Include_Additional_Traits_491;

impl Mutator for Modify_Function_Trait_Bound_Include_Additional_Traits_491 {
    fn name(&self) -> &str {
        "Modify_Function_Trait_Bound_Include_Additional_Traits_491"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut available_traits = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                available_traits.insert(item_trait.ident.to_string());
            }
        }
        let std_traits = vec![
            "Clone", "Copy", "Debug", "Display", "Default", "PartialEq", "Eq", "PartialOrd", "Ord",
            "Hash", "From", "Into", "AsRef", "AsMut", "Deref", "DerefMut", "Drop", "Fn", "FnMut",
            "FnOnce", "Iterator", "Extend", "IntoIterator", "FromIterator", "TryFrom", "TryInto",
            "ToOwned", "Borrow", "BorrowMut", "ToString", "Write", "Read", "Seek", "BufRead",
            "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream", "Sink",
            "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary", "Octal", "LowerHex", "UpperHex", "LowerExp",
            "UpperExp", "Pointer", "Write", "Seek", "BufRead", "BufWriter", "BufReader", "Error",
            "Result", "Option", "Future", "Stream", "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek",
            "AsyncBufRead", "Unpin", "Pin", "Send", "Sync", "UnwindSafe", "RefUnwindSafe", "Any",
            "TypeId", "PartialEq", "Eq", "PartialOrd", "Ord", "Hash", "Debug", "Display", "Binary",
            "Octal", "LowerHex", "UpperHex", "LowerExp", "UpperExp", "Pointer", "Write", "Seek",
            "BufRead", "BufWriter", "BufReader", "Error", "Result", "Option", "Future", "Stream",
            "Sink", "AsyncRead", "AsyncWrite", "AsyncSeek", "AsyncBufRead", "Unpin", "Pin", "Send",
            "Sync", "UnwindSafe", "RefUnwindSafe", "Any", "TypeId", "PartialEq", "Eq", "PartialOrd",
            "Ord", "Hash", "Debug", "Display", "Binary",
        ];
        let mut rng = thread_rng();
        let mut target_trait_bounds = Vec::new();
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                            target_trait_bounds.push(type_impl_trait.clone());
                        }
                    }
                }
                if let ReturnType::Type(_, ty) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**ty {
                        target_trait_bounds.push(type_impl_trait.clone());
                    }
                }
            }
        }
        for type_impl_trait in target_trait_bounds {
            let mut additional_traits = Vec::new();
            for _ in 0..3 {
                if let Some(trait_name) = std_traits.choose(&mut rng) {
                    additional_traits.push(trait_name.to_string());
                }
            }
            for _ in 0..3 {
                if let Some(trait_name) = available_traits.iter().choose(&mut rng) {
                    additional_traits.push(trait_name.clone());
                }
            }
            let mut new_bounds = type_impl_trait.bounds.clone();
            for trait_name in additional_traits {
                let path: SynPath = parse_quote!(#trait_name);
                let type_path = TypePath { qself: None, path };
                let type_param_bound = TypeParamBound::Trait(TraitBound {
                    paren_token: None,
                    modifier: TraitBoundModifier::None,
                    lifetimes: None,
                    path: type_path.path.clone(),
                });
                new_bounds.push(type_param_bound);
            }
            let new_type_impl_trait = TypeImplTrait {
                impl_token: type_impl_trait.impl_token,
                bounds: new_bounds,
            };
            let mut visitor = ImplTraitReplacer {
                target: type_impl_trait,
                replacement: new_type_impl_trait,
            };
            visitor.visit_file_mut(file);
        }
    }
}

struct ImplTraitReplacer {
    target: TypeImplTrait,
    replacement: TypeImplTrait,
}

impl VisitMut for ImplTraitReplacer {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::ImplTrait(type_impl_trait) = node {
            if type_impl_trait == &self.target {
                *node = Type::ImplTrait(self.replacement.clone());
            }
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}