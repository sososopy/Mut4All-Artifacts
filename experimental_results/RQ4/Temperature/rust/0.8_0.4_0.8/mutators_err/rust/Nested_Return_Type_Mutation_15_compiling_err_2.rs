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

pub struct Nested_Return_Type_Mutation_15;

impl Mutator for Nested_Return_Type_Mutation_15 {
    fn name(&self) -> &str {
        "Nested_Return_Type_Mutation_15"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut new_block = item_fn.block.clone();
                        let wrapper_struct: ItemStruct = parse_quote! {
                            struct Wrapper<T: ?Sized>(Box<T>);
                        };
                        let wrapper_impl: Item = parse_quote! {
                            impl<T: ?Sized> std::ops::Deref for Wrapper<T> {
                                type Target = T;
                                fn deref(&self) -> &Self::Target {
                                    &self.0
                                }
                            }
                        };
                        file.items.push(Item::Struct(wrapper_struct));
                        file.items.push(wrapper_impl);

                        let new_return_type: Type = parse_quote! {
                            Wrapper<dyn #type_impl_trait>
                        };
                        item_fn.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));

                        for stmt in &mut new_block.stmts {
                            if let Stmt::Expr(expr, semi) = stmt {
                                *stmt = Stmt::Expr(parse_quote! {
                                    Wrapper(Box::new(#expr) as Box<dyn #type_impl_trait>)
                                }, semi.clone());
                            }
                        }
                        item_fn.block = new_block;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions returning `impl Trait` types, wrapping their return values in a `Wrapper` struct that implements `Deref`. This transformation complicates the return type, potentially exposing issues in the compiler's type inference and coercion logic when dealing with nested `impl Trait` constructs."
    }
}