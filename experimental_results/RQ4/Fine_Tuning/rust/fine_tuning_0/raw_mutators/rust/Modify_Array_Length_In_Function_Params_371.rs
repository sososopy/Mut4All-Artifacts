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

pub struct Modify_Array_Length_In_Function_Params_371;

impl Mutator for Modify_Array_Length_In_Function_Params_371 {
    fn name(&self) -> &str {
        "Modify_Array_Length_In_Function_Params_371"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                let mut new_params = Vec::new();
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Array(type_array) = &*pat_type.ty {
                            if let Expr::Lit(expr_lit) = &type_array.len {
                                if let syn::Lit::Int(_) = &expr_lit.lit {
                                    let len_ident = Ident::new("len", Span::call_site());
                                    new_params.push(parse_quote!(#len_ident: usize));
                                    pat_type.ty = Box::new(Type::Array(TypeArray {
                                        elem: type_array.elem.clone(),
                                        len: Box::new(Expr::Path(ExprPath {
                                            path: SynPath::from(len_ident),
                                            qself: None,
                                        })),
                                        bracket_token: type_array.bracket_token,
                                    }));
                                }
                            }
                        }
                    }
                }

                func.sig.inputs.extend(new_params);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies function parameters that are arrays with constant lengths by replacing the constant length with a new parameter. This transformation introduces dynamic length specification, potentially causing type inference issues and testing the compiler's handling of parameter dependencies and array length evaluation."
    }
}