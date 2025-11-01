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
    TraitItem,
};

use crate::mutator::Mutator;

pub struct Alter_Function_Signature_260;

impl Mutator for Alter_Function_Signature_260 {
    fn name(&self) -> &str {
        "Alter_Function_Signature_260"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Fn(method) = item {
                        if method.sig.generics.lifetimes().count() > 0 {
                            let new_lifetime = Lifetime::new("'conflict", Span::call_site());
                            for input in &mut method.sig.inputs {
                                if let FnArg::Typed(pat_type) = input {
                                    if let Type::Path(type_path) = &mut *pat_type.ty {
                                        if let Some(last_segment) = type_path.path.segments.last_mut() {
                                            if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                                                args.args.push(GenericArgument::Lifetime(new_lifetime.clone()));
                                            }
                                        }
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
        "This mutation targets trait method declarations with lifetime parameters. It introduces a conflicting lifetime to the method signature, aiming to challenge the borrow checker's assumptions and reveal inconsistencies in lifetime resolution during compilation."
    }
}