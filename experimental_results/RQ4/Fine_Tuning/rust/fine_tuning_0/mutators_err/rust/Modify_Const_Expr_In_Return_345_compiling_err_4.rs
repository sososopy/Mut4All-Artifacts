use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, File, Item, ReturnType, Type, parse_quote,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Modify_Const_Expr_In_Return_345;

impl Mutator for Modify_Const_Expr_In_Return_345 {
    fn name(&self) -> &str {
        "Modify_Const_Expr_In_Return_345"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(type_path) = &mut **ty {
                        if let Some(segment) = type_path.path.segments.last_mut() {
                            if let syn::PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                                for arg in &mut args.args {
                                    if let syn::GenericArgument::Const(ref mut expr) = arg {
                                        let new_expr: Expr = parse_quote! { #expr + 1 * 2 };
                                        *expr = new_expr;
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
        ""
    }
}