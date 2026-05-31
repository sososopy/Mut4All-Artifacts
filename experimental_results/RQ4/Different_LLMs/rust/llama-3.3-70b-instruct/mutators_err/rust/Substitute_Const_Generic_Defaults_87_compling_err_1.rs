use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Substitute_Const_Generic_Defaults_87;

impl Mutator for Substitute_Const_Generic_Defaults_87 {
    fn name(&self) -> &str {
        "Substitute_Const_Generic_Defaults_87"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_def) = item {
                if let Some(generics) = &mut struct_def.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            if let Some(default) = &param.default {
                                let new_default = match default {
                                    Expr::Lit(lit) => {
                                        let new_value = match lit.lit {
                                            Lit::Int(int) => {
                                                let new_value = int.base10_parse::<u64>().unwrap() + 1;
                                                Lit::Int(LitInt::new(new_value, Span::call_site()))
                                            }
                                            Lit::Float(float) => {
                                                let new_value = float.base10_parse::<f64>().unwrap() + 1.0;
                                                Lit::Float(LitFloat::new(new_value, Span::call_site()))
                                            }
                                            _ => panic!("Unsupported literal type"),
                                        };
                                        Expr::Lit(new_value)
                                    }
                                    _ => panic!("Unsupported default expression"),
                                };
                                param.default = Some(new_default);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes the default values of const generics in struct definitions. It aims to test the compiler's handling of const generics with different default values and their interaction with other language features."
    }
}