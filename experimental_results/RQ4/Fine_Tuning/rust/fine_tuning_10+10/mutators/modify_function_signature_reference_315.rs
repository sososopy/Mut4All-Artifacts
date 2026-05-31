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

pub struct Modify_Function_Signature_Reference_315;

impl Mutator for Modify_Function_Signature_Reference_315 {
    fn name(&self) -> &str {
        "Modify_Function_Signature_Reference_315"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(PatType { ty, .. }) = input {
                        if let Type::Reference(type_ref) = &mut **ty {
                            type_ref.mutability = if type_ref.mutability.is_some() {
                                None
                            } else {
                                Some(token::Mut { span: Span::call_site() })
                            };
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function definitions with reference parameters and toggles their mutability. If a parameter is an immutable reference, it is changed to a mutable reference and vice versa. This mutation explores the handling of reference mutability in functions, especially those with inline assembly or C linkage."
    }
}