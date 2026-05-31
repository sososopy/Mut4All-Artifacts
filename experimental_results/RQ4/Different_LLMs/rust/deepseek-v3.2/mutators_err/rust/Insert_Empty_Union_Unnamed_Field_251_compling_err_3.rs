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

pub struct Insert_Empty_Union_Unnamed_Field_251;

impl Mutator for Insert_Empty_Union_Unnamed_Field_251 {
    fn name(&self) -> &str {
        "Insert_Empty_Union_Unnamed_Field_251"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let syn::Fields::Named(fields) = &mut struct_item.fields {
                    let new_field = syn::Field {
                        attrs: Vec::new(),
                        vis: syn::Visibility::Inherited,
                        mutability: syn::FieldMutability::None,
                        ident: None,
                        colon_token: Some(token::Colon::default()),
                        ty: syn::Type::Verbatim(quote! { union {} }),
                    };
                    fields.named.push(new_field);
                } else if let syn::Fields::Unit = &mut struct_item.fields {
                    let mut punctuated = Punctuated::new();
                    let new_field = syn::Field {
                        attrs: Vec::new(),
                        vis: syn::Visibility::Inherited,
                        mutability: syn::FieldMutability::None,
                        ident: None,
                        colon_token: Some(token::Colon::default()),
                        ty: syn::Type::Verbatim(quote! { union {} }),
                    };
                    punctuated.push(new_field);
                    struct_item.fields = syn::Fields::Named(syn::FieldsNamed {
                        brace_token: token::Brace::default(),
                        named: punctuated,
                    });
                }
            }
            if let Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    if let syn::Fields::Named(fields) = &mut variant.fields {
                        let new_field = syn::Field {
                            attrs: Vec::new(),
                            vis: syn::Visibility::Inherited,
                            mutability: syn::FieldMutability::None,
                            ident: None,
                            colon_token: Some(token::Colon::default()),
                            ty: syn::Type::Verbatim(quote! { union {} }),
                        };
                        fields.named.push(new_field);
                    } else if let syn::Fields::Unit = &mut variant.fields {
                        let mut punctuated = Punctuated::new();
                        let new_field = syn::Field {
                            attrs: Vec::new(),
                            vis: syn::Visibility::Inherited,
                            mutability: syn::FieldMutability::None,
                            ident: None,
                            colon_token: Some(token::Colon::default()),
                            ty: syn::Type::Verbatim(quote! { union {} }),
                        };
                        punctuated.push(new_field);
                        variant.fields = syn::Fields::Named(syn::FieldsNamed {
                            brace_token: token::Brace::default(),
                            named: punctuated,
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts an unnamed field with an empty union type into struct and enum variant definitions. This transformation exploits the compiler's handling of unnamed fields and empty unions, potentially causing missing data in the AST and triggering ICEs during pretty-printing or other phases."
    }
}