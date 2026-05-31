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

pub struct Change_Const_Trait_Method_Param_Type_313;

impl Mutator for Change_Const_Trait_Method_Param_Type_313 {
    fn name(&self) -> &str {
        "Change_Const_Trait_Method_Param_Type_313"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut integer_types = HashSet::new();
        let mut candidate_impls = Vec::new();

        struct IntegerTypeCollector<'a> {
            types: &'a mut HashSet<String>,
        }
        impl<'ast> Visit<'ast> for IntegerTypeCollector<'ast> {
            fn visit_type(&mut self, ty: &'ast Type) {
                if let Type::Path(TypePath { path, .. }) = ty {
                    if let Some(segment) = path.segments.last() {
                        let ident_str = segment.ident.to_string();
                        match ident_str.as_str() {
                            "i8" | "i16" | "i32" | "i64" | "i128" | "isize" |
                            "u8" | "u16" | "u32" | "u64" | "u128" | "usize" => {
                                self.types.insert(ident_str);
                            }
                            _ => {}
                        }
                    }
                }
                syn::visit::visit_type(self, ty);
            }
        }
        let mut collector = IntegerTypeCollector { types: &mut integer_types };
        collector.visit_file(file);

        let mut impl_blocks = Vec::new();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_none() {
                    continue;
                }
                impl_blocks.push(item_impl.clone());
            }
        }

        for impl_block in &impl_blocks {
            let mut local_candidates = Vec::new();
            for impl_item in &mut impl_block.items.iter() {
                if let ImplItem::Fn(method) = impl_item {
                    if method.sig.constness.is_none() {
                        continue;
                    }
                    if method.sig.inputs.len() == 1 {
                        if let FnArg::Typed(PatType { ty, .. }) = &method.sig.inputs[0] {
                            if let Type::Path(TypePath { path, .. }) = &**ty {
                                if let Some(segment) = path.segments.last() {
                                    let ident_str = segment.ident.to_string();
                                    if matches!(ident_str.as_str(),
                                        "i8" | "i16" | "i32" | "i64" | "i128" | "isize" |
                                        "u8" | "u16" | "u32" | "u64" | "u128" | "usize"
                                    ) {
                                        local_candidates.push((method.clone(), ident_str));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            for (method, ident_str) in local_candidates {
                candidate_impls.push((impl_block.clone(), method, ident_str));
            }
        }

        if candidate_impls.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let (impl_block, mut method, original_type) = candidate_impls.choose_mut(&mut rng).unwrap();

        let available_types: Vec<String> = integer_types.iter()
            .filter(|&t| t != original_type)
            .cloned()
            .collect();

        let new_type_ident = if !available_types.is_empty() {
            available_types.choose(&mut rng).unwrap().clone()
        } else {
            let all_int_types = vec![
                "i8", "i16", "i32", "i64", "i128", "isize",
                "u8", "u16", "u32", "u64", "u128", "usize"
            ];
            let new_type = all_int_types.iter()
                .filter(|&&t| t != original_type.as_str())
                .cloned()
                .collect::<Vec<_>>()
                .choose(&mut rng)
                .unwrap()
                .to_string();
            let type_alias = Item::Type(syn::ItemType {
                attrs: Vec::new(),
                vis: syn::Visibility::Inherited,
                type_token: Default::default(),
                ident: Ident::new("NewInt", Span::call_site()),
                generics: Default::default(),
                eq_token: Default::default(),
                ty: Box::new(syn::Type::Path(syn::TypePath {
                    qself: None,
                    path: syn::Path::from(Ident::new(&new_type, Span::call_site())),
                })),
                semi_token: Default::default(),
            });
            file.items.insert(0, type_alias);
            "NewInt".to_string()
        };

        if let FnArg::Typed(PatType { ty, .. }) = &mut method.sig.inputs[0] {
            *ty = Box::new(syn::Type::Path(syn::TypePath {
                qself: None,
                path: syn::Path::from(Ident::new(&new_type_ident, Span::call_site())),
            }));
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_none() {
                    continue;
                }
                if item_impl.self_ty == impl_block.self_ty && item_impl.trait_ == impl_block.trait_ {
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(existing_method) = impl_item {
                            if existing_method.sig.ident == method.sig.ident {
                                *existing_method = method.clone();
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the parameter type of a const trait method implementation to a different integer type while keeping the trait definition unchanged. It first collects all integer types used in the seed program, then selects a const trait method implementation with a single integer parameter. The original parameter type is replaced with another integer type from the program, or if none exists, a new type alias is introduced. This creates a mismatch between the trait signature and implementation, targeting const evaluation inconsistencies and layout computation bugs in generic const expressions."
    }
}