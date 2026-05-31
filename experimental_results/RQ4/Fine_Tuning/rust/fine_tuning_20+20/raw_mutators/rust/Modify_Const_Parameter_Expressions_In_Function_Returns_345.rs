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

pub struct Modify_Const_Parameter_Expressions_In_Function_Returns_345;

impl Mutator for Modify_Const_Parameter_Expressions_In_Function_Returns_345 {
    fn name(&self) -> &str {
        "Modify_Const_Parameter_Expressions_In_Function_Returns_345"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let const_params: Vec<_> = item_fn
                    .sig
                    .generics
                    .params
                    .iter()
                    .filter_map(|param| match param {
                        GenericParam::Const(const_param) => Some(const_param.ident.to_string()),
                        _ => None,
                    })
                    .collect();
                if const_params.is_empty() {
                    continue;
                }
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        let mut new_type_path = type_path.clone();
                        for segment in &mut new_type_path.path.segments {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Const(expr) = arg {
                                        let mut expr_str = expr.to_token_stream().to_string();
                                        for const_param in &const_params {
                                            if expr_str.contains(const_param) {
                                                expr_str = format!(
                                                    "({}) + {} / 2",
                                                    expr_str, const_param
                                                );
                                                break;
                                            }
                                        }
                                        *expr = parse_quote!(#expr_str);
                                    }
                                }
                            }
                        }
                        item_fn.sig.output =
                            ReturnType::Type(Default::default(), Box::new(Type::Path(new_type_path)));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with generic const parameters that return a struct using const expressions. It modifies the return type's const expression to create a more complex expression, aiming to stress-test the const generics system by combining existing const parameters in a non-trivial way."
    }
}