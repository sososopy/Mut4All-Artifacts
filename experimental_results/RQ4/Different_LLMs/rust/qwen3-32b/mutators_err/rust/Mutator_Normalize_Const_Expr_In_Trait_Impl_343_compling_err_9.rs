use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashMap, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprLit, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Mutator_Normalize_Const_Expr_In_Trait_Impl_343;

impl Mutator for Mutator_Normalize_Const_Expr_In_Trait_Impl_343 {
    fn name(&self) -> &str {
        "Mutator_Normalize_Const_Expr_In_Trait_Impl_343"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut const_decls: HashMap<Ident, String> = HashMap::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                if let Some(expr_box) = item_const.expr.as_ref() {
                    let expr = &**expr_box;
                    if let &Expr::Lit(expr_lit) = expr {
                        match &expr_lit.lit {
                            Lit::Int(int_lit) => {
                                if let Ok(value) = int_lit.base10_parse::<u64>() {
                                    const_decls.insert(item_const.ident.clone(), value.to_string());
                                }
                            },
                            Lit::Str(str_lit) => {
                                const_decls.insert(item_const.ident.clone(), str_lit.value());
                            },
                            _ => {}
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &mut item_impl.trait_ {
                    for segment in &mut trait_path.segments {
                        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Const(expr) = arg {
                                    process_const_expr(expr, &const_decls);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator normalizes const expressions in trait implementations by replacing const variables with their literal values or vice versa. This transformation ensures that all const generic arguments are either literals or variables with matching values, promoting consistency and testing the compiler's handling of const generics in trait implementations."
    }
}

fn process_const_expr(expr: &mut Expr, const_decls: &HashMap<Ident, String>) {
    match *expr {
        Expr::Path(ref expr_path) => {
            if let Some(segment) = expr_path.path.segments.iter().next() {
                if let Some(value_str) = const_decls.get(&segment.ident) {
                    *expr = parse_quote!(#value_str);
                }
            }
        },
        Expr::Lit(ref expr_lit) => {
            let mut found_const = None;
            match &expr_lit.lit {
                Lit::Int(int_lit) => {
                    if let Ok(value) = int_lit.base10_parse::<u64>() {
                        let value_str = value.to_string();
                        for (name, val) in const_decls {
                            if val == &value_str {
                                found_const = Some(name.clone());
                                break;
                            }
                        }
                    }
                },
                Lit::Str(str_lit) => {
                    let value_str = str_lit.value();
                    for (name, val) in const_decls {
                        if val == &value_str {
                            found_const = Some(name.clone());
                            break;
                        }
                    }
                },
                _ => {}
            }
            if let Some(const_name) = found_const {
                *expr = parse_quote!(#const_name);
            }
        },
        _ => {}
    }
}