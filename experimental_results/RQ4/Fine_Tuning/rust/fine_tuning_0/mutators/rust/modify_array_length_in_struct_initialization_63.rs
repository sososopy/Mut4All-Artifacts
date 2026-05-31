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

pub struct Modify_Array_Length_In_Struct_Initialization_63;

impl Mutator for Modify_Array_Length_In_Struct_Initialization_63 {
    fn name(&self) -> &str {
        "Modify_Array_Length_In_Struct_Initialization_63"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Array(type_array) = &mut field.ty {
                        if let Expr::Lit(expr_lit) = &mut type_array.len {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                if let Ok(value) = lit_int.base10_parse::<usize>() {
                                    let new_value = value + 1; // Increase the array size by 1
                                    type_array.len = parse_quote! { #new_value };
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
                                for field in &mut expr_struct.fields {
                                    if let Expr::Array(expr_array) = &mut field.expr {
                                        if let Some(Expr::Lit(ExprLit { lit: syn::Lit::Int(lit_int), .. })) = expr_array.elems.first_mut() {
                                            if let Ok(value) = lit_int.base10_parse::<usize>() {
                                                let new_value = value + 1; // Reflect the change in initialization
                                                *lit_int = syn::LitInt::new(&new_value.to_string(), lit_int.span());
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
        "The mutation operator targets struct definitions with array fields initialized using constant expressions. It modifies the length of such arrays by increasing them by one, ensuring that the initialization reflects the new size. This transformation tests the compiler's ability to handle changes in array sizes within structs, potentially revealing issues in constant evaluation and type checking."
    }
}