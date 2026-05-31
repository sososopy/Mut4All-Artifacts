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

pub struct Replace_Const_Generic_Type_With_Non_Supported_Type_397;

fn is_supported_const_generic_type(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Path(TypePath { path, .. })
            if path.is_ident("usize") || path.is_ident("isize") || path.is_ident("bool") || path.is_ident("char")
    )
}

fn find_non_supported_type_in_file(file: &syn::File) -> Option<Type> {
    for item in &file.items {
        if let Item::Struct(item_struct) = item {
            return Some(Type::Path(TypePath {
                qself: None,
                path: item_struct.ident.clone().into(),
            }));
        }
    }
    None
}

fn create_placeholder_type() -> Type {
    Type::Path(TypePath {
        qself: None,
        path: Ident::new("PlaceholderType", Span::call_site()).into(),
    })
}

fn generate_placeholder_type_declaration() -> Item {
    Item::Struct(ItemStruct {
        attrs: Vec::new(),
        vis: syn::Visibility::Inherited,
        struct_token: token::Struct {
            span: Span::call_site(),
        },
        ident: Ident::new("PlaceholderType", Span::call_site()),
        generics: Default::default(),
        fields: syn::Fields::Unit,
        semi_token: Some(token::Semi {
            spans: [Span::call_site()],
        }),
    })
}

impl Mutator for Replace_Const_Generic_Type_With_Non_Supported_Type_397 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Type_With_Non_Supported_Type_397"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut found = false;
        let non_supported_type = find_non_supported_type_in_file(file);
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut new_generics = item_struct.generics.clone();
                for param in &mut new_generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if is_supported_const_generic_type(&const_param.ty) {
                            if let Some(non_supported_type) = &non_supported_type {
                                const_param.ty = non_supported_type.clone();
                            } else {
                                const_param.ty = create_placeholder_type();
                            }
                            found = true;
                        }
                    }
                }
                item_struct.generics = new_generics;
            }
            if let Item::Trait(item_trait) = item {
                let mut new_generics = item_trait.generics.clone();
                for param in &mut new_generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if is_supported_const_generic_type(&const_param.ty) {
                            if let Some(non_supported_type) = &non_supported_type {
                                const_param.ty = non_supported_type.clone();
                            } else {
                                const_param.ty = create_placeholder_type();
                            }
                            found = true;
                        }
                    }
                }
                item_trait.generics = new_generics;
            }
            if let Item::Fn(item_fn) = item {
                let mut new_generics = item_fn.sig.generics.clone();
                for param in &mut new_generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if is_supported_const_generic_type(&const_param.ty) {
                            if let Some(non_supported_type) = &non_supported_type {
                                const_param.ty = non_supported_type.clone();
                            } else {
                                const_param.ty = create_placeholder_type();
                            }
                            found = true;
                        }
                    }
                }
                item_fn.sig.generics = new_generics;
            }
        }
        if found && non_supported_type.is_none() {
            file.items.push(generate_placeholder_type_declaration());
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets const generic parameters in structs, traits, and functions, replacing their types with non-supported types like user-defined structs. It ensures the presence of a non-supported type in the code, either by finding an existing struct or injecting a placeholder. This transformation stresses the compiler's const generic handling by introducing invalid const parameter types, aiming to trigger ICEs or errors related to const evaluation and type resolution."
    }
}