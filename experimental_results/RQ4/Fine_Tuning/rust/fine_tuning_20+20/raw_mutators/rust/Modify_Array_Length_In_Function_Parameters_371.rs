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

pub struct Modify_Array_Length_In_Function_Parameters_371;

impl Mutator for Modify_Array_Length_In_Function_Parameters_371 {
    fn name(&self) -> &str {
        "Modify_Array_Length_In_Function_Parameters_371"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_params = Vec::new();
                for input in &func.sig.inputs {
                    if let syn::FnArg::Typed(pat_type) = input {
                        if let syn::Type::Array(type_array) = &*pat_type.ty {
                            if let syn::Expr::Path(expr_path) = &type_array.len {
                                if expr_path.path.segments.len() == 1 {
                                    let len_ident = expr_path.path.segments[0].ident.clone();
                                    let new_ty = syn::Type::Slice(syn::TypeSlice {
                                        bracket_token: type_array.bracket_token,
                                        elem: type_array.elem.clone(),
                                    });
                                    let new_pat_type = syn::PatType {
                                        attrs: pat_type.attrs.clone(),
                                        pat: pat_type.pat.clone(),
                                        colon_token: pat_type.colon_token,
                                        ty: Box::new(syn::Type::Reference(syn::TypeReference {
                                            and_token: Default::default(),
                                            lifetime: None,
                                            mutability: None,
                                            elem: Box::new(new_ty),
                                        })),
                                    };
                                    new_params.push(syn::FnArg::Typed(new_pat_type));
                                    new_params.push(syn::FnArg::Typed(syn::PatType {
                                        attrs: Vec::new(),
                                        pat: Box::new(syn::Pat::Ident(syn::PatIdent {
                                            attrs: Vec::new(),
                                            by_ref: None,
                                            mutability: None,
                                            ident: len_ident,
                                            subpat: None,
                                        })),
                                        colon_token: Default::default(),
                                        ty: Box::new(syn::Type::Path(syn::TypePath {
                                            qself: None,
                                            path: syn::Path::from(syn::Ident::new(
                                                "usize",
                                                Span::call_site(),
                                            )),
                                        })),
                                    }));
                                    continue;
                                }
                            }
                        }
                    }
                    new_params.push(input.clone());
                }
                func.sig.inputs = syn::punctuated::Punctuated::from_iter(new_params);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function parameters that are arrays with constant length expressions. It transforms such parameters into slices and introduces a new usize parameter to represent the length, using the original constant expression as the new parameter's name. This mutation leverages Rust's const generics and slice types to stress-test the compiler's handling of parameter-dependent array lengths, potentially triggering ICEs related to const evaluation and type inference."
    }
}