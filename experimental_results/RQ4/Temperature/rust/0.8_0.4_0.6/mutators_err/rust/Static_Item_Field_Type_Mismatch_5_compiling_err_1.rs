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

pub struct Static_Item_Field_Type_Mismatch_5;

impl Mutator for Static_Item_Field_Type_Mismatch_5 {
    fn name(&self) -> &str {
        "Static_Item_Field_Type_Mismatch_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut rng = thread_rng();
                if item_struct.fields.len() > 1 {
                    if let syn::Fields::Named(fields_named) = &mut item_struct.fields {
                        let field_to_mutate = fields_named.named.iter_mut().choose(&mut rng);
                        if let Some(field) = field_to_mutate {
                            field.ty = Box::new(syn::Type::Reference(syn::TypeReference {
                                and_token: token::And::default(),
                                lifetime: None,
                                mutability: None,
                                elem: Box::new(syn::Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path::from(Ident::new("str", Span::call_site())),
                                })),
                            }));
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let syn::Type::Path(type_path) = &*item_static.ty {
                    if let Some(last_segment) = type_path.path.segments.last() {
                        if last_segment.ident == "Foo" {
                            if let syn::Expr::Struct(expr_struct) = &mut *item_static.expr {
                                for field_value in &mut expr_struct.fields {
                                    if field_value.member == syn::Member::Named(Ident::new("field1", Span::call_site())) {
                                        field_value.expr = Box::new(syn::Expr::Lit(syn::ExprLit {
                                            attrs: Vec::new(),
                                            lit: syn::Lit::Str(syn::LitStr::new("text", Span::call_site())),
                                        }));
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
        "This mutator targets structs used in static items, changing the type of one field to an incompatible type and adjusting the static initialization accordingly. It aims to expose potential type mismatch errors in constant contexts by altering struct field types and testing the compiler's type checking in static item initializations."
    }
}