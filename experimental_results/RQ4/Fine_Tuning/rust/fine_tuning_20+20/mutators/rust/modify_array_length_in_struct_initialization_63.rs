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

pub struct Modify_Array_Length_In_Struct_Initialization_63;

impl Mutator for Modify_Array_Length_In_Struct_Initialization_63 {
    fn name(&self) -> &str {
        "Modify_Array_Length_In_Struct_Initialization_63"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Fields::Named(fields_named) = &mut item_struct.fields {
                    for field in &mut fields_named.named {
                        if let Type::Array(type_array) = &field.ty {
                            if let Expr::Lit(expr_lit) = &type_array.len {
                                if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                    if let Ok(n) = lit_int.base10_parse::<usize>() {
                                        let new_len = n + 1;
                                        field.ty = Type::Array(TypeArray {
                                            elem: type_array.elem.clone(),
                                            len: Expr::Lit(ExprLit {
                                                attrs: Vec::new(),
                                                lit: syn::Lit::Int(syn::LitInt::new(
                                                    &new_len.to_string(),
                                                    lit_int.span(),
                                                )),
                                            }),
                                            bracket_token: type_array.bracket_token,
                                            semi_token: type_array.semi_token,
                                        });
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        for stmt in &mut method.block.stmts {
                            if let Stmt::Expr(Expr::Struct(expr_struct), _) = stmt {
                                if let Some(path) = expr_struct.path.get_ident() {
                                    if path == "Sample" {
                                        for field in &mut expr_struct.fields {
                                            if field.member == Member::Named(Ident::new("items", Span::call_site())) {
                                                field.expr = Expr::Array(ExprArray {
                                                    attrs: Vec::new(),
                                                    bracket_token: token::Bracket::default(),
                                                    elems: {
                                                        let mut elems = Punctuated::new();
                                                        elems.push(Expr::Lit(ExprLit {
                                                            attrs: Vec::new(),
                                                            lit: syn::Lit::Int(syn::LitInt::new(
                                                                "0",
                                                                Span::call_site(),
                                                            )),
                                                        }));
                                                        elems
                                                    },
                                                });
                                            }
                                        }
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
        "This mutator targets struct definitions and impl blocks that initialize array fields with a fixed length. It increases the array length by 1 in both the struct definition and the corresponding initialization expression within the impl block. This transformation stresses the compiler's handling of array lengths, particularly in constant evaluation and layout computation, by introducing a subtle change that can affect memory layout and type checking."
    }
}