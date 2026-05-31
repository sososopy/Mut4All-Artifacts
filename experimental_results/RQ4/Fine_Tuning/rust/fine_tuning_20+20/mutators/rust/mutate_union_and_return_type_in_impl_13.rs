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

pub struct Mutate_Union_And_Return_Type_In_Impl_13;

impl Mutator for Mutate_Union_And_Return_Type_In_Impl_13 {
    fn name(&self) -> &str {
        "Mutate_Union_And_Return_Type_In_Impl_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let mut has_union = false;
                        for stmt in &method.block.stmts {
                            if let Stmt::Item(syn::Item::Union(_)) = stmt {
                                has_union = true;
                                break;
                            }
                        }
                        if !has_union {
                            continue;
                        }
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            let mut new_return_type: Type = parse_quote!(());
                            if let Type::Path(type_path) = &**return_type {
                                if let Some(last_segment) = type_path.path.segments.last() {
                                    if last_segment.ident == "Option" {
                                        new_return_type =
                                            parse_quote!((Option<<Self as Iterator>::Item>, Option<U>));
                                    }
                                }
                            }
                            method.sig.output = ReturnType::Type(
                                token::RArrow {
                                    spans: [Span::call_site(), Span::call_site()],
                                },
                                Box::new(new_return_type),
                            );
                        }
                        let mut new_stmts = Vec::new();
                        for stmt in &method.block.stmts {
                            if let Stmt::Item(syn::Item::Union(union_item)) = stmt {
                                let mut new_fields = union_item.fields.clone();
                                if let Some(first_field) = union_item.fields.named.first() {
                                    new_fields.named.push(first_field.clone());
                                }
                                let new_union_item = syn::ItemUnion {
                                    attrs: union_item.attrs.clone(),
                                    vis: union_item.vis.clone(),
                                    union_token: union_item.union_token,
                                    ident: union_item.ident.clone(),
                                    generics: union_item.generics.clone(),
                                    fields: new_fields,
                                };
                                new_stmts.push(Stmt::Item(syn::Item::Union(new_union_item)));
                            } else {
                                new_stmts.push(stmt.clone());
                            }
                        }
                        method.block.stmts = new_stmts;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions within impl blocks that use a union. It modifies the union by duplicating an existing field and changes the function's return type to a tuple of two Options, one for the original return type and one for the union type. This transformation stresses the compiler's handling of union field access, type inference, and return type normalization, particularly in the context of trait implementations."
    }
}