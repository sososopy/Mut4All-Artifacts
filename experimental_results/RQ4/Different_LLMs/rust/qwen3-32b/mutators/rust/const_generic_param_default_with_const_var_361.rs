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

pub struct Const_Generic_Param_Default_With_Const_Var_361;

impl Mutator for Const_Generic_Param_Default_With_Const_Var_361 {
    fn name(&self) -> &str {
        "Const_Generic_Param_Default_With_Const_Var_361"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                self.process_generics(&mut item_struct.generics);
            } else if let syn::Item::Enum(item_enum) = item {
                self.process_generics(&mut item_enum.generics);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters in struct and enum definitions that lack default values. It replaces each such parameter with one that has a default value assigned to an existing const usize variable in the same scope. This transformation introduces complex substitution scenarios during type checking, potentially exposing ICEs in const parameter resolution and testing the compiler's handling of default const generics."
    }
}

impl Const_Generic_Param_Default_With_Const_Var_361 {
    fn process_generics(&self, generics: &mut syn::Generics) {
        for param in &mut generics.params {
            if let syn::GenericParam::Const(param) = param {
                if param.default.is_none() {
                    let ident = &param.ident;
                    let ty = &param.ty;
                    let new_param = parse_quote!(const #ident: #ty = EXISTING_CONST_USIZE);
                    *param = new_param;
                }
            }
        }
    }
}