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

pub struct Replace_Const_Generic_Bound_With_Non_Const_Generic_Bound_133;

impl Mutator for Replace_Const_Generic_Bound_With_Non_Const_Generic_Bound_133 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Bound_With_Non_Const_Generic_Bound_133"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for (i, bound) in trait_item.supertraits.iter_mut().enumerate() {
                    if let TypeParamBound::Trait(TraitBound {
                        paren_token: _,
                        lifetimes: _,
                        path: syn_path,
                    }) = bound {
                        if let Some(segment) = syn_path.segments.last_mut() {
                            if let PathArguments::AngleBracketed(angle_bracketed) = &mut segment.arguments {
                                for arg in angle_bracketed.args.iter_mut() {
                                    if let GenericArgument::Const(arg_const) = arg {
                                        let new_arg = GenericArgument::Type(Type::Path(TypePath {
                                            qself: None,
                                            path: syn_path.clone(),
                                        }));
                                        *arg = new_arg;
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
        "The mutation operator replaces const generic bounds with non-const generic bounds in trait definitions. This transformation can help expose bugs related to the handling of const generics and associated types."
    }
}