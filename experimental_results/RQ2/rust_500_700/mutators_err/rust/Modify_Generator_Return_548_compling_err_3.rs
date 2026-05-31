use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Modify_Generator_Return_548;

impl Mutator for Modify_Generator_Return_548 {
    fn name(&self) -> &str {
        "Modify_Generator_Return_548"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        if item_fn.block.stmts.iter().any(|stmt| matches!(stmt, Stmt::Expr(Expr::Yield(_), _))) {
                            let new_return_type: Type = parse_quote!(Vec<i32>);
                            item_fn.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));
                            
                            let mut new_block = syn::Block {
                                brace_token: item_fn.block.brace_token,
                                stmts: vec![],
                            };
                            
                            new_block.stmts.push(parse_quote! {
                                let mut result = Vec::new();
                            });
                            
                            for stmt in &item_fn.block.stmts {
                                if let Stmt::Expr(Expr::Yield(yield_expr), _) = stmt {
                                    if let Some(expr) = &yield_expr.expr {
                                        new_block.stmts.push(parse_quote! {
                                            result.push(#expr);
                                        });
                                    }
                                }
                            }
                            
                            new_block.stmts.push(parse_quote! {
                                result
                            });
                            
                            item_fn.block = Box::new(new_block);
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