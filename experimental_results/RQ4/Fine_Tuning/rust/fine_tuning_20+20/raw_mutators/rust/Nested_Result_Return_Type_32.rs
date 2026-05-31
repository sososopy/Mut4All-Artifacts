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

pub struct Nested_Result_Return_Type_32;

impl Mutator for Nested_Result_Return_Type_32 {
    fn name(&self) -> &str {
        "Nested_Result_Return_Type_32"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if let Some(last_segment) = type_path.path.segments.last() {
                            if last_segment.ident == "Result" {
                                let new_return_type: Type = parse_quote! {
                                    Result<#return_type, <#return_type as std::ops::Try>::Residual>
                                };
                                item_fn.sig.output =
                                    ReturnType::Type(Default::default(), Box::new(new_return_type));
                                let mut new_block = item_fn.block.clone();
                                for stmt in &mut new_block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        *expr = parse_quote! { Ok(#expr) };
                                    }
                                }
                                item_fn.block = new_block;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions returning a `Result` type and transforms the return type into a nested `Result<Result<T, E>, E>` structure. It modifies the function body to wrap existing return expressions with an additional `Ok` layer. This mutation stresses the borrow checker by introducing complex nested result types, potentially revealing issues in lifetime and ownership management."
    }
}