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

pub struct Alter_Const_Generics_Default_Values_113;

impl Mutator for Alter_Const_Generics_Default_Values_113 {
    fn name(&self) -> &str {
        "Alter_Const_Generics_Default_Values_113"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut mutated = false;
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            const_param.default = Some(parse_quote! {
                                { let x: #const_param.ty = 0; x }
                            });
                            mutated = true;
                        }
                    }
                }
                if mutated {
                    item_struct.generics.where_clause = None;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with const generics that have default values. It replaces the default value with a more complex expression, specifically a block that declares a variable of the const type and initializes it to zero. This transformation challenges the compiler's handling of const generic defaults by introducing a non-trivial const evaluation scenario, potentially exposing weaknesses in const evaluation and generic resolution."
    }
}