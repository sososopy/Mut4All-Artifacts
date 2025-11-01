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

pub struct Modify_Default_Type_Of_Const_Generic_Parameters_182;

impl Mutator for Modify_Default_Type_Of_Const_Generic_Parameters_182 {
    fn name(&self) -> &str {
        "Modify_Default_Type_Of_Const_Generic_Parameters_182"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, .. }) = item {
                for param in &mut generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &const_param.default {
                            if let Expr::Path(ExprPath { path, .. }) = default.as_ref() {
                                if path.segments.len() == 1 {
                                    let segment = &path.segments[0];
                                    if segment.ident != "bool" {
                                        const_param.default = Some(parse_quote! { bool });
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
        "The mutation operator targets struct definitions with const generic parameters and modifies the default type of these parameters to a type that is not a valid expression. This is done by checking if the default type is a simple path and changing it to 'bool' if it is not already 'bool'. This alteration forces the compiler to handle cases where default types are improperly specified, potentially exposing bugs related to type aliasing and constant expression evaluations."
    }
}