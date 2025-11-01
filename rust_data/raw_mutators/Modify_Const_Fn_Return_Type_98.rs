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

pub struct Modify_Const_Fn_Return_Type_98;

impl Mutator for Modify_Const_Fn_Return_Type_98 {
    fn name(&self) -> &str {
        "Modify_Const_Fn_Return_Type_98"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                if func.sig.constness.is_some() {
                    if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                        if let Type::ImplTrait(ref mut impl_trait) = **ty {
                            for bound in impl_trait.bounds.iter_mut() {
                                if let TypeParamBound::Trait(ref mut trait_bound) = bound {
                                    if let Some(ref mut path) = trait_bound.path.segments.last_mut() {
                                        path.ident = Ident::new("~const", path.ident.span());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets const functions with complex return types involving traits. By modifying the return type to include an incorrect keyword like '~const', it introduces potential syntax errors or parser confusion. This mutation aims to explore how the Rust compiler handles such erroneous constructs, potentially revealing issues in type resolution or parsing."
    }
}