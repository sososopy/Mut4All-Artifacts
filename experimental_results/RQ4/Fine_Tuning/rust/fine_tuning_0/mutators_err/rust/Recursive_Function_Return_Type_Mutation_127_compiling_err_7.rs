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

pub struct Recursive_Function_Return_Type_Mutation_127;

impl Mutator for Recursive_Function_Return_Type_Mutation_127 {
    fn name(&self) -> &str {
        "Recursive_Function_Return_Type_Mutation_127"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut has_recursive_call = false;
                        for stmt in &func.block.stmts {
                            if let Stmt::Expr(Expr::Call(ExprCall { func: call_func, .. })) = stmt {
                                if let Expr::Path(ExprPath { path, .. }) = &**call_func {
                                    if path.is_ident(&func.sig.ident) {
                                        has_recursive_call = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if has_recursive_call {
                            let new_trait: Item = parse_quote! {
                                trait NewTrait {}
                            };
                            let impl_trait: Item = parse_quote! {
                                impl NewTrait for u32 {}
                            };
                            new_items.push(new_trait);
                            new_items.push(impl_trait);

                            let new_return_type: Type = parse_quote! { NewTrait };
                            func.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));
                        }
                    }
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}