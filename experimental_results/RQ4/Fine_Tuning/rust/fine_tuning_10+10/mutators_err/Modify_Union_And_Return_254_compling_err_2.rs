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

pub struct Modify_Union_And_Return_254;

impl Mutator for Modify_Union_And_Return_254 {
    fn name(&self) -> &str {
        "Modify_Union_And_Return_254"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = item {
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::Path(type_path) = &**return_type {
                                if type_path.path.segments.last().unwrap().ident == "Option" {
                                    if let Some(union_index) = method.block.stmts.iter().position(|stmt| {
                                        if let Stmt::Item(Item::Union(_)) = stmt {
                                            true
                                        } else {
                                            false
                                        }
                                    }) {
                                        if let Stmt::Item(Item::Union(union_item)) = &mut method.block.stmts[union_index] {
                                            let field_type = union_item.fields.named.iter().next().unwrap().ty.clone();
                                            let new_field = syn::Field {
                                                attrs: vec![],
                                                vis: syn::Visibility::Inherited,
                                                ident: Some(Ident::new("b", Span::call_site())),
                                                colon_token: Some(token::Colon::default()),
                                                mutability: None,
                                                ty: field_type.clone(),
                                            };
                                            union_item.fields.named.push(new_field);
                                            method.sig.output = parse_quote!((Option<<Self as Iterator>::Item>, Option<U>));
                                            method.block.stmts.push(parse_quote!(return (None, None);));
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
        "The mutation operator targets functions within impl blocks that use a union. It modifies the union by adding a new field of the same type, changes the function's return type to a tuple with two Option elements, and updates the function logic to return this tuple. This approach ensures broad application across different codebases by focusing on type-based matching rather than specific type names."
    }
}