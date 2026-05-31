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

pub struct Array_Element_Type_Substitution_663;

impl Mutator for Array_Element_Type_Substitution_663 {
    fn name(&self) -> &str {
        "Array_Element_Type_Substitution_663"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(init) = &mut local.init {
                                if let Expr::Array(array) = &mut *init.expr {
                                    if let Some(first_elem) = array.elems.first() {
                                        if let Expr::Call(first_call) = first_elem {
                                            if let Expr::Path(ExprPath { path, .. }) = &*first_call.func {
                                                if path.segments.last().unwrap().ident == "mk_gen" {
                                                    let new_struct: ItemStruct = parse_quote! {
                                                        struct MyGen;
                                                    };
                                                    let new_impl: Item = parse_quote! {
                                                        impl Generator for MyGen {
                                                            type Yield = ();
                                                            type Return = !;
                                                            fn resume(&mut self) -> std::ops::GeneratorState<(), !> {
                                                                loop { yield; }
                                                            }
                                                        }
                                                    };
                                                    new_items.push(Item::Struct(new_struct));
                                                    new_items.push(new_impl);

                                                    let new_array: Expr = parse_quote! {
                                                        [MyGen, MyGen]
                                                    };
                                                    *init.expr = new_array;
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
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets arrays initialized with `impl Trait` elements, specifically those implementing the `Generator` trait. It replaces the `impl Trait` element type with a concrete struct `MyGen` that implements the required trait. This transformation tests the compiler's ability to handle type inference and binding inconsistencies within arrays by introducing a concrete type substitution."
    }
}