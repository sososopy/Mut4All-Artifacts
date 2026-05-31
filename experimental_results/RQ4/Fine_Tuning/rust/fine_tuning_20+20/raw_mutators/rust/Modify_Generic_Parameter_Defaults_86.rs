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

pub struct Modify_Generic_Parameter_Defaults_86;

impl Mutator for Modify_Generic_Parameter_Defaults_86 {
    fn name(&self) -> &str {
        "Modify_Generic_Parameter_Defaults_86"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_const_default = false;
                let mut const_default_index = 0;
                let mut const_default_param = None;
                for (i, param) in item_struct.generics.params.iter().enumerate() {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            has_const_default = true;
                            const_default_index = i;
                            const_default_param = Some(param.clone());
                            break;
                        }
                    }
                }
                if has_const_default {
                    let mut new_params = Punctuated::new();
                    for (i, param) in item_struct.generics.params.iter().enumerate() {
                        if i != const_default_index {
                            new_params.push(param.clone());
                        }
                    }
                    if let Some(default_param) = const_default_param {
                        new_params.push(default_param);
                    }
                    item_struct.generics.params = new_params;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions with const generics that have default values. It checks if any const generic parameter with a default value is not the last in the parameter list. If such a parameter is found, it moves it to the end of the list, ensuring that all parameters with defaults are trailing. This transformation tests the compiler's handling of generic parameter ordering and default value semantics, potentially exposing bugs in AST parsing, generic resolution, or code generation."
    }
}