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

pub struct Modify_Trait_Bounds_With_Const_Generics_133;

impl Mutator for Modify_Trait_Bounds_With_Const_Generics_133 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_With_Const_Generics_133"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_exprs = Vec::new();
        let mut type_names = Vec::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                const_exprs.push(item_const.expr.clone());
            }
            if let Item::Type(item_type) = item {
                type_names.push(item_type.ident.clone());
            }
            if let Item::Struct(item_struct) = item {
                type_names.push(item_struct.ident.clone());
            }
            if let Item::Enum(item_enum) = item {
                type_names.push(item_enum.ident.clone());
            }
        }
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for trait_item in &mut item_trait.items {
                    if let TraitItem::Type(assoc_type) = trait_item {
                        if let TypeParamBound::Trait(trait_bound) = &mut assoc_type.bounds[0] {
                            let path = &trait_bound.path;
                            let mut segments = path.segments.clone();
                            let last_segment = segments.last_mut();
                            if let Some(last_segment) = last_segment {
                                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                    for arg in &mut args.args {
                                        if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                            let mut new_segments = type_path.path.segments.clone();
                                            let last_type_segment = new_segments.last_mut();
                                            if let Some(last_type_segment) = last_type_segment {
                                                if let PathArguments::AngleBracketed(type_args) = &mut last_type_segment.arguments {
                                                    if type_args.args.is_empty() {
                                                        let const_expr = if const_exprs.is_empty() {
                                                            parse_quote!(1)
                                                        } else {
                                                            const_exprs[0].clone()
                                                        };
                                                        type_args.args.push(GenericArgument::Const(*const_expr));
                                                        let assigned_type = if type_names.is_empty() {
                                                            parse_quote!(())
                                                        } else {
                                                            let ident = type_names[0].clone();
                                                            parse_quote!(#ident)
                                                        };
                                                        args.args.push(GenericArgument::AssocType(syn::AssocType {
                                                            ident: Ident::new("U", Span::call_site()),
                                                            generics: syn::Generics::default(),
                                                            eq_token: syn::token::Eq::default(),
                                                            ty: assigned_type,
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies trait bounds involving associated types with const generics by inserting explicit const parameter assignments. It scans for trait definitions with associated type bounds, identifies bounds where a const-generic associated type is used without const arguments, and adds a const expression (either from existing constants or a default literal) and an associated type assignment (using an existing type or a unit type). This transformation mimics the pattern that triggered the ICE, exposing bugs in the compiler's handling of const-generic associated types in bounds."
    }
}