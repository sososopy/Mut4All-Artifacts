use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, GenericParam::Const, Item,
    ItemFn, ItemStruct, ItemEnum, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, PathSegment, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Replace_Const_Generic_Default_With_Variant_80;

impl Mutator for Replace_Const_Generic_Default_With_Variant_80 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Default_With_Variant_80"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                self.mutate_struct(item_struct);
            }
            if let Item::Enum(item_enum) = item {
                self.mutate_enum(item_enum);
            }
            if let Item::Union(item_union) = item {
                self.mutate_union(item_union);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters with default values in type definitions (structs, enums, unions). It replaces the default expression with a path referencing a variant or associated constant of the same type, creating self-referential dependencies. For enums, it uses variant discriminants via casts; for structs/union with associated constants, it uses those constants. This transformation stresses const evaluation during substitution, potentially triggering cycles or exposing bugs in const generics handling."
    }
}

impl Replace_Const_Generic_Default_With_Variant_80 {
    fn mutate_struct(&self, item_struct: &mut ItemStruct) {
        let type_name = &item_struct.ident;
        let mut has_associated_const = false;
        if let syn::Fields::Named(fields_named) = &item_struct.fields {
            for field in &fields_named.named {
                if let syn::Type::Path(type_path) = &field.ty {
                    if let Some(segment) = type_path.path.segments.last() {
                        if segment.ident == "const" {
                            has_associated_const = true;
                            break;
                        }
                    }
                }
            }
        }
        if !has_associated_const {
            return;
        }
        let generics_clone = item_struct.generics.clone();
        for param in &mut item_struct.generics.params {
            if let GenericParam::Const(const_param) = param {
                if let Some(default_expr) = &mut const_param.default {
                    let new_expr = self.generate_variant_path(type_name, &const_param.ident, &generics_clone, false);
                    if let Some(new_expr) = new_expr {
                        *default_expr = new_expr;
                    }
                }
            }
        }
    }

    fn mutate_enum(&self, item_enum: &mut ItemEnum) {
        let type_name = &item_enum.ident;
        if item_enum.variants.is_empty() {
            return;
        }
        let generics_clone = item_enum.generics.clone();
        for param in &mut item_enum.generics.params {
            if let GenericParam::Const(const_param) = param {
                if let Some(default_expr) = &mut const_param.default {
                    let new_expr = self.generate_variant_path(type_name, &const_param.ident, &generics_clone, true);
                    if let Some(new_expr) = new_expr {
                        *default_expr = new_expr;
                    }
                }
            }
        }
    }

    fn mutate_union(&self, item_union: &mut ItemUnion) {
        let type_name = &item_union.ident;
        let mut has_associated_const = false;
        for field in &item_union.fields.named {
            if let syn::Type::Path(type_path) = &field.ty {
                if let Some(segment) = type_path.path.segments.last() {
                    if segment.ident == "const" {
                        has_associated_const = true;
                        break;
                    }
                }
            }
        }
        if !has_associated_const {
            return;
        }
        let generics_clone = item_union.generics.clone();
        for param in &mut item_union.generics.params {
            if let GenericParam::Const(const_param) = param {
                if let Some(default_expr) = &mut const_param.default {
                    let new_expr = self.generate_variant_path(type_name, &const_param.ident, &generics_clone, false);
                    if let Some(new_expr) = new_expr {
                        *default_expr = new_expr;
                    }
                }
            }
        }
    }

    fn generate_variant_path(&self, type_name: &Ident, const_param_name: &Ident, generics: &Generics, is_enum: bool) -> Option<Expr> {
        let mut generic_args = Punctuated::new();
        for param in generics.params.iter() {
            match param {
                GenericParam::Const(param) => {
                    generic_args.push(GenericArgument::Const(Expr::Path(ExprPath {
                        attrs: Vec::new(),
                        qself: None,
                        path: SynPath::from(param.ident.clone()),
                    })));
                }
                GenericParam::Type(param) => {
                    generic_args.push(GenericArgument::Type(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(param.ident.clone()),
                    })));
                }
                GenericParam::Lifetime(param) => {
                    generic_args.push(GenericArgument::Lifetime(param.lifetime.clone()));
                }
            }
        }
        if is_enum {
            let variant_name = Ident::new("VariantA", Span::call_site());
            let path_segments = Punctuated::from_iter(vec![
                PathSegment {
                    ident: type_name.clone(),
                    arguments: PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                        colon2_token: None,
                        lt_token: token::Lt::default(),
                        args: generic_args,
                        gt_token: token::Gt::default(),
                    }),
                },
                PathSegment {
                    ident: variant_name,
                    arguments: PathArguments::None,
                },
            ]);
            let path_expr = Expr::Path(ExprPath {
                attrs: Vec::new(),
                qself: None,
                path: SynPath {
                    leading_colon: None,
                    segments: path_segments,
                },
            });
            let cast_expr: Expr = parse_quote! { #path_expr as u8 };
            Some(cast_expr)
        } else {
            let const_name = Ident::new("CONST", Span::call_site());
            let path_segments = Punctuated::from_iter(vec![
                PathSegment {
                    ident: type_name.clone(),
                    arguments: PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                        colon2_token: None,
                        lt_token: token::Lt::default(),
                        args: generic_args,
                        gt_token: token::Gt::default(),
                    }),
                },
                PathSegment {
                    ident: const_name,
                    arguments: PathArguments::None,
                },
            ]);
            let path_expr = Expr::Path(ExprPath {
                attrs: Vec::new(),
                qself: None,
                path: SynPath {
                    leading_colon: None,
                    segments: path_segments,
                },
            });
            Some(path_expr)
        }
    }
}