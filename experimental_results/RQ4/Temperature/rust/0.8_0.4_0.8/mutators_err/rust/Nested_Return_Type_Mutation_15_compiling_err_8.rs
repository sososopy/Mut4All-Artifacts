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

pub struct NestedReturnTypeMutation15;

impl Mutator for NestedReturnTypeMutation15 {
    fn name(&self) -> &str {
        "Nested_Return_Type_Mutation_15"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut additional_items = Vec::new();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let return_type = match &item_fn.sig.output {
                    ReturnType::Type(_, return_type) => return_type.clone(),
                    _ => continue,
                };
                
                if let Type::ImplTrait(type_impl_trait) = &*return_type {
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
                    additional_items.push(Item::Struct(wrapper_struct));
                    additional_items.push(wrapper_impl);

                    let new_return_type: Type = parse_quote! {
                        Wrapper<dyn #type_impl_trait>
                    };

                    let boxed_new_return_type = Box::new(new_return_type);
                    item_fn.sig.output = ReturnType::Type(Default::default(), boxed_new_return_type);

                    for stmt in &mut new_block.stmts {
                        if let Stmt::Expr(expr) = stmt {
                            *stmt = Stmt::Expr(parse_quote! {
                                Wrapper(Box::new(#expr) as Box<dyn #type_impl_trait>)
                            });
                        }
                    }
                    item_fn.block = new_block;
                }
            }
        }
        file.items.extend(additional_items);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions returning `impl Trait` types, wrapping their return values in a `Wrapper` struct that implements `Deref`. This transformation complicates the return type, potentially exposing issues in the compiler's type inference and coercion logic when dealing with nested `impl Trait` constructs."
    }
}