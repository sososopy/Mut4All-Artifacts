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

pub struct Introduce_Lifetime_Annotations_588;

impl Mutator for Introduce_Lifetime_Annotations_588 {
    fn name(&self) -> &str {
        "Introduce_Lifetime_Annotations_588"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut has_lifetime = false;
                for input in &func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &*pat_type.ty {
                            if type_ref.lifetime.is_some() {
                                has_lifetime = true;
                                break;
                            }
                        }
                    }
                }
                if !has_lifetime {
                    let lifetime = Lifetime::new("'a", Span::call_site());
                    func.sig.generics.params.push(GenericParam::Lifetime(
                        LifetimeParam::new(lifetime.clone()),
                    ));
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                type_ref.lifetime = Some(lifetime.clone());
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces explicit lifetime annotations to function parameters involving references where lifetimes are not already specified. This transformation aims to test the compiler's handling of lifetime resolution and scoping by making lifetime relationships explicit, potentially uncovering issues in lifetime inference or constraints."
    }
}