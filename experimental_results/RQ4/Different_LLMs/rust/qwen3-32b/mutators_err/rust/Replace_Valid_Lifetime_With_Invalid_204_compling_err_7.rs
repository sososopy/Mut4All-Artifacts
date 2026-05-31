use proc_macro2::*;
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

pub struct Replace_Valid_Lifetime_With_Invalid_204;

impl Mutator for Replace_Valid_Lifetime_With_Invalid_204 {
    fn name(&self) -> &str {
        "Replace_Valid_Lifetime_With_Invalid_204"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct LifetimeVisitor;
        impl VisitMut for LifetimeVisitor {
            fn visit_type_mut(&mut self, i: &mut syn::Type) {
                if let syn::Type::Path(type_path) = i {
                    if type_path.span().source_file().is_expansion() {
                        if let Some(args) = type_path.path.segments.last_mut().and_then(|seg| {
                            if let PathArguments::AngleBracketed(args) = &mut seg.arguments {
                                Some(args)
                            } else {
                                None
                            }
                        }) {
                            for arg in &mut args.args {
                                if let GenericArgument::Lifetime(_) = arg {
                                    let invalid_type: Type = parse_quote!(invalid);
                                    *arg = GenericArgument::Type(invalid_type);
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_type_mut(self, i);
            }
        }
        let mut visitor = LifetimeVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro-generated code where a valid lifetime prefix is used in a trait bound. It replaces the valid lifetime with an invalid identifier, causing syntax errors or type-checking failures. This exploits the interaction between macro parsing and edition-specific rules for prefixed lifetimes, potentially leading to ICEs or incorrect error messages."
    }
}