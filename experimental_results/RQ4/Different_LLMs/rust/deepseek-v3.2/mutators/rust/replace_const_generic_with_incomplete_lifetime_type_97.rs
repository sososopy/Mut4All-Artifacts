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

pub struct Replace_Const_Generic_With_Incomplete_Lifetime_Type_97;

impl Mutator for Replace_Const_Generic_With_Incomplete_Lifetime_Type_97 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Incomplete_Lifetime_Type_97"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut generic_types_with_lifetimes = Vec::new();
        let mut has_lifetime_generic = false;
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.generics.params.iter().any(|p| matches!(p, GenericParam::Lifetime(_))) {
                    generic_types_with_lifetimes.push(item_struct.ident.clone());
                    has_lifetime_generic = true;
                }
            }
            if let Item::Enum(item_enum) = item {
                if item_enum.generics.params.iter().any(|p| matches!(p, GenericParam::Lifetime(_))) {
                    generic_types_with_lifetimes.push(item_enum.ident.clone());
                    has_lifetime_generic = true;
                }
            }
        }
        if !has_lifetime_generic {
            let new_struct = Item::Struct(ItemStruct {
                attrs: Vec::new(),
                vis: syn::Visibility::Inherited,
                struct_token: token::Struct::default(),
                ident: Ident::new("GenericLifetime", Span::call_site()),
                generics: {
                    let mut generics = Generics::default();
                    generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: Lifetime::new("'a", Span::call_site()),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                    generics
                },
                fields: syn::Fields::Unnamed(syn::FieldsUnnamed {
                    paren_token: token::Paren::default(),
                    unnamed: Punctuated::from_iter(vec![syn::Field {
                        attrs: Vec::new(),
                        vis: syn::Visibility::Inherited,
                        ident: None,
                        colon_token: None,
                        mutability: syn::FieldMutability::None,
                        ty: syn::Type::Reference(syn::TypeReference {
                            and_token: token::And::default(),
                            lifetime: Some(Lifetime::new("'a", Span::call_site())),
                            mutability: None,
                            elem: Box::new(syn::Type::Tuple(syn::TypeTuple {
                                paren_token: token::Paren::default(),
                                elems: Punctuated::new(),
                            })),
                        }),
                    }]),
                }),
                semi_token: Some(token::Semi::default()),
            });
            file.items.insert(0, new_struct);
            generic_types_with_lifetimes.push(Ident::new("GenericLifetime", Span::call_site()));
        }
        let target_generic_type = generic_types_with_lifetimes.choose(&mut thread_rng()).cloned().unwrap_or_else(|| Ident::new("GenericLifetime", Span::call_site()));
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if type_path.path.segments.len() == 1 && type_path.path.segments[0].arguments.is_none() {
                                const_param.ty = Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: target_generic_type.clone(),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator locates struct definitions with const generic parameters and replaces the const parameter's type with a generic type that requires lifetime arguments, but omits those arguments. If no suitable generic type exists in the seed program, it introduces a new generic struct with lifetime parameters at the module's top. This creates incomplete generic type usage, mimicking bug-triggering patterns where const parameters have generic ADTs lacking required lifetime arguments, potentially causing compiler errors or ICEs in const generics resolution."
    }
}