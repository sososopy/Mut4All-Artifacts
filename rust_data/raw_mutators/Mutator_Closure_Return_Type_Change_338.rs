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

pub struct Mutator_Closure_Return_Type_Change_338;

impl Mutator for Mutator_Closure_Return_Type_Change_338 {
    fn name(&self) -> &str {
        "Mutator_Closure_Return_Type_Change_338"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &mut local.init {
                            if let Expr::Closure(closure) = expr.as_mut() {
                                if let Some(output_type) = &closure.output {
                                    if let ReturnType::Type(_, ty) = output_type {
                                        if let Type::Path(type_path) = ty.as_ref() {
                                            let original_type = &type_path.path.segments.last().unwrap().ident;
                                            let new_type = match original_type.to_string().as_str() {
                                                "u8" => "i16",
                                                "i32" => "i64",
                                                _ => continue,
                                            };
                                            let new_type_ident = Ident::new(new_type, Span::call_site());
                                            *ty = parse_quote!(#new_type_ident);

                                            if let Expr::Block(block) = &mut *closure.body {
                                                for stmt in &mut block.block.stmts {
                                                    if let Stmt::Expr(expr) = stmt {
                                                        if let Expr::Lit(expr_lit) = expr {
                                                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                                let value = lit_int.base10_parse::<i64>().unwrap();
                                                                *expr = parse_quote!(#value as #new_type_ident);
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets closures within functions, specifically altering their return types. It identifies closures with primitive return types and modifies them to another compatible primitive type, such as changing 'u8' to 'i16'. The operator ensures the return expression within the closure is adjusted to match the new type, using type casting where necessary. This approach aims to reveal potential issues in type conversion and optimization processes."
    }
}