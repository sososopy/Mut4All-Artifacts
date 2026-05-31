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

pub struct Modify_Function_Return_Type_With_Incomplete_Impl_Trait_120;

impl Mutator for Modify_Function_Return_Type_With_Incomplete_Impl_Trait_120 {
    fn name(&self) -> &str {
        "Modify_Function_Return_Type_With_Incomplete_Impl_Trait_120"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        // Remove the trait specification after `impl`
                        let mut new_output: ReturnType = parse_quote! { -> impl };
                        item_fn.sig.output = new_output;

                        // Modify the function body to include a `yield` statement
                        let yield_expr: Expr = parse_quote! { yield 1; };
                        let mut new_block = item_fn.block.clone();
                        new_block.stmts.push(Stmt::Expr(yield_expr, None));
                        item_fn.block = new_block;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions with `impl Trait` return types, removing the trait specification to create an incomplete type declaration. It also adds a `yield` statement to the function body, which is typically used in generators, challenging the compiler's type inference and coroutine handling capabilities."
    }
}