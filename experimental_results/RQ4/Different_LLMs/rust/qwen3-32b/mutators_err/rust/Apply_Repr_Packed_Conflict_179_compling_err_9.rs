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

pub struct Apply_Repr_Packed_Conflict_179;

impl Mutator for Apply_Repr_Packed_Conflict_179 {
    fn name(&self) -> &str {
        "Apply_Repr_Packed_Conflict_179"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                let mut max_alignment = 0;
                let mut fields_with_alignment = Vec::new();
                if let syn::Fields::Named(named) = &struct_item.fields {
                    for field in &named.named {
                        if let syn::Type::Path(type_path) = &field.ty {
                            for segment in &type_path.path.segments {
                                if let Some(align) = match segment.ident.to_string().as_str() {
                                    "u8" => Some(1),
                                    "u16" => Some(2),
                                    "u32" => Some(4),
                                    "u64" => Some(8),
                                    "usize" => Some(8),
                                    "i8" => Some(1),
                                    "i16" => Some(2),
                                    "i32" => Some(4),
                                    "i64" => Some(8),
                                    "isize" => Some(8),
                                    _ => None,
                                } {
                                    if align > max_alignment {
                                        max_alignment = align;
                                    }
                                    fields_with_alignment.push((field, align));
                                    break;
                                }
                            }
                        }
                    }
                }
                if max_alignment >= 2 {
                    let n = 1; // Smallest power of two less than max_alignment
                    let attr = parse_quote!(#[repr(packed(#n))]);
                    struct_item.attrs.push(attr);
                    if let syn::Fields::Named(named) = &struct_item.fields {
                        if fields_with_alignment.iter().any(|(_, a)| *a > n) {
                            let field_names: Vec<_> = named.named.iter()
                                .map(|f| f.ident.as_ref().unwrap().clone())
                                .collect();
                            let const_name = Ident::new("C", Span::call_site());
                            let generics = struct_item.generics.to_token_stream();
                            let const_decl = parse_quote! {
                                const #const_name: #struct_item.ident #generics = #struct_item.ident { #(#field_names: 0),* };
                            };
                            new_items.push(Item::Const(const_decl));
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