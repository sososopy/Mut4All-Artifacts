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

pub struct Modify_Array_Dimensions_In_Structs_35;

impl Mutator for Modify_Array_Dimensions_In_Structs_35 {
    fn name(&self) -> &str {
        "Modify_Array_Dimensions_In_Structs_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Array(type_array) = &mut field.ty {
                        if let Expr::Lit(expr_lit) = &mut type_array.len {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                let current_size = lit_int.base10_parse::<usize>().unwrap_or(0);
                                let mut rng = thread_rng();
                                let modify_choice = rng.gen_range(0..3);
                                let new_size_expr = match modify_choice {
                                    0 => Expr::Lit(ExprLit {
                                        attrs: vec![],
                                        lit: syn::Lit::Int(syn::LitInt::new(
                                            &(current_size + 1).to_string(),
                                            Span::call_site(),
                                        )),
                                    }),
                                    1 if current_size > 1 => Expr::Lit(ExprLit {
                                        attrs: vec![],
                                        lit: syn::Lit::Int(syn::LitInt::new(
                                            &(current_size - 1).to_string(),
                                            Span::call_site(),
                                        )),
                                    }),
                                    _ => Expr::Binary(syn::ExprBinary {
                                        attrs: vec![],
                                        left: Box::new(Expr::Lit(ExprLit {
                                            attrs: vec![],
                                            lit: syn::Lit::Int(syn::LitInt::new(
                                                "2",
                                                Span::call_site(),
                                            )),
                                        })),
                                        op: syn::BinOp::Add(Default::default()),
                                        right: Box::new(Expr::Lit(ExprLit {
                                            attrs: vec![],
                                            lit: syn::Lit::Int(syn::LitInt::new(
                                                &(current_size - 2).to_string(),
                                                Span::call_site(),
                                            )),
                                        })),
                                    }),
                                };
                                type_array.len = new_size_expr;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct field declarations containing arrays with fixed dimensions. It modifies the dimensions by increasing, decreasing, or replacing them with a constant expression. This tests the compiler's handling of array sizes in generic constant expressions, potentially uncovering issues in size evaluation or boundary checking."
    }
}