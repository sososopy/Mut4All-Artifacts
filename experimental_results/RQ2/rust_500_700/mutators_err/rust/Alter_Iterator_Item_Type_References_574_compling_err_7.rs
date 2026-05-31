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

pub struct Alter_Iterator_Item_Type_References_574;

impl Mutator for Alter_Iterator_Item_Type_References_574 {
    fn name(&self) -> &str {
        "Alter_Iterator_Item_Type_References_574"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.segments.last().unwrap().ident == "Iterator" {
                                    let last_segment = trait_bound.path.segments.last_mut().unwrap();
                                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                        for arg in &mut args.args {
                                            if let GenericArgument::Type(Type::Reference(type_reference)) = arg {
                                                let new_item_type = if let Type::Tuple(tuple) = &*type_reference.elem {
                                                    Type::Tuple(tuple.clone())
                                                } else {
                                                    continue;
                                                };

                                                let map_expr: Expr = parse_quote! {
                                                    .map(|item| {
                                                        let (a, b) = item;
                                                        (a, b)
                                                    })
                                                };

                                                item_fn.block.stmts.push(Stmt::Expr(map_expr, None));
                                                *arg = GenericArgument::Type(new_item_type);
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
        "This mutation operator targets functions returning `impl Iterator` with reference `Item` types. It modifies the `Item` type by dereferencing it to an owned type, ensuring the function body handles this change correctly. This explores potential compiler bugs in reference handling and lifetime management within `impl Iterator` contexts."
    }
}