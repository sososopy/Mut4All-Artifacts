use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_NoMangle_With_Panic_Unwind_119;

impl Mutator for Modify_NoMangle_With_Panic_Unwind_119 {
    fn name(&self) -> &str {
        "Modify_NoMangle_With_Panic_Unwind_119"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.attrs.iter().any(|attr| attr.path.is_ident("no_mangle")) {
                    let panic_attr: syn::Attribute = parse_quote! {
                        #[cfg_attr(target_os = "linux", panic = "unwind")]
                    };
                    func.attrs.insert(0, panic_attr);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with the `#[no_mangle]` attribute, adding a `#[cfg_attr(target_os = \"linux\", panic = \"unwind\")]` attribute above them. This transformation tests the interaction between function mangling prevention and OS-specific panic behavior, potentially revealing issues related to monomorphization or target-specific configurations."
    }
}