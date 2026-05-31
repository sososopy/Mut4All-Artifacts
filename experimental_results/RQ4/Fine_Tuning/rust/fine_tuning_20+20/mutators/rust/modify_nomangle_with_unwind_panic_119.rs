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

pub struct Modify_NoMangle_With_Unwind_Panic_119;

impl Mutator for Modify_NoMangle_With_Unwind_Panic_119 {
    fn name(&self) -> &str {
        "Modify_NoMangle_With_Unwind_Panic_119"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut has_no_mangle = false;
                for attr in &func.attrs {
                    if attr.path().is_ident("no_mangle") {
                        has_no_mangle = true;
                        break;
                    }
                }
                if has_no_mangle {
                    let panic_attr: syn::Attribute = parse_quote! {
                        #[cfg_attr(target_os = "linux", panic = "unwind")]
                    };
                    func.attrs.insert(0, panic_attr);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with the #[no_mangle] attribute by adding a #[cfg_attr(target_os = \"linux\", panic = \"unwind\")] attribute above them. It modifies the panic behavior to unwind specifically for Linux targets, testing the compiler's handling of target-specific panic configurations in conjunction with no-mangle functions."
    }
}