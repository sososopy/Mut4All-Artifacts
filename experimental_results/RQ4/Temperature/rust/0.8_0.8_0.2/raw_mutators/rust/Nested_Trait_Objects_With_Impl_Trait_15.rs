use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprBlock, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Nested_Trait_Objects_With_Impl_Trait_15;

impl Mutator for Nested_Trait_Objects_With_Impl_Trait_15 {
    fn name(&self) -> &str {
        "Nested_Trait_Objects_With_Impl_Trait_15"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let nested_trait_object: Expr = parse_quote! {
                            Box::new({
                                let wrapped = Box::new(std::iter::once(1));
                                Box::new(std::iter::once(wrapped)) as Box<dyn Iterator<Item = _>>
                            }) as Box<dyn Iterator<Item = _>>
                        };
                        
                        let mut new_block = item_fn.block.clone();
                        new_block.stmts.clear();
                        new_block.stmts.push(Stmt::Expr(nested_trait_object, None));
                        item_fn.block = new_block;
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutation operator specifically targets functions that return `impl Trait`, introducing nested layers of trait objects using `dyn Trait`. It aims to stress the Rust compiler by increasing the complexity of trait object coercion and unsizing, potentially uncovering issues related to nested `impl Trait` and `dyn Trait` interactions."
    }
}