use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashMap, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Inline_Assembly_Function_Reference_Parameter_173;

impl Mutator for Inline_Assembly_Function_Reference_Parameter_173 {
    fn name(&self) -> &str {
        "Inline_Assembly_Function_Reference_Parameter_173"
    }
    fn mutate(&self, file: &mut syn::File) {
        // First pass: collect functions
        let mut functions = HashMap::new();
        for item in &file.items {
            if let Item::Fn(func) = item {
                functions.insert(func.sig.ident.clone(), func);
            }
        }

        // Second pass: process macros
        for item in &mut file.items {
            if let Item::Macro(macro_item) = item {
                if is_asm_macro(&macro_item.mac.path) {
                    let macro_tokens = macro_item.mac.tokens.to_string();
                    let re = Regex::new(r"sym\s+(\w+)").unwrap();
                    for cap in re.captures_iter(&macro_tokens) {
                        if let Some(m) = cap.get(1) {
                            let func_name = m.as_str();
                            if let Some(func) = functions.get_mut(&Ident::new(func_name, Span::call_site())) {
                                modify_function_parameters(func);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions referenced by inline assembly via `sym` in `asm!` macros. It transforms one concrete-type parameter of the referenced function into a reference (`&T` or `&mut T`), altering the function's signature. This forces the compiler to handle indirect function calls with reference parameters, potentially exposing bugs in type handling, symbol resolution, or inline assembly integration."
    }
}

fn is_asm_macro(path: &syn::Path) -> bool {
    path.segments.last().map_or(false, |seg| seg.ident == "asm")
}

fn modify_function_parameters(func: &mut ItemFn) {
    let mut rng = thread_rng();
    let mut valid_args: Vec<&mut PatType> = func.sig.inputs.iter_mut()
        .filter_map(|arg| {
            if let FnArg::Typed(pat_type) = arg {
                if let Type::Reference(_) = *pat_type.ty {
                    None
                } else {
                    Some(pat_type)
                }
            } else {
                None
            }
        })
        .collect();

    if valid_args.is_empty() {
        return;
    }

    let selected = valid_args.choose(&mut rng).unwrap();
    let original_type = selected.ty.clone();
    let is_mut = rng.gen_bool(0.5);
    let new_type = if is_mut {
        parse_quote! { &mut #original_type }
    } else {
        parse_quote! { & #original_type }
    };
    *selected.ty = new_type;
}